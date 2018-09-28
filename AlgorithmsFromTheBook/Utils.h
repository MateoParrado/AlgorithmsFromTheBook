#pragma once

template<class T>
//returns the distance between two coord pair points
double dist(std::pair<T, T> * p1, std::pair<T, T> * p2) {
	return sqrt((p1->first - p2->first)*(p1->first - p2->first) + (p1->second - p2->second)*(p1->second - p2->second));
};

//recursive exponentiation
int exponent(int base, int exp) {
	if (exp == 1) return base;
	if (exp == 2) return base * base;
	return exponent(base, exp / 2) * exponent(base, exp - exp / 2);
}

//has to be double bc otherwise x + 0.001 == x
double derivative(double(*func)(double), double pos) {
	return (func(pos + 0.001) - func(pos - 0.001)) / 0.002;
}

//traps is the numeber of trapezoids it is broken into
double integral(double(*func)(double), double start, double end, unsigned int traps) {
	double step = (end - start) / traps;

	double cache = func(start);

	double ret = 0;

	for (double i = start + step; i < end; i += step) {
		double y = func(i);

		ret += step * (y + cache) / 2;

		cache = y;
	}

	return ret;
}

template<class T>
//get a pair in the form {slope, intercept} of the regression line of a group of points
//no idea how this works
std::pair<double, double> linearRegression(std::pair<T, T> * x, unsigned int size) {
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

template<class T>
//get the squared error of a dataset (vertical error, not perpendicular)
double squaredError(std::pair<T, T> * x, std::pair<double, double> eq, unsigned int size) {
	auto func = [eq](T x) {return eq.first * x + eq.second;};

	double ret = 0.0;

	for (unsigned int i = 0; i < size; i++) {
		double temp = func(x[i].first);
		ret += (x[i].second - temp)*(x[i].second - temp);
	}

	return ret;
}

template<class T>
//get the squared error of a dataset (vertical error, not perpendicular)
double squaredError(std::pair<T, T> * x, unsigned int size) {
	std::pair<double, double> eq = linearRegression(x, size);

	auto func = [eq](T x) {return eq.first * x + eq.second;};

	double ret = 0.0;

	for (unsigned int i = 0; i < size; i++) {
		double temp = func(x[i].first);
		ret += (x[i].second - temp)*(x[i].second - temp);
	}

	return ret;
}

template<class T>
//linear regression for when multiple lines would fit the data better than one single one
//cost is the added cost of making a new line, so that you dont end up with just lines between every pair of points
std::vector<std::pair<unsigned int, unsigned int>> * segmentedLinearRegression(std::pair<T, T> * x, const unsigned int size, unsigned int cost) {
	//the minimum possible cost at every point
	std::vector<double> opt(size);

	//the error[i*size + j] is the minimum error of a line between the two points
	std::vector<double> error(size*size);

	//initialize every error
	for (unsigned int i = 0; i < size; i++) {
		error[size*i + i] = 0;

		if (!i) continue;

		error[size*i + i - 1] = 0;
		error[size*(i - 1) + i] = 0;

		for (unsigned int j = 0; j < i - 1; j++) {
			double temp = squaredError(&x[j], i - j + 1);

			//dont think I need this line but leaving it just in case
			//error[i*size + j] = temp;

			error[j*size + i] = temp;
		}
	}
	
	//fill the opt array
	for (unsigned int i = 1; i < size; i++) {
		double minVal = 1000000000;
		for (unsigned int j = 1; j < i; j++) {
			double temp = error[i*size + j] + cost + opt[j-1];
			if (temp < minVal) minVal = temp;
		}

		opt[i] = minVal;
	}

	std::vector<std::pair<unsigned int, unsigned int>> * retVec = new std::vector<std::pair<unsigned int, unsigned int>>;

	//num we are currently finding the minimum for
	unsigned int checkNum = size - 1;

	while(checkNum) {
		unsigned int minI = 0;
		double minIVal = error[checkNum] + cost;

		//find which point to draw a line to to minimize squared error
		for (unsigned int i = 1; i < checkNum; i++) {
			double temp = error[i*size + checkNum] + cost + opt[i - 1];
			if (temp < minIVal) {
				minIVal = temp;
				minI = i;
			}
		}

		retVec->push_back(std::make_pair(minI, checkNum));

		//find the minimum from the point we drew to
		checkNum = minI;
	}

	return retVec;
}