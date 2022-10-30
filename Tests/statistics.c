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


#include <stdio.h>
#include <math.h>

#include "statistics.h" 

#define _PI		      3.14159265359
#define _RESOLUTION    1024

/*
 *     static variables used when computing integrals
 */
static double _vara = 0.0, _varb = 0.0, _varc = 0.0;



/*
 *     Find the smallest value in a data set.
 *
 *     \param *data  Pointer to the data array.
 *     \param  int   The length of the array.
 *
 *     \return       The minimum value in the data set.
 */
double statistics_min(const double *data, int n)
{
	double m = *data++;
	for (int c = 1; c < n; c++, data++) if (*data < m) m = *data;
	return m;
}



/*
 *     Find the largest value in a data set.
 *
 *     \param *data  Pointer to the data array.
 *     \param  int   The length of the array.
 *
 *     \return       The maximum value in the data set.
 */
double statistics_max(const double *data, int n)
{
	double m = *data++;
	for (int c = 1; c < n; c++, data++) if (*data > m) m = *data;		
	return m;
}



/*
 *     Calculate the difference between the largest and smallest
 *     number in a data set.
 *
 *     \param *data  Pointer to the data array.
 *     \param  int   The length of the array.
 *
 *     \return       The range.
 */
double statistics_range(const double *data, int n)
{
	return statistics_max(data, n) - statistics_min(data, n);
}



/*
 *     Calculate the mean or average of a dat set.
 *
 *     \param *data    Pointer to the data array.
 *     \param  n       The length of the array.
 *
 *     \return         Mean
 */
double statistics_mean(const double *data, int n)
{
	double sum = 0.0;
	for (int c = 0; c < n; c++) sum += *data++;
	return sum / n;
}



/*
 *     Compute the variance of a data set
 *
 *     \param *data       Pointer to an array of doubles, holding the data.
 *     \param  n          Number of entries in the data set.
 *     \param  selection  Use POPULATION when computing the variance of a population.
 *                        Use SAMPLE when computing the variance of a sample.
 *
 *     \return            Variance.
 */
double statistics_var(const double *data, int n, int selection)
{
	double sum = 0.0, m = statistics_mean(data, n);
	for (int c = 0; c < n; c++) sum += pow(*data++ - m, 2);
	return sum / (n - selection);
}



/*
 *     Calculate the variance of a data set.
 *
 *     Compute the variance of a data set
 *
 *     \param *data       Pointer to an array of doubles, holding the data.
 *     \param  n          Number of entries in the data set.
 *     \param  selection  Use POPULATION when computing the variance of a population.
 *                        Use SAMPLE when computing the variance of a sample.
 *
 *     \return            Variance.
 *
 *     Note: This function takes a slightly different approach
 *           to calculate the variance, that may be faster, depending
 *           on the length of the array.
 */
double statistics_varraw(const double *data, int n, int selection)
{
	double A = 0.0, B = 0.0;
	for (int c = 0; c < n; c++) A += (*data * *data), B += *data++;
	return (A - B*B/n) / (n - selection);
}



/*
 *     Calculate the standard deviation of a data set.
 *
 *     \param *data    Pointer to the data array.
 *     \param  n       The length of the array.
 *
 *     \return         Standard deviation 
 */
double statistics_std(const double *data, int n, int selection)
{
	return sqrt(statistics_var(data, n, selection));
}



/*
 *     Calculate the mean and standard deviation of an data set.
 *
 *     \param *mean       Pointer to variable that will receive the mean.
 *     \param *stddev     Pointer to variable that will receive the standard deviation.
 *     \param *data       Pointer to the data array.
 *     \param  n          The length of the array.
 *     \param  selection  Use POPULATION when computing the variance of a population.
 *                        Use SAMPLE when computing the variance of a sample.
 *
 *     Note: If you need both the mean and standard deviation of a dataset, it may be
 *           faster to use this function rather than calculating them individually.
 */
void statistics_ds(double *mean, double *stddev, const double *data, int n, int selection)
{
	double A = 0.0, B = 0.0;
	for (int c = 0; c < n; c++) A += (*data * *data), B += *data++;
	*mean = B / n;
	*stddev = sqrt((A - B*B/n) / (n - selection));
}



/*
 *     Compute the covarience between two sets of data
 *    
 *     \param *x          Pointer to an array of doubles, holding the first set of data.
 *     \param *y          Pointer to an array of doubles, holding the second set of data.
 *     \param  n          Number of entries in the data sets.
 *     \param  selection  Use POPULATION when computing the covariance of a population.
 *                        Use SAMPLE when computing the covariance of a sample.
 *
 *     Note:  x and y should have equally many entries, but none of them should
 *            have fewer than n entries. Ignoring this will cause problems.
 *
 *     Note:  The correlation coefficient is cov(x, y) / std(x)*std(y)
 */
double statistics_cov(const double *x, const double *y, int n, int selection)
{
	double sum = 0.0;
	double mx = statistics_mean(x, n);
	double my = statistics_mean(y, n);
	
	for (int c = 0; c < n; c++) sum += (*x++ - mx) * (*y++ - my);
	return sum / (n - selection);
}



