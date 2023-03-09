/*
 * statistics.h
 *
 * Created: 15. May. 2022
 *  Author: Frank Bjørnø
 *
 * Purpose: 
 *      Provide funstions for statistical analysis of data.
 *
 * License:
 * 
 *          Copyright (C) 2022 Frank Bjørnø
 *
 *          1. Permission is hereby granted, free of charge, to any person obtaining a copy 
 *          of this software and associated documentation files (the "Software"), to deal 
 *          in the Software without restriction, including without limitation the rights 
 *          to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
 *          of the Software, and to permit persons to whom the Software is furnished to do 
 *          so, subject to the following conditions:
 *        
 *          2. The above copyright notice and this permission notice shall be included in all 
 *          copies or substantial portions of the Software.
 *
 *          3. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 *          INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 *          PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 *          HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
 *          CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
 *          OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#pragma once

typedef enum {ST_POPULATION = 0, ST_SAMPLE} SELECTION;

	//  descriptive statistics
double statistics_min(const double *data, int n);
double statistics_max(const double *data, int n);
double statistics_range(const double *data, int n);

double statistics_mean(const double *data, int n);
double statistics_var(const double *data, int n, SELECTION sel);
double statistics_var_raw(const double *data, int n, SELECTION sel);
double statistics_std(const double *data, int n, SELECTION sel);
void   statistics_ds(double *mean, double *stddev, const double *data, int n, SELECTION sel);
double statistics_cov(const double *x, const double *y, int n, SELECTION sel);
double statistics_acov(double *x, int size, int lag, SELECTION sel);

	//  distributions
double statistics_cmnorm_ot(double z, double m, double s);
double statistics_cmnorm_tt(double a, double b, double m, double s);
double statistics_cmchisq(double X, unsigned int df);

	//  tests
double statistics_csgof(double *obs, double *exp, int size, char *labels[], double alpha);