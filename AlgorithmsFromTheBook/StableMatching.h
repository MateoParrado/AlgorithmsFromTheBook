#pragma once
#include <vector>
#include "LinkedList.h"

//finds a stable matching between two sets of "men" and "women"
//the second two parameters should be pointers to the 0th index of two dimensional arrays
std::vector<std::pair<unsigned int, unsigned int>> stableMatch(SinglyLinkedList::LinkedList<unsigned int> men, const unsigned int * firstManFirstPick, const unsigned int * firstWomanFirstPick) {
	const unsigned int originalSize = men.size;

	std::vector<std::pair<unsigned int, unsigned int>> ret;
	ret.reserve(originalSize);

	std::vector<int> womensRankings(originalSize * originalSize); //preprocessing womens choicews for easy access later
	for (unsigned int i = 0; i < originalSize; i++) {
		for (unsigned int j = 0; j < originalSize; j++) {
			womensRankings[i*originalSize + *(firstWomanFirstPick + originalSize*i + j)] = j;
		}
	}
	
	std::vector<int> menNextProp(originalSize);//array of who the guy will propose to next

	while (men.size) {
		int man = men.head->obj;
		unsigned int nextProp = *(firstManFirstPick + originalSize * man + menNextProp[man]);

		for (unsigned int i = 0; i < ret.size(); i++) {//looks for man's choice in the rankings

			if (ret[i].second == nextProp) {

				int currentMan = ret[i].first;//the womans current husband

				if (womensRankings[ret[i].second * originalSize + currentMan] < womensRankings[ret[i].second * originalSize + man]) {//does she want to stay?
					menNextProp[man]++;

					goto loopOver;
				}
				else {
					men.pushBackNode(currentMan);

					ret.erase(begin(ret) + i);

					std::pair<unsigned int, unsigned int> temp(man, nextProp);

					ret.push_back(temp);

					menNextProp[man]++;

					men.popFrontNode();

					goto loopOver;
				}
			}
		}
		{//if the woman isn't in ret yet
			std::pair<unsigned int, unsigned int> temp(man, nextProp);

			ret.push_back(temp);

			menNextProp[man]++;

			men.popFrontNode();
		}
	loopOver: ;
	}

	return ret;
}