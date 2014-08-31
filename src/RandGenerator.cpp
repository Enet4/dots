/** \file RandGenerator.cpp
 * \author Eduardo Pinho
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

//namespace RandGenerator
#include "RandGenerator.h"
#include <cassert>
#include <stdlib.h>


void RandGenerator::set_seed(unsigned int rand_seed)
{
	srand(rand_seed);
}

void RandGenerator::pdf2cdf(const double* pdf, double *cdf, int n)
{
	cdf[0] = pdf[0];
	for (int i = 1 ; i < n ; i++)
		cdf[i] = cdf[i-1] + pdf[i];
	assert (cdf[n-1] = 1);
}

int RandGenerator::genvar(const double* cdf)
{
	double u = (double)rand() / RAND_MAX;
	int i = 0;
	while (cdf[i] < u)
	{
		i++;
	}
	return i;
}
