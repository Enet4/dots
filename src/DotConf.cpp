/** \file DotConf.cpp
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
//Class DotConf
#include "DotConf.h"

using namespace std;

DotConf::DotConf()
:	hunger_chance(0.05),
	dot_density(4.0),
	death_chance_maj(1000.0),
	looking_chance_mean(150.0),
	looking_chance_var(16.0),
	eat_time(3),
	generation_time(4),
	look_prob()
{
    updateLookProb();
}

DotConf::DotConf(const DotConf& other)
:	hunger_chance(other.hunger_chance),
	dot_density(other.dot_density),
	death_chance_maj(other.death_chance_maj),
	looking_chance_mean(other.looking_chance_mean),
	looking_chance_var(other.looking_chance_var),
	eat_time(other.eat_time),
	generation_time(other.generation_time),
	look_prob(other.look_prob)
{
}

DotConf::DotConf(DotConf&& other)
:	hunger_chance(other.hunger_chance),
	dot_density(other.dot_density),
	death_chance_maj(other.death_chance_maj),
	looking_chance_mean(other.looking_chance_mean),
	looking_chance_var(other.looking_chance_var),
	eat_time(other.eat_time),
	generation_time(other.generation_time),
	look_prob(other.look_prob)
{
}

void DotConf::updateLookProb(void)
{
    this->look_prob = GaussFunc((int)death_chance_maj + 1,
                looking_chance_mean,
                looking_chance_var,
                looking_chance_p);
}
