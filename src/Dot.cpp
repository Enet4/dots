/** \file Dot.cpp
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
//Class Dot
//enum class DotType
//enum DotStatus
#include "Dot.h"

#include <array>

unsigned int Dot::current_id = 0;

Dot::Dot(void)
:   p_dotconf(nullptr)
,   id(0)
,	x(0), y(0)
,   count(0), age(0), type(DotType::DOT_ALPHA)
,	status(STATUS_INVALID), partner(0), has_partner(false)
{
}

Dot::Dot(unsigned int id, int nX, int nY, DotType ntype, const DotConf& dconf)
:   p_dotconf(&dconf)
,   id(id)
,	x(nX), y(nY)
,   count(0), age(0), type(ntype)
,	status(STATUS_NORMAL), partner(0), has_partner(false)
{
}

Dot Dot::create(int nX, int nY, DotType ntype, const DotConf& dconf)
{
    return Dot(current_id++, nX, nY, ntype, dconf);
}

Dot::~Dot()
{
}

unsigned int Dot::getID() const { return this->id; }

int Dot::getX() const {	return this->x; }

int Dot::getY() const {	return this->y; }

DotType Dot::getType() const {	return this->type; }

DotStatus Dot::getStatus() const {	return this->status; }

void Dot::setStatus(DotStatus nstatus) {
	this->status = nstatus;
}

unsigned int Dot::getAge() const { return this->age; }

int Dot::getCount() const {	return this->count; }

void Dot::resetCount() {
	this->count = 0;
}

void Dot::incCount() {
	this->count++;
}

unsigned int Dot::getPartnerId() const {
	return this->partner;
}

bool Dot::hasPartner() const {
    return this->has_partner;
}

void Dot::setPartner(const Dot& npartner) {
	this->partner = npartner.getID();
	this->has_partner = true;
}

void Dot::resetPartner(void) {
	this->has_partner = false;
}

unsigned int Dot::incAge() {
	return ++this->age;
}

bool Dot::operator==(const Dot& other) const {
	return this->getID() == other.getID();
}

bool Dot::operator!=(const Dot& other) const {
	return this->getID() != other.getID();
}

void Dot::updateCDF(double pdensity) {
    if (this->status == STATUS_INVALID) return;

    constexpr int N = 6; // 6 stands for six possible Dot states

	//double tmp;
	std::array<double,N> pdf;

    pdf.fill(0);

	//death - 1
	pdf[1] = (double)this->age/this->p_dotconf->death_chance_maj;
	pdf[1] *= pdf[1]; //squared

	switch (this->status)
	{
	//default:

	case STATUS_NORMAL:
		//Null Probability:
		//	eating - 4
		//	generating - 5

		//hungry - 2
		pdf[2] = (1-pdf[1]) * this->p_dotconf->hunger_chance;

		//looking - 3
		pdf[3] = (1-pdf[1]-pdf[2]) * this->p_dotconf->look_prob.getPDF(this->age);

		//normal - 0
		pdf[0] = 1 - pdf[1] - pdf[2] - pdf[3];

	break;
	case STATUS_HUNGRY:
	{
		//Null Probability:
		//	normal - 0
		//	looking - 3
		//	generating - 5

		//eating - 4
		pdf[4] = (1 - pdf[1]) * (1 / (pdensity + 1));

		//hungry - 2
		pdf[2] = 1 - pdf[1] - pdf[4];

	}
	break;
	case STATUS_LOOKING:
		//Null Probability:
		//	get hungry - 2
		//	eating - 4
		//	normal - 0
		//	PROCEDURAL: generating - 5

		// searching dot - 3
		pdf[3] = 1 - pdf[1];
	break;
	case STATUS_EATING:
		//Null Probability:
		//	PROCEDURAL: normal - 0
		//	get hungry - 2
		//	get looking - 3
		//	generating - 5

		//eating - 4
		pdf[4] = 1 - pdf[1];
	break;
	case STATUS_GENERATING:
		//Null Probability:
		//	PROCEDURAL: normal - 0
		//	get hungry - 2
		//	get looking - 3
		//	eating - 4

		//generating - 5
		pdf[5] = 1 - pdf[1];

	break;
    default:
    break;
	}

	//set cdf
	RandGenerator::pdf2cdf(pdf.data(), this->status_cdf.data(), N);
}

void Dot::move(int d)
{
    if (this->status == STATUS_INVALID) return;
	switch (d)
	{
	case 0:
		this->x++;
		break;
	case 1:
		this->y--;
		break;
	case 2:
		this->x--;
		break;
	case 3:
		this->y++;
		break;
	}
}

void Dot::setPos(int nx, int ny)
{
    if (this->status == STATUS_INVALID) return;
	this->x = nx;
	this->y = ny;
}

std::ostream& Dot::report(std::ostream& stream) const
{
	return stream << "Report of dot #" << this->id << ":"
			"\nAge: " << this->age <<
			"\nType: " << this->typeToString() <<
			"\nCurrent status: " << this->statusToString() <<
			"\nCount: " << this->count <<
			"\n----------------------\n";
}

const char* Dot::typeToString() const
{
	switch (this->type)
	{
		case DotType::DOT_ALPHA: return "Alpha";
		case DotType::DOT_BETA: return "Beta";
		default: return "HAX";
	}
}

const char* Dot::statusToString() const
{
	switch (status)
	{
		case STATUS_NORMAL: return "Normal";
		case STATUS_DEAD: return "Dead";
		case STATUS_HUNGRY: return "Hungry";
		case STATUS_EATING: return "Eating";
		case STATUS_LOOKING: return "Looking";
		case STATUS_GENERATING: return "Generating";
		default: return "HAX";
	}
}

const double* Dot::getCDF()
{
	return this->status_cdf.data();
}
