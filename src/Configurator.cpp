/** \file Configurator.cpp
 *
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
//namespace Configurator
#include "Configurator.h"

using namespace std;

bool Configurator::configure(std::unique_ptr<Simulator> & simulator)
{
	//Simulator config variables
	int rand_seed, grid_w = 0, grid_h = 0, init_dots = 0;

	//DotConf create
	DotConf dotconf;

	int varnum = 0;
	ifstream input;

	cout << "Attempting to read " << CONFIG_FILENAME << "..." << endl;

	input.open(CONFIG_FILENAME);

	if (!input)
	{
		cout << "Fail #1" << endl;
		return false;
	}

	while (!input.eof() && varnum < 12)
	{
		switch (varnum)
		{
		case 0:  //rand_seed
			input >> rand_seed;
			cout << "rand_seed: " << rand_seed << endl;
			break;
		case 1: //grid_width
			input >> grid_w;
			cout << "grid_w: " << grid_w << endl;
			break;
		case 2:  //grid_height
			input >> grid_h;
			cout << "grid_h: " << grid_h << endl;
			break;
		case 3:  //init_dots
			input >> init_dots;
			cout << "init_dots: " << init_dots << endl;
			break;
		case 4:  //hunger_chance
			input >> dotconf.hunger_chance;
			cout << "hunger_chance: " << dotconf.hunger_chance << endl;
			break;
		case 5:  //eating_chance_p
			input >> dotconf.dot_density;
			cout << "dot_density: " << dotconf.dot_density << endl;
			break;
		case 6:  //death_chance_maj
			input >> dotconf.death_chance_maj;
			cout << "death_chance_maj: " << dotconf.death_chance_maj << endl;
			break;
		case 7:  //looking_chance_mean
			input >> dotconf.looking_chance_mean;
			cout << "looking_chance_mean: " << dotconf.looking_chance_mean << endl;
			break;
		case 8:  //looking_chance_var
			input >> dotconf.looking_chance_var;
			cout << "looking_chance_var: " << dotconf.looking_chance_var << endl;
			break;
		case 9:  //looking_chance_p
			input >> dotconf.looking_chance_p;
			cout << "looking_chance_p: " << dotconf.looking_chance_p << endl;
			break;
		case 10: //eat_time
			input >> dotconf.eat_time;
			cout << "eat_time: " << dotconf.eat_time << endl;
			break;
		case 11: //generation_time
			input >> dotconf.generation_time;
			cout << "generation_time: " << dotconf.generation_time << endl;
			break;
		}
		varnum++;

		if (input.eof()) break;

		if (input.get() == '#') // comment, skip line
			do
			{
				if (input.eof())
					break;
				input.get();
				input.ignore();
			}
			while(input.get() != '\n');
	}

	dotconf.updateLookProb();

	simulator.reset(new Simulator(rand_seed, dotconf, grid_w, grid_h));

	for (int i = 0 ; i < init_dots ; i++) {
		simulator->addRDot();
	}

	return (varnum == 12);
}
