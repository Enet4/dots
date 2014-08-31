/** \file Dot.h
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
#pragma once
#ifndef Dot_H
#define Dot_H

#include "DotConf.h"
#include "GaussFunc.h"
#include "RandGenerator.h"
#include <stdlib.h>
#include <ostream>
#include <array>

enum DotStatus : int
{
	STATUS_INVALID		= -1,
	STATUS_NORMAL		= 0,
	STATUS_DEAD			= 1,
	STATUS_HUNGRY		= 2,
	STATUS_LOOKING		= 3,
	STATUS_EATING		= 4,
	STATUS_GENERATING	= 5
};

enum class DotType : bool
{
	DOT_ALPHA,
	DOT_BETA
};

class Dot
{

private:
	static unsigned int current_id;

    /** Reference to Dot configurations. */
    const DotConf* p_dotconf;

    /** Dot's ID */
	unsigned int id;
    /** x position in the world */
	unsigned int x;
    /** y position in the world */
	unsigned int y;
    /** The Dot's counter (for eating and generating). */
	int count;
    /** The Dot's age */
	unsigned int age;
    /** State transition chances. */
	std::array<double,6> status_cdf;
    /** Dot type (Alpha / Beta) */
	DotType type;
    /** The Dot's current status (state) */
	DotStatus status;
    /** ID of the generation parter. */
	unsigned int partner;
	/** Whether it has a partner. */
	bool has_partner;

public:
    /** Default constructor. */
	Dot(void);
    /** Main constructor. */
	Dot(unsigned int id, int nX, int nY, DotType ntype, const DotConf& dconf);
    /** Copy constructor. */
    Dot(const Dot& other) = default;
    /** Move constructor. */
	Dot(Dot&& other) = default;

    /** Destructor. */
	~Dot();

	/** Copy assignment operator. */
    Dot& operator=(const Dot& other) = default;

	/** Move assignment operator. */
    Dot& operator=(Dot&& other) = default;

    /** Dot ID getter. */
	unsigned int getID() const;
	/** Dot X getter. */
	int getX() const;
	/** Dot Y getter. */
	int getY() const;
	/** Dot age getter. */
	unsigned int getAge() const;

	/** Increment dot age by 1.
	 * \return the dot's age after incrementing
	 */
	unsigned int incAge();

    /** Dot's counter value getter.
     */
	int getCount() const;

	/** Reset the dot's counter, by setting it to <b>0</b>. */
	void resetCount();

	/** Increment the dot's counter by 1. */
	void incCount();

	/** Dot type getter. */
	DotType getType() const;

	/** Dot status getter. */
	DotStatus getStatus() const;

	/** Dot status setter. */
	void setStatus(DotStatus nstatus);

	/** Get ID of Dot's partner.
	 * \return a dot ID, only makes sense if it has a partner.
	 */
	unsigned int getPartnerId() const;

	/** Check for partner.
	 * \return whether the dot has a partner.
	 */
	bool hasPartner(void) const;

	/** Dot's partner setter. */
	void setPartner(const Dot& npartner);

	/** Dot's partner resetter (clears partner). */
	void resetPartner(void);

	const double* getCDF();

    /** Compares two dots by ID. */
	bool operator== (const Dot& other) const;

    /** Compares two dots by ID. */
	bool operator!= (const Dot& other) const;

    /** Set Dot position. */
	void setPos(int nx, int ny);

    /** Move Dot a specific direction.
     * \param d
     * <b>0</b> - right<br>
     * <b>1</b> - up<br>
     * <b>2</b> - left<br>
     * <b>3</b> - down<br>
     */
	void move(int d);

	void updateCDF(double pdensity = 0);

	std::ostream& report(std::ostream& stream) const;

	const char* typeToString() const;
	const char* statusToString() const;

    static Dot create(int nX, int nY, DotType ntype, const DotConf& dconf);
};

#endif
