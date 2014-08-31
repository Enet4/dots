/** \file GaussFunc.h
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
#ifndef GaussFunc_H

#define GaussFunc_H

// f(x) = 1 / (2 * pi * var) * e^-(x - mean)^2 / (2 * var) )
// var = std_deviation^2 = E[X^2] - (E[X])^2

// mean = 0
// var = 1
// => f(x) = 0.5 * pi * e^-(0.5x^2)

class GaussFunc
{
private:
	int size;
	double * pdf_list;
	double p(double x, double k, double var) const;
	void init_prob(double mean, double k, double var);
public:
    GaussFunc(void);
	GaussFunc(int nIndexes, double mean, double variance, double coef);
	GaussFunc(const GaussFunc& other);
	GaussFunc(GaussFunc&& other);
	~GaussFunc();

	GaussFunc& operator=(const GaussFunc& other);
	GaussFunc& operator=(GaussFunc&& other);

	double getPDF(int i) const;
	int getSize(void) const;

    static constexpr long double PI = 3.141592653589793238462643383;
};

#endif
