#pragma once

#include <vector>
#include <algorithm>
#include <string>

//returns the vector containing the tasks that should be executed st the largest number of tasks can be executed
//input in the form (startTime, finishTime)
std::vector<std::pair<int, int>> intervalScheduler(std::vector<std::pair<int, int>> tasks) {
	std::sort(tasks.begin(), tasks.end(), [](auto &left, auto &right) {
		return left.second < right.second;
	}); // sort by the second, aka finish time

	std::vector<std::pair<int, int>> ret;

	while (!tasks.empty()) {
		ret.push_back(tasks[0]);

		while (!tasks.empty()) {//to avoid vector out of bounds error
			if (tasks[0].first < ret[ret.size() - 1].second) tasks.erase(tasks.begin());
			else break;
		}
	}

	return ret;
}

//returns a vector of vectors of tasks that should be executed to execute every task while using the fewest "resources"
//same as trying to minimize number of threads needed to run some number of tasks
//input in the form (startTime, finishTime)
std::vector < std::vector<std::pair<int, int>>> parallelScheduler(std::vector<std::pair<int, int>> tasks){
	std::sort(tasks.begin(), tasks.end(), [](auto &left, auto &right) {
		return left.second < right.second;
	}); // sort by the second, aka finish time
	
	std::vector<int> nums;//storing the number of the vector to be returned in
	nums.reserve(tasks.size());

	unsigned int minCheck = 0; //to avoid rechecking tasks that cant overlap

	unsigned int numVecs = 0;//number of vectors to return

	std::vector<int> numInVec(5);//number of times a task is pushed to a vector

	for (unsigned int i = 0; i < tasks.size(); i++) {
		std::vector<int> opts;

		for (unsigned int j = minCheck; j < i; j++) {//finding which vectors it cant be returned in
			if (tasks[j].second <= tasks[i].first) minCheck++;
			else {
				opts.push_back(nums[j]);
			}
		}

		for (unsigned int j = 0; j > -1; j++) {//infinite loop
			if (std::find(opts.begin(), opts.end(), j) == opts.end()) {
				nums.push_back(j);

				if (j > numVecs) { 
					numVecs = j; 
					if (numInVec.size() < j) numInVec.push_back(0);
				}

				numInVec[j]++;

				break;
			}
		}
	}
	
	std::vector<std::vector<std::pair<int, int>>> ret;
	ret.reserve(numVecs);

	for (unsigned int i = 0; i < numVecs + 1; i++) {
		std::vector<std::pair<int, int>> x;
		ret.push_back(x);
	}

	for (unsigned int i = 0; i < numVecs + 1; i++) {
		ret[i].reserve(numInVec[i]);
	}

	for (unsigned int i = 0; i < tasks.size(); i++) {
		ret[nums[i]].push_back(tasks[i]);
	}

	return ret;
}

//schedules tasks in the form (timeToExecute, deadline) in a way to minimize their deadlines
std::vector<std::pair<int, int>> minimalLatenessScheduler(std::vector<std::pair<int, int>> tasks) {
	std::sort(tasks.begin(), tasks.end(), [](auto &left, auto &right) {
		return left.second < right.second;
	});//sort by deadline

	return tasks;
}

struct weightedTask {
	unsigned int start, end, weight;
	weightedTask(unsigned int _start, unsigned int _end, unsigned int _weight) : start(_start), end(_end), weight(_weight) {  }
};

//get the number of tasks that are before j and dont intersect with it
unsigned int getP(std::vector<weightedTask> * x, unsigned int j) {
	if (!j) return 0;

	unsigned int retVal = j - 1;
	for (;;) {
		if (((*x)[retVal].end <= (*x)[j].start)) return retVal + 1;
		if (!retVal) return 0;
		retVal--;
	}
}

//returns the max value of work able to be done from a given set of weighted tasks
//must be sorted by end time
unsigned int getMaxPossibleWeight(std::vector<weightedTask> * x, unsigned int size = -1, unsigned int * ptr = nullptr) {
	bool delAtEnd = false;

	if (!ptr) {
		ptr = new unsigned int[x->size()];
		size = x->size();

		for (unsigned int i = 0; i < x->size(); i++) {
			ptr[i] = 0;
		}
		delAtEnd = true;
	}

	if (!size) return 0;
	if (ptr[size - 1]) {
		return ptr[size - 1];
	}

	unsigned int temp = std::max((*x)[size - 1].weight + getMaxPossibleWeight(x, getP(x, size - 1), ptr), getMaxPossibleWeight(x, size - 1, ptr));
	ptr[size - 1] = temp;

	if (delAtEnd) delete[] ptr;

	return temp;
}

//returns a ptr to an array of the values of the intervals given by each thing used below, hard to explain
//must be sorted by end time
unsigned int * getWeightsArray(std::vector<weightedTask> * x, unsigned int size = -1, unsigned int * ptr = nullptr) {

	if (!ptr) {
		ptr = new unsigned int[x->size()];
		size = x->size();

		for (unsigned int i = 0; i < x->size(); i++) {
			ptr[i] = 0;
		}
	}

	unsigned int temp = std::max((*x)[size - 1].weight + getMaxPossibleWeight(x, getP(x, size - 1), ptr), getMaxPossibleWeight(x, size - 1, ptr));
	ptr[size - 1] = temp;

	return ptr;
}

//same as above but better memory wise
unsigned int * iterativeGetWeightsArray(std::vector<weightedTask> * x) {
	unsigned int * ptr = new unsigned int[x->size()];
	ptr[0] = (*x)[0].weight;

	for (unsigned int i = 1; i < x->size(); i++) {
		unsigned int addOn = getP(x, i);
		if (addOn) addOn = ptr[addOn];

		ptr[i] = std::max((*x)[i].weight + addOn, ptr[i - 1]);
	}

	return ptr;
}

//returns the schedules that create the highest possible weight value
//must be sorted by end time
std::string weightedIntervalScheduler(std::vector<weightedTask> * x, unsigned int j = -1, unsigned int * ptr = nullptr) {
	if (!ptr) {
		ptr = iterativeGetWeightsArray(x);
		j = x->size() - 1;
	}

	unsigned int p = getP(x, j);

	if (!p) {
		if (!j) return std::to_string(j);

		if ((*x)[j].weight > ptr[j - 1]) {
			return std::to_string(j);
		}
	}
	else if ((*x)[j].weight + ptr[p - 1] > ptr[j - 1]) {
		return std::to_string(j) + weightedIntervalScheduler(x, p - 1, ptr);
	}
	return weightedIntervalScheduler(x, j - 1, ptr);
}

#pragma deprecated (getP, getMaxPossibleWeight, getWeightsArray, getIterativeWeightsArray)