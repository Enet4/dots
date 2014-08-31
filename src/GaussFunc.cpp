/** \file GaussFunc.cpp
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
#include "GaussFunc.h"
#include <cmath>

GaussFunc::GaussFunc(void)
:   size(0)
,   pdf_list(nullptr)
{
}

GaussFunc::GaussFunc(int nIndexes, double n_mean = 0, double n_variance = 1.0, double coef = 1.0)
:	size(nIndexes)
{
	this->pdf_list = new double [nIndexes];
	double k = coef / std::sqrt(2 * PI * n_variance);
	init_prob(n_mean, k, n_variance);
}

GaussFunc::GaussFunc(const GaussFunc& other)
:	size(other.size)
{
    // copy pdf_list
	this->pdf_list = new double [other.size];
    for (int i = 0 ; i < other.size ; i++) {
        this->pdf_list[i] = other.pdf_list[i];
    }
}

GaussFunc::GaussFunc(GaussFunc&& other)
:	size(other.size)
,   pdf_list(other.pdf_list)
{
    // move pdf_list, clear the other list
    other.size = 0;
    other.pdf_list = nullptr;
}

GaussFunc::~GaussFunc()
{
    if (pdf_list) {
        delete [] pdf_list;
        pdf_list = nullptr;
    }
}

GaussFunc& GaussFunc::operator=(const GaussFunc& other) {
    this->size = other.size;
    // copy pdf_list
	this->pdf_list = new double [size];
    for (int i = 0 ; i < size ; i++) {
        this->pdf_list[i] = other.pdf_list[i];
    }
    return *this;
}

GaussFunc& GaussFunc::operator=(GaussFunc&& other) {
    this->size = other.size;
    // move pdf_list, clear the other list
    this->pdf_list = other.pdf_list;
    other.size = 0;
    other.pdf_list = nullptr;

    return *this;
}


void GaussFunc::init_prob(double mean, double k, double var)
{
	if (pdf_list == nullptr)
		return;

	for (int i = 0 ; i < this->size ; i++)
	{
		pdf_list[i] = this->p((double)i - mean, k, var);
	}
}

double GaussFunc::p(double x, double k, double var) const
{
	return k * exp( -0.5 * x*x / var);
}

double GaussFunc::getPDF(int i) const
{
	if (pdf_list != nullptr && i >= 0 && i < this->size)
		return pdf_list[i];
	return 0;
}