/*
 *     Compute the autocovariance with a specific lag. 
 *
 *     \param *x          Pointer to array with data
 *     \param  size       Length of the array
 *     \param  lag    
 *     \param  selection  Use POPULATION when computing the covariance of a population.
 *                        Use SAMPLE when computing the covariance of a sample.
 *
 *     note:  size means number of elements in the data array, not
 *            number of pairs of xn and xn+lag
 *
 *     note:  population mean is the mean of the population from which
 *            the data sample in *x is drawn from, not necessarily the 
 *            mean of *x.
 */
double statistics_acov(double *x, int size, int lag, int selection)
{
	double m = statistics_mean(x, size);
	double sum = 0.0, *y = x + lag;
	for (int c = 0; c < size - lag; c++) sum += (*x++ - m) * (*y++ - m);
	return sum / (size - lag - selection);
}



/*
 *     Swap the values of two variables.
 */
static void _swap(double *a, double *b)
{
	double temp = *a;
	*a = *b;
	*b = temp;
}



/*
 *     Use Simpson's rule to integrate a function over an interval [a, b].
 * 
 *     \param a     Lower end of interval.
 *     \param b     Upper end of interval.
 *     \param n     Number of sub-intervals. Increasing n gives better accuracy of integral.
 *     \param func  Pointer to the function to integrate.
 *
 *     \return      Estimated integral.
 *
 *     note:        The function to integrate must take a double, or compatible 
 *                  with a double, as argument and return a double compatible.
 *
 *     note:        If func causes a divide by zero, there will be problems.
 */
static double _integral(double a, double b, unsigned int n, double (*func)(double))
{
	if (b == a) return 0.0;						//  if b equals a, the integral is zero
	
	if (n < 2) n = 2;							//  make sure n >= 2
	if (n % 2 != 0) n += 1;						//  ensure n is even
	if (b < a) _swap(&a, &b);						//  ensure a < b
	
	double Sn = func(a) + func(b);				//  Sn is the sum of n sub-intervals
	const double dx = (b - a) / n;				//  the length of the sub-intervals
	unsigned short int m = 4;					//  multiplier alternates between 4 and 2
	
	for (int c = 0; c < n - 1; c++)
	{
		Sn += m * func(a += dx);		
		m ^= 6;
	}
	
	return Sn * (dx / 3);
}



/*
 *     Normal function.
 *
 *     \param x    Function input
 *
 *     \return     The value of N(m, s) at x where m is the mean and s is the standard deviation
 *
 *     Note:       The caller must set the static variables
 *			         _vara = mean
 *			         _varb = standard deviation
 *			         _varc = 1.0 / (std * sqrt(2 * _PI))
 *			    before calling this function.
 */
static double _normal(double x)
{		
	return _varc * exp(-0.5 * pow((x - _vara) / _varb, 2));
}



/*
 *     Cumulative normal distribution with one tail.
 *
 *     \param z   Limit of integration.
 *     \param m   Mean.
 *     \param s   Standard deviation.
 *
 *     \return    Area under the normal distribution, N(m, s), over the 
 *                interval (-inf, z)
 */
double statistics_cmnorm_ot(double z, double m, double s)
{
	_varc = 1.0 / (s * sqrt(2 * _PI));
	_vara = m, _varb = s;
	
	double A = _integral(m, z, _RESOLUTION, _normal);
	A = (z > m) ? A : -A;	
	return 0.5 + A;
}



/*
 *     Cumulative normal distribution with two tails.
 *
 *     \param a   Left limit of integration.
 *     \param b   Right limit of integration.
 *     \param m   Mean.
 *     \param s   Standard deviation.
 *
 *     \return    Area under the normal distribution, N(m, s), over the 
 *                interval (a, b)
 */
double statistics_cmnorm_tt(double a, double b, double m, double s)
{
	_varc = 1.0 / (s * sqrt(2 * _PI));
	_vara = m, _varb = s;
	
	return _integral(a, b, _RESOLUTION, _normal);
}


/*
 *     The integrand of the Gauss error function
 */
static double _erf(double x)
{	
	return exp(-pow(x, 2));
}





/*
 *     The integrand of the incomplete gamma function
 *
 *     Note: _vara must be set to k - 1 before calling this function,
 *           where k is degrees of freedom if used in the cumulative chi 
 *           square function.
 */
static double _igmfc(double t)
{	
	return pow(t, _vara - 1.0) * exp(-t);
}

/*
 *     The lower incomplete gamma function
 *
 *     \param k   A complex number.
 *     \param x   integration limit ()
 *
 *     \return    integral from [0, x] of t^(k - 1) * e^(-t)dt
 *
 *     Note:      The cumulative chi squared function is the incomplete gamma 
 *                function divided by the regularized gamma function. When using
 *                the function is this capacity, k should be df / 2.0 and x should
 *                be X / 2.0.
 */
static double _igamma(double k, double x)
{
	_vara = k;		
		//  resolution must be extremely high for this integral to
		//  be accurate for large values of x
		
	return _integral(0, x, 65536, _igmfc);	
}


