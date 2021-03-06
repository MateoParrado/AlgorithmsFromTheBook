#pragma once
#include <utility>

bool isEven(int x)
{
	for (;;) 
	{
		if (!x) return true;
		if (x == 1) return false;

		x *= x;
	}
}

template<class T>
//returns the distance between two coord pair points
double _fastcall dist(std::pair<T, T> * p1, std::pair<T, T> * p2)
{
	return sqrt((p1->first - p2->first)*(p1->first - p2->first) + (p1->second - p2->second)*(p1->second - p2->second));
};

//recursive exponentiation
int _fastcall exponent(int base, int exp) 
{
	if (!exp) return 1;
	if (exp == 1) return base;
	if (exp == 2) return base * base;
	return exponent(base, exp / 2) * exponent(base, exp - exp / 2);
}

//has to be double bc otherwise x + 0.001 == x
double _fastcall derivative(double(*func)(double), double pos) 
{
	return (func(pos + 0.0000001) - func(pos - 0.0000001)) / 0.0000002;
}

//traps is the numeber of trapezoids it is broken into
double integral(double(*func)(double), double start, double end, unsigned int traps)
{
	//should we switch the vakue of the integral?
	int retMultiplier = 1;
	
	if (end < start)
	{
		retMultiplier = -1;

		double tempStart = start;
		start = end;
		end = tempStart;
	}


	double step = (end - start) / traps;

	double cache = func(start);

	double ret = 0;

	for (double i = start + step; i < end; i += step)
	{
		double y = func(i);

		ret += step * (y + cache) / 2;

		cache = y;
	}

	return ret * retMultiplier;
}

//finds the greatest common divisor of two integers x and y
unsigned int _fastcall gcd(unsigned int x, unsigned int y) 
{
	if (y > x)
	{
		unsigned int tempX = x;
		x = y;
		y = tempX;
	}

	if (!(x % y)) 
	{
		return y;
	}
	return gcd(y, x % y);
}

//finds greatest common divisor of two ints
unsigned int dijkstraGcd(unsigned int x, unsigned int y)
{
	if (x == y) return x;

	return (x > y) ? dijkstraGcd(x - y, y) : dijkstraGcd(x, y - x);
}

template<class T>
//get a pair in the form {slope, intercept} of the regression line of a group of points
//no idea how this works
std::pair<double, double> linearRegression(std::pair<T, T> * x, unsigned int size) 
{
	if (!size) return { 0, 0 };

	double xSum = 0.0;
	double ySum = 0.0;

	for (unsigned int i = 0; i < size; i++) 
	{
		xSum += x[i].first;
		ySum += x[i].second;
	}

	double avgX = xSum / size;
	double avgY = ySum / size;

	double numerator = 0.0;
	double denominator = 0.0;

	for (unsigned int i = 0; i < size; i++)
	{
		numerator += (x[i].first - avgX) * (x[i].second - avgY);
		denominator += (x[i].first - avgX) * (x[i].first - avgX);
	}

	double slope = numerator / denominator;

	double intercept = (ySum - slope * xSum) / size;

	return std::make_pair(slope, intercept);
}

unsigned int factorial(unsigned int i)
{
	if (!i || i == 1) return 1;

	unsigned int retVal = i--;
	while (i) 
	{
		retVal *= i--;
	}

	return retVal;
}

template<class T>
//get the squared error of a dataset (vertical error, not perpendicular)
double squaredError(std::pair<T, T> * x, std::pair<double, double> eq, unsigned int size) 
{
	auto func = [eq](T x) {return eq.first * x + eq.second;};

	double ret = 0.0;

	for (unsigned int i = 0; i < size; i++) 
	{
		double temp = func(x[i].first);
		ret += (x[i].second - temp)*(x[i].second - temp);
	}

	return ret;
}

template<class T>
//get the squared error of a dataset (vertical error, not perpendicular)
double squaredError(std::pair<T, T> * x, unsigned int size)
{
	std::pair<double, double> eq = linearRegression(x, size);

	auto func = [eq](T x) {return eq.first * x + eq.second;};

	double ret = 0.0;

	for (unsigned int i = 0; i < size; i++)
	{
		double temp = func(x[i].first);
		ret += (x[i].second - temp)*(x[i].second - temp);
	}

	return ret;
}

//returns the ith prime
unsigned int getPrimes(unsigned int i)
{
	if (!i) return 2;
	if (i == 1) return 3;
	if (i == 2) return 5;

	std::vector<unsigned int> primes(2);
	primes.reserve(i + 1);

	primes[0] = 3;
	primes[1] = 5;

	for (unsigned int j = 7; true; j += 2) 
	{
		for (unsigned int k = 0; k < primes.size(); k++) 
		{
			if (!(j % primes[k])) 
			{
				goto notPrime;
			}
		}

		if (primes.size() + 1 == i) 
		{
			return j;
		}

		primes.push_back(j);

	notPrime:;
	}
}

//reverses bits in a char
inline unsigned char reverse(unsigned char c)
{
	c = (c & 0xF0) >> 4 | (c & 0x0F) << 4;
	c = (c & 0xCC) >> 2 | (c & 0x33) << 2;
	c = (c & 0xAA) >> 1 | (c & 0x55) << 1;

	return c;
}

#pragma warning (disable : 4146)

static inline uint32_t rotl(uint32_t n, unsigned int c)
{
	return (n << c) | (n >> (32 - c));
}

static inline uint32_t rotr(uint32_t n, unsigned int c)
{
	return n >> c | n << (32 - c);
}

#pragma warning (default : 4146)