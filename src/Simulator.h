/** \file Simulator.h
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
#ifndef Simulator_H
#define Simulator_H

#include <map>
#include <set>
#include "Dot.h"
#include "DotConf.h"
#include "RandGenerator.h"
#include <ostream>

class Simulator
{
private:
    std::map<unsigned int, Dot> dots;

	int grid_w;
	int grid_h;

	unsigned int n_frame;

	unsigned int stat_age_total;
	unsigned int stat_deaths_total;
	unsigned int stat_max_age;
	unsigned int stat_max_dots;

	DotConf dconfig;

public:
    using DotMap = std::map<unsigned int, Dot>;

	Simulator(unsigned int rseed, const DotConf& dconfig, int nw, int nh);
	~Simulator();

	int getWidth() const noexcept;
	int getHeight() const noexcept;

	unsigned int getFrame() const;

	unsigned int addDot(int x, int y, DotType type);
	unsigned int addRDot(int x, int y);
	unsigned int addRDot();

	void step();

	unsigned int ndots() const;
	DotMap getDots(void) const;

	double getDeathAverage() const;
	double getNDeaths() const;
	unsigned int getMaxAge() const;
	unsigned int getMaxDots() const;

private:
	void stepDot(Dot& cdot, const DotMap& dots_copy, std::set<unsigned int>& generated);

	void randWalk(Dot& dot) const;
	double distSqr(const Dot& d1, const Dot &d2) const;

	double pop_density(const Dot& d, const DotMap& dots_copy) const;

    /** Get a reference to the nearest dot of d1 with the opposite
     * type and a non-busy state (either normal or looking)
     * \param d1
     * \param dots_copy
     * \return the nearest opposing dot of d1, or nullptr if no other dot is available.
     */
	const Dot* nearestOppOf(const Dot& d1, const DotMap& dots_copy) const;

	bool stepToNearest(Dot& d1, const DotMap& dots_copy);
	void stepTo(Dot& d1, const Dot& d2) const;
};


#endif