/*
 *     Cumulative chi-square function.
 *
 *     \param X   Integration limit.  
 *     \param df  Degrees of freedom.
 *
 *     \return    The integral of the chi-square function with df degrees
 *                of freedom over the interval [0, X].
 *
 *     Note:  The function works best with relatively small values for X and df.
 *            It is advised to keep df under 100, and X under 100000, although, with
 *            smaller values for df, one can get away with larger values for X.
 *            The problem lies with the integrand of the lower incomplete gamma function, 
 *            specifically with the factor t^(df - 1) which can outgrow the bounds of a
 *            double when df gets very large, causing the function to return Nan.
 */
double statistics_cmchisq(double X, unsigned int df)
{
	if (X == 0.0) return 0.0;
	if (df == 0) df++;
	double A;
	switch(df)
	{
		case 1:
		{
			/* 
			 *  When df is 1, the cumulative chi squared fuction can't
			 *  be computed using the incomplete gamma function because
			 *  it would return inf. The incomplete gamma function can, 
			 *  however, when df is 1, be expressed as the Gauss error 
			 *  function. The gamma function in the denominator of the 
			 *  cumulative chi squared distribution reduces to the square
			 *  root of pi which further simplifies the expression.
			 */
			A = _integral(0, sqrt(X/2.0), _RESOLUTION, _erf);
			A *= (2 / sqrt(_PI));
			break;
		}
		case 2:
		{
			/*
			 *  When df is 2, the cumulative chi squared function simplifies
			 *  to this expression
			 */
			A = 1 - exp(-X/2);
			break;
		}
		default:
		{	
			/*
			 *  In general, the cumulative chi squared function is the incomplete
			 *  gamma function divided the gamma function
			 */
			A = _igamma(df / 2.0, X / 2.0);			
			A /= tgamma(df / 2.0);
			break;
		}
	}	
	return A;
}



/*
 *     Perform a chi square goodness of fit test where 
 *     H0: The observed distribution is the same as the expected distribution.
 *     H1: The observed distribution is not the same as the expected distribution.
 *
 *     \param *obs     Pointer to an array of observed frequencies.
 *     \param *exp     Pointer to an array of expected frequencies.
 *     \param  size    Size of the arrays, i.e. number of categroies.
 *     \param *labels  Pointer to an array of character arrays with labels for
 *                     the catagories. This may be NULL. The first element of 
 *                     the array should be the title. 
 *     \return         p-value. 
 *
 *     Note:    The first element of the labels array should be a header for the 
 *              labels. If no header is wanted, it should be set to "".
 *
 *     Note:    p is probability af a type I error if H0 is rejected. 
 *              This probability is represented by the area under the curve right 
 *              of the chi test statistic and can be interpreted as the probability 
 *              of being wrong if one were to reject the null hypothesis, that is 
 *              claiming that the observed distribution is different to the expected 
 *              distribution. 
 *
 *     Example: Suppose the chi_test returned p value is 0.9971. If one were to reject 
 *              the null hypothesis at this level, there would be a 99.71% chance of
 *              being wrong, meaning that it is very likely that the distributions
 *              are the same. It is common to reject the null hypothesis only when the
 *              probability of being wrong, becomes very small, e.g. p < 0.1.  
 */
double statistics_csgof(double *obs, double *exp, int size, char *labels[], double alpha)
{		
	double *o = obs, *e = exp, p, cstat = 0.0;	
	
		//  calculate the chi test statistic and p value
	for (int c = 0; c < size; c++) cstat += pow(*o++ - *e, 2) / *e++;
	p = 1.0 - statistics_cmchisq(cstat, size - 1);	
		
		//  display title	
	printf("\n\n\n    Pearson's chi-square test:\n");
	
		//  display hypotheses	
	printf("\n    H0: Observed frequencies do not differ significantly from expected frequencies.\n");
	printf("    HA: Observed frequencies differ significantly from expected frequencies.\n\n");
	
	                
		//  display test statistics
	printf("       X%c = %8.4f\n", 253, cstat);
	printf("       df = %3d\n", size - 1);
	printf("        P = %8.4f\n\n", p);
	
		//  display table header
	if (labels != NULL) printf("    %-15.15s  ", *labels++);
	else printf("       ");
	printf("observed     expected     residual     component  \n")	;

	
		//  display line under header
	if (labels != NULL) printf("  ---------------");
	else printf("  -----");
	printf("------------------------------------------------------\n");
	
	
		//  display table
	o = obs; e = exp;
	for(int c = 0; c < size; c++)
	{
		if (labels != NULL) printf("    %-16.16s ", *labels++);
		else printf("       ");		
		printf("%8.0f %12.2f %12.2f %13.2f\n", *o, *e, *o - *e, pow(*o++ - *e, 2) / *e++);
	}
	
	
		//  display line under table
	if (labels != NULL) printf("  ---------------");
	else printf("  -----");
	printf("------------------------------------------------------\n");
	
		//  display conclusion
	printf("    Reject H0 (P < %4.2f)?     %3s\n\n", alpha, (p < alpha) ? "YES" : "NO");
	return p;
}