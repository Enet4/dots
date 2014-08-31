/** \file Simulator.cpp
 * Copyright (C) 2014 Eduardo Pinho (enet4mikeenet AT gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 * KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
//Class Simulator
#include "Simulator.h"

using namespace std;

Simulator::Simulator(unsigned int rseed, const DotConf& dotconfig, int nw = 64, int nh = 64)
:	dots(),
    grid_w(nw),
	grid_h(nh),
	n_frame(0),
	stat_age_total(0),
	stat_deaths_total(0),
	stat_max_age(0),
	stat_max_dots(0),
	dconfig(dotconfig)
{
	RandGenerator::set_seed(rseed);
}

Simulator::~Simulator()
{
}

unsigned int Simulator::addDot(int x, int y, DotType type)
{
	Dot d = Dot::create(x, y, type, dconfig);
	auto id = d.getID();
	dots[id] = d;
	return id;
}

unsigned int Simulator::addRDot()
{
	int x = rand() % grid_w;
	int y = rand() % grid_h;
	return addRDot(x,y);
}

unsigned int Simulator::addRDot(int x, int y)
{
	int st = (rand() & 1);
    return addDot(x, y, (st==0) ? DotType::DOT_ALPHA : DotType::DOT_BETA);
}

void Simulator::step()
{
    // Pre-filter dead dots
	for(auto it = begin(dots) ; it != end(dots) ; ) {

		// If status = STATUS_DEAD, remove dot from the list
		if (it->second.getStatus() == STATUS_DEAD) {
			dots.erase(it);
			it = begin(dots);
		} else
            ++it;
	}

    // take a copy of the current status (everything is copy constructed)
    auto dots_copy = dots;
    set<unsigned int> generated;

    auto deaths = 0u;
	for(auto it = begin(dots) ; it != end(dots) ; ++it) {
//	    const unsigned int id = it->first;
        Dot& dot = it->second;

		this->stepDot(dot, dots_copy, generated);

		if (dot.getStatus() == STATUS_DEAD)
		{
			this->stat_age_total += dot.getAge();
			this->stat_deaths_total += 1;
			deaths++;
		}

	}
    auto living_dots = dots.size()-deaths;
	if (stat_max_dots < living_dots)
		stat_max_dots = living_dots;

	this->n_frame++;
}

void Simulator::stepDot(Dot& dot, const DotMap& dots_copy, set<unsigned int>& generated)
{
    auto& cdot = dot;
//    const auto& cdot_prev = (dots_copy.find(dot.getID()) != end(dots_copy))
//            ? dots_copy[dot.getID()] : dot;

	bool walk = true;
    // Dot simulation proceedings for each dot:

    // 1. Check partner
    const Dot* p_partner = nullptr;
    if (dot.hasPartner()) {
        p_partner = &dots_copy.find(dot.getPartnerId())->second;

        if (p_partner->getStatus() == STATUS_DEAD) {
            // disband from partner
            dot.resetPartner();
            dot.resetCount();
            dot.setStatus(STATUS_NORMAL);
            p_partner = nullptr;
        }
    }

    //	2. Calculate probability matrix
    cdot.updateCDF(pop_density(dot, dots_copy));

    //	3. Perform a roll, apply new status
    //		3.1. If new status = STATUS_EATING -> Set count = 1
    //		3.2. If new status = STATUS_DEAD -> Don't walk!
    bool prevIsEating = (dot.getStatus() == STATUS_EATING);

    cdot.setStatus(static_cast<DotStatus>(RandGenerator::genvar(cdot.getCDF())));

    if (!prevIsEating && (cdot.getStatus() == STATUS_EATING)) {
        cdot.resetCount();
        cdot.incCount();
    }
    if (cdot.getStatus() == STATUS_DEAD) {
        walk = false;
        cdot.resetCount();
        cdot.resetPartner();
        return;
    }
    //	5. If status = STATUS_EATING
    //		5.1. If count == eating_time
    //			5.1.1. change to STATUS_NORMAL
    //			5.1.2. reset count
    //		Else
    //			5.1.1. count++
    //			5.1.2. Don't walk!
    if (cdot.getStatus() == STATUS_EATING) {
        if (dot.getCount() == dconfig.eat_time) {
            cdot.setStatus(STATUS_NORMAL);
            cdot.resetCount();
        } else {
            cdot.incCount();
            walk = false;
        }
    }


    //	6. If status is STATUS_GENERATING
    //		6.1. If count == generation_time
    //			6.1.1. Set both dots' status to STATUS_NORMAL
    //			6.1.2. Set count = 0 to both dots
    //			6.1.3. Create a new dot of uniformly random type at same position
    //		Else
    //			6.1.1. count++
    //			6.1.1. Don't walk!
    if (cdot.getStatus() == STATUS_GENERATING) {
//        if (cdot.getCount() == 0) {
//            //this means the dot was either already generated
//            //by its partner or the partner died
//            cdot.resetPartner();
//            cdot.setStatus(STATUS_NORMAL);
//        }
        if (cdot.getCount() == dconfig.generation_time)
        {
            cdot.setStatus(STATUS_NORMAL);
            cdot.resetCount();
            cdot.resetPartner();

            if (generated.find(cdot.getID()) == end(generated)) {
                //create a new dot
                this->addRDot(cdot.getX(), cdot.getY());
                // mark it as generated
                generated.insert(cdot.getID());
            }
        } else {
            cdot.incCount();
            walk = false;
        }
    }

    //	7. If generation condition is met:
    //		7.1. Change both dots' status to STATUS_GENERATING
    //		7.2. Set count = 1 to both dots
    //		7.4. Don't walk!
    if (cdot.getStatus() == STATUS_NORMAL || cdot.getStatus() == STATUS_LOOKING)
    {
        const Dot* p = nearestOppOf(cdot, dots_copy);
        if (p == nullptr) {
            if (cdot.getStatus() == STATUS_LOOKING) {
                // stop looking, there's no dot to look for
                cdot.setStatus(STATUS_NORMAL);
                cdot.resetCount();
            }
        } else if (cdot.getType() != p->getType()) {
            const Dot& odot = *p;
            if (distSqr(cdot, odot) == 0 && cdot != odot &&
                (   cdot.getStatus() == STATUS_LOOKING
                 || odot.getStatus() == STATUS_LOOKING)) {
                // encounter!
                cdot.setStatus(STATUS_GENERATING);
                cdot.resetCount();
                cdot.incCount();
                cdot.setPartner(odot);
                walk = false;
            }

            // TURTLE SOLUTION
            if (distSqr(cdot, *p) < 2
                && odot.getStatus() == STATUS_LOOKING
                && odot.getType() != cdot.getType()) {
                // do not walk, let the partner do it
                    if (cdot.getType() == DotType::DOT_ALPHA) {
                        // alpha do the X stepping
                        if (cdot.getX() == odot.getX())
                            walk = false;
                    } else {
                        // beta do the Y stepping
                        if (cdot.getY() == odot.getY())
                            walk = false;
                    }
            }
        }
    }

    //	8. Perform walk: If status = STATUS_LOOKING
    //		8.1. Step closer to Nearest Opposite Dot
    //		8.2. If there is no other opposite Dot, return to STATUS_NORMAL
    //		Else
    //		8.1. Random Walk
    if (walk) {
        if (cdot.getStatus() == STATUS_LOOKING) {
            if (!stepToNearest(cdot, dots_copy)) {
                cdot.setStatus(STATUS_NORMAL);
                cdot.resetCount();
                randWalk(cdot);
            }
        } else
            randWalk(cdot);
    }

    //	8. Increment Dot age.
    cdot.incAge();

    if (this->stat_max_age < cdot.getAge() && cdot.getStatus() != STATUS_DEAD)
        this->stat_max_age = cdot.getAge();
}

int Simulator::getWidth() const noexcept
{
	return this->grid_w;
}
int Simulator::getHeight() const noexcept
{
	return this->grid_h;
}

unsigned int Simulator::ndots() const
{
	return dots.size();
}

double Simulator::getDeathAverage() const
{
	return (double)this->stat_age_total / this->stat_deaths_total;
}

double Simulator::getNDeaths() const
{
	return this->stat_deaths_total;
}

unsigned int Simulator::getMaxAge() const
{
	return this->stat_max_age;
}

unsigned int Simulator::getMaxDots() const
{
	return this->stat_max_dots;
}

unsigned int Simulator::getFrame() const
{
	return this->n_frame;
}

double Simulator::distSqr(const Dot& d1, const Dot &d2) const
{
	int dx = abs(d1.getX() - d2.getX());
	if ( dx > this->grid_w/2 )
		dx = this->grid_w - dx;

	int dy = abs(d1.getY() - d2.getY());
	if ( dy > this->grid_h/2 )
		dy = this->grid_h - dy;

	return (double)(dx*dx + dy*dy);
}

void Simulator::randWalk(Dot& dot) const
{
	const int w = this->getWidth();
	const int h = this->getHeight();
	double cprob[] = { 0.25, 0.5, 0.75, 1 };
	int d = RandGenerator::genvar(cprob);
	dot.move(d);
	const int x = dot.getX(), y = dot.getY();
	dot.setPos((x+w) % w, (y+h) % h);

}

double Simulator::pop_density(const Dot& this_dot, const DotMap& dots_copy) const
{
	double tmp = 0;

	for(auto it = begin(dots_copy); it != end(dots_copy) ; ++it) {
        const auto& d = it->second;
		if (this_dot == d)
			continue;

		tmp += dconfig.dot_density / (distSqr(this_dot, d));
	}
	return tmp;
}

const Dot* Simulator::nearestOppOf(const Dot& d1, const map<unsigned int, Dot>& dots_copy) const
{
	const Dot* ndot = nullptr;
	const DotType t = d1.getType();

    auto it = begin(dots_copy);

	double nd = grid_w*grid_w + grid_h*grid_h;

    ndot = &(it->second);
	++it;

	for( ; it != end(dots_copy); ++it) {
	    const Dot& od = it->second;
		if (d1 == od
			|| od.getType() == t
			|| (   od.getStatus() != STATUS_NORMAL
                && od.getStatus() != STATUS_LOOKING) )
				continue;

		double tdist = distSqr(d1, od);
		if (tdist < nd) {
			nd = tdist;
			ndot = &od;
		}
	}

	return ndot;
}

void Simulator::stepTo(Dot& d1, const Dot& d2) const
{
	if (d1.getX() == d2.getX() && d1.getY() == d2.getY())
		return;

	int dx = abs(d1.getX() - d2.getX());
	if ( dx > this->grid_w/2 )
		dx = this->grid_w - dx;

	int dy = abs(d1.getY() - d2.getY());
	if ( dy > this->grid_h/2 )
		dy = this->grid_h - dy;

    if (dx == dy) {
        if (d1.getType() == DotType::DOT_ALPHA)
            d1.move( (d2.getX() > d1.getX()) ? 0 : 2 ); // Alpha prioritizes X
        else
            d1.move( (d2.getY() > d1.getY()) ? 3 : 1 ); // Beta prioritizes Y
    }
	if (dx > dy)
		d1.move( (d2.getX() > d1.getX()) ? 0 : 2 ); // right or left
	else
		d1.move( (d2.getY() > d1.getY()) ? 3 : 1 ); // down or up

	d1.setPos(d1.getX() % this->getWidth(), d1.getY() % this->getHeight());
}

bool Simulator::stepToNearest(Dot& d1, const DotMap& dots_copy)
{
	const Dot* p_t_d = nearestOppOf(d1, dots_copy);
	if (p_t_d == nullptr)
		return false;

	stepTo(d1, *p_t_d);
	return true;
}

Simulator::DotMap Simulator::getDots(void) const
{
    // just let it copy
    return this->dots;
}
