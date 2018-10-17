#pragma once
#include <utility>

template<class T>
//returns the distance between two coord pair points
double dist(std::pair<T, T> * p1, std::pair<T, T> * p2) {
	return sqrt((p1->first - p2->first)*(p1->first - p2->first) + (p1->second - p2->second)*(p1->second - p2->second));
};

//recursive exponentiation
int exponent(int base, int exp) {
	if (!exp) return 1;
	if (exp == 1) return base;
	if (exp == 2) return base * base;
	return exponent(base, exp / 2) * exponent(base, exp - exp / 2);
}

//has to be double bc otherwise x + 0.001 == x
double derivative(double(*func)(double), double pos) {
	return (func(pos + 0.0000001) - func(pos - 0.0000001)) / 0.0000002;
}

//traps is the numeber of trapezoids it is broken into
double integral(double(*func)(double), double start, double end, unsigned int traps) {
	//should we switch the vakue of the integral?
	int retMultiplier = 1;
	
	if (end < start) {
		retMultiplier = -1;

		double tempStart = start;
		start = end;
		end = tempStart;
	}


	double step = (end - start) / traps;

	double cache = func(start);

	double ret = 0;

	for (double i = start + step; i < end; i += step) {
		double y = func(i);

		ret += step * (y + cache) / 2;

		cache = y;
	}

	return ret * retMultiplier;
}

//finds the greatest common divisor of two integers x and y
unsigned int gcd(unsigned int x, unsigned int y) {
	if (y > x) {
		unsigned int tempX = x;
		x = y;
		y = tempX;
	}

	if (!(x % y)) {
		return y;
	}
	return gcd(y, x % y);
}

//finds greatest common divisor of two ints
unsigned int dijkstraGcd(unsigned int x, unsigned int y) {
	if (x == y) return x;

	return (x > y) ? dijkstraGcd(x - y, y) : dijkstraGcd(x, y - x);
}
template<class T>
//get a pair in the form {slope, intercept} of the regression line of a group of points
//no idea how this works
std::pair<double, double> linearRegression(std::pair<T, T> * x, unsigned int size) {
	if (!size) return { 0, 0 };

	double xSum = 0.0;
	double ySum = 0.0;

	for (unsigned int i = 0; i < size; i++) {
		xSum += x[i].first;
		ySum += x[i].second;
	}

	double avgX = xSum / size;
	double avgY = ySum / size;

	double numerator = 0.0;
	double denominator = 0.0;

	for (unsigned int i = 0; i < size; i++) {
		numerator += (x[i].first - avgX) * (x[i].second - avgY);
		denominator += (x[i].first - avgX) * (x[i].first - avgX);
	}

	double slope = numerator / denominator;

	double intercept = (ySum - slope * xSum) / size;

	return std::make_pair(slope, intercept);
}

unsigned int factorial(unsigned int i) {
	if (!i || i == 1) return 1;

	unsigned int retVal = i--;
	while (i) {
		retVal *= i--;
	}

	return retVal;
}