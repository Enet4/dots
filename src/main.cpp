/** \file main.cpp
 *
 *	Dots Simulator
 *	2011-2014
 * \author Eduardo Pinho
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

#include <stdlib.h>
#include <iostream>
#include <string>
#include <GL/freeglut.h>

#include <memory>
#include <list>
#include "Dot.h"
#include "Configurator.h"
#include "DotConf.h"
#include "Simulator.h"

constexpr unsigned int DISPLAY_WIDTH = 512;
constexpr unsigned int DISPLAY_HEIGHT = 512;

constexpr unsigned char KEYCODE_EXIT = 27;
constexpr unsigned char KEYCODE_PAUSE = ' ';
constexpr unsigned char KEYCODE_SPEEDUP = '+';
constexpr unsigned char KEYCODE_SPEEDDOWN = '-';

using namespace std;

// STATIC VARIABLES

static unique_ptr<Simulator> p_sim = nullptr;
static int timebase;
static int speed = 4;

//static int pause_key = 0;

static bool pause = false;

void setOrthographicProjection(int w, int h) {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, w, 0, h);

	// invert the y axis, down is positive
	glScalef(1, -1, 1);

	// move the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -h, 0);

}

void drawDot(const Dot& dot)
{
	float x = (float)dot.getX(), y = (float)dot.getY();
	bool isAlpha = (dot.getType() == DotType::DOT_ALPHA);
	glBegin(GL_QUADS);
	//COLOR CHOICE
		switch (dot.getStatus())
		{
		case STATUS_DEAD:
			glColor3f(0.8f, 0.8f, 0.8f);	//White for dead
			break;
		case STATUS_LOOKING:
			if (isAlpha)
				glColor3f(1.0f, 0.2f, 0.4f);
			else
				glColor3f(0.4f, 0.2f, 1.0f);
			break;
		case STATUS_GENERATING:
			glColor3f(1.0f, 0.2f, 1.0f);	//Magenta for generating
			break;
		case STATUS_HUNGRY:
			if (isAlpha)
				glColor3f(0.5f, 0.1f, 0.0f);
			else
				glColor3f(0.0f, 0.1f, 0.5f);
			break;
		case STATUS_EATING:
			if (isAlpha)
				glColor3f(0.8f, 0.4f, 0.1f);
			else
				glColor3f(0.1f, 0.4f, 0.8f);
			break;
		default:
			if (isAlpha)
				glColor3f(1.0f, 0.1f, 0.0f);
			else
				glColor3f(0.0f, 0.1f, 1.0f);
			break;
		}
		glVertex2f(x,	y);
		glVertex2f(x+1,	y);
		glVertex2f(x+1,	y+1);
		glVertex2f(x,	y+1);

	glEnd();
}

void renderScene() {

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Iterate here to get all dots from the simulator

    auto dots = p_sim->getDots();
	for (auto it = begin(dots) ; it != end(dots) ; ++it) {
		drawDot(it->second);
	}

	glFlush();
}

void resize_win(int width, int height)
{
	if(height == 0)
		height = 1;
	if(width == 0)
		width = 1;

	// Set the viewport to be the entire window
    glViewport(0, 0, width, height);
}

void quit()
{
	exit(0);
}

void checkKeys(unsigned char key, int x, int y)
{
	if (key == KEYCODE_EXIT)
		quit();

	if (key == KEYCODE_PAUSE)
	{
		pause = !pause;
		if (pause)
		{
			std::cout	<< "- SIMULATION PAUSED -" << std::endl
				<< "Frame Nr: " << p_sim->getFrame() << std::endl
				<< "Number of live Dots: " << p_sim->ndots() << std::endl
				<< "Number of dead Dots so far: " << p_sim->getNDeaths() << std::endl
				<< "Average Age of Death so far: " << p_sim->getDeathAverage() << std::endl
				<< "Maximum Dot Age so far: " << p_sim->getMaxAge() << std::endl
				<< "Maximum nr. of Live Dots so far:" << p_sim->getMaxDots() << std::endl;
		}
		else
		{
			std::cout << "- SIMULATION RESUMED -" << std::endl;
			timebase = glutGet(GLUT_ELAPSED_TIME);
		}
	}

	if (key == KEYCODE_SPEEDUP)
	{
		if ( (glutGetModifiers() & GLUT_ACTIVE_SHIFT) == GLUT_ACTIVE_SHIFT )
			speed += 10;
		else
			speed++;

		cout << "- Simulation speed increased to " << speed << " steps per second. " << std::endl;
	}

	if (key == KEYCODE_SPEEDDOWN)
	{
		int s = ((glutGetModifiers() & GLUT_ACTIVE_SHIFT) == GLUT_ACTIVE_SHIFT) ? 10 : 1;
		if (speed > s)
		{
			speed -= s;
			cout << "- Simulation speed decreased to " << speed << " steps per second. " << std::endl;
		}
	}
}

void idleLoop(void)
{
	if (!pause)
	{
		int time = glutGet(GLUT_ELAPSED_TIME);

		if (time - timebase > 1000.0 / speed) {
			p_sim->step();
            if (p_sim->ndots() == 0) {
            cout    << " All dots are dead! " << endl
					<< "Frame Nr: " << p_sim->getFrame() << endl
					<< "Number of dead Dots: " << p_sim->getNDeaths() << endl
					<< "Average Age of Death: " << p_sim->getDeathAverage() << endl
					<< "Maximum Dot Age:" << p_sim->getMaxAge() << endl
					<< "Maximum nr. of Live Dots:" << p_sim->getMaxDots() << endl
					<< " Press Esc to leave." << endl;
            }
			timebase = time;
			renderScene();
		}
	}
}

int main(int argc, char** argv)
{
	std::cout << "Welcome to Dots!\n version 1.1" << std::endl;

	glutInit(&argc, argv);

	glutInitWindowPosition(-1, -1);

	glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutCreateWindow("Dots Simulator");

	glutDisplayFunc(renderScene);
	glutIdleFunc(idleLoop);
	glutReshapeFunc(resize_win);
	glutKeyboardFunc(checkKeys);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	timebase = glutGet(GLUT_ELAPSED_TIME);

	//Configure DotConf & Simulator
	bool configure_ok = Configurator::configure(p_sim);
	if (!configure_ok)
	{
		std::cerr << "Program failed: Cannot read config.txt" << std::endl;
		return -1;
	}

	setOrthographicProjection(p_sim->getWidth(), p_sim->getHeight());

	// Set the viewport to be the entire window
    glViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

	glutMainLoop();

	quit();
	return 0;
}
