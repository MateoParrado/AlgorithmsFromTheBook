#pragma once
#include "boost\multi_array.hpp"

enum RnaBase {
	Guanine, Adenine, Cytosine, Uracil
};

struct RNA {
	RnaBase * bases = nullptr;
	unsigned int size;

	//construct from array
	RNA(RnaBase * b, unsigned int size) : size(size){
		bases = DBG_NEW RnaBase[size];
		for (unsigned int i = 0; i < size; i++) {  
			bases[i] = b[i];
		}
	}
	//construct from initializer list
	RNA(std::initializer_list<RnaBase> list) : size(list.size()){
		bases = DBG_NEW RnaBase[list.size()];
		for (unsigned int i = 0; i < list.size(); i++) {
			bases[i] = list.begin()[i];
		}
	}
	//construct from string of uppercase letters
	RNA(std::string str) : size(str.size()){
		bases = DBG_NEW RnaBase[size];

		for (unsigned int i = 0; i < size; i++) {
			RnaBase tempBase;
			switch (str[i]) {
			case 'A':
				tempBase = Adenine;
				break;
			case 'C':
				tempBase = Cytosine;
				break;
			case 'G':
				tempBase = Guanine;
				break;
			case 'U':
				tempBase = Uracil;
				break;
			default:
				throw 0;
			}

			bases[i] = tempBase;
		}
	}

	bool canBePaired(unsigned int i, unsigned int j) {
		switch (bases[i]) {
		case Adenine:
			if (bases[j] == Uracil) return true;
			break;
		case Guanine:
			if (bases[j] == Cytosine) return true;
			break;
		case Cytosine:
			if (bases[j] == Guanine) return true;
			break;
		case Uracil:
			if (bases[j] == Adenine) return true;
			break;
		}
		return false;
	}

	~RNA(){
		delete[] bases;
	}
};

void tracebackNussinov(unsigned int i, unsigned int j, boost::multi_array<unsigned int, 2> * opt, std::vector<std::pair<unsigned int, unsigned int>> * vec, RNA * rna) {
	//endcase
	if (j <= i) {
		return;
	}

	//if j is unpaired, no point in running the rest of the algorithm so suptract one until it hits either a paired value or i
	while ((*opt)[i][j] == (*opt)[i][j - 1]) {
		j--;
		if (j <= i) return;
	}

	//find maximum for every value between k and i
	for (unsigned int k = i; k < j - 4; k++) {
		//make sure only compatible bases are paired
		if (!rna->canBePaired(k, j)) continue;

		//if k is zero only consider the forwards pairing
		if (!k) {

			//if k and j are paired, pair them and find all pairings in between them
			if ((*opt)[i][j] == (*opt)[k + 1][j - 1] + 1) {
				vec->push_back(std::make_pair(k, j));
				tracebackNussinov(k + 1, j - 1, opt, vec, rna);
				return;
			}
		}
		//otherwise consider the forwards and backwards pairing
		else if ((*opt)[i][j] == (*opt)[i][k - 1] + (*opt)[k + 1][j - 1] + 1) {
			//if k and j are paired you now need to check not only between them, but also to the left of k

			vec->push_back(std::make_pair(k, j));

			tracebackNussinov(i, k - 1, opt, vec, rna);
			tracebackNussinov(k + 1, j - 1, opt, vec, rna);
			return;
		}
	}
}

#pragma warning (disable : 4018)

//find the optimal structure of an rna sequence
//this means finding the structure that pairs the most nucleotides where no two paired nucleotides are separated by less than 4 nucleotides
//finds the likely structure of an rna sequence absed off of its minimum energy shape
std::vector<std::pair<unsigned int, unsigned int>> * rnaStructure(RNA * rna) {
	//create the memoized array
	typedef boost::multi_array<unsigned int, 2>::index index;
	boost::multi_array<unsigned int, 2> opt(boost::extents[rna->size][rna->size]);

	//start checking at the 5th element down (because min loop size is four)
	for (unsigned int k = 5; k < rna->size; k++) {
		//go through all possible indices
		for (index i = 0; i < rna->size - k; i++) {
			//check if i and j should be matched(take the maximum of their unpaired and their paired values)
			index j = i + k;

			unsigned int unpairedVal = opt[i][j-1];

			unsigned int maxPairVal = 0;

			for (unsigned char t = i; t < j - 4; t++) {
				//find the maximum assuming that i, j is included
				if (rna->canBePaired(t, j)) {
					//the immediate if is to not calculate it if t is less tha zero
					unsigned int temp = 1 + (t ? opt[i][t - 1] : 0) + opt[t + 1][j - 1];
					if (temp > maxPairVal) maxPairVal = temp;
				}
			}

			opt[i][j] = std::max(unpairedVal, maxPairVal);
			opt[j][i] = opt[i][j];
		}
	}

	std::vector<std::pair<unsigned int, unsigned int>> * retVec = DBG_NEW std::vector<std::pair<unsigned int, unsigned int>>;

	//traceback through opt and find the maximum
	tracebackNussinov(0, rna->size - 1, &opt, retVec, rna);

	return retVec;
}

#pragma warning (default: 4018)

//make sure that tracebackNussinov gets called nowhere else
#pragma deprecated (tracebackNussinov)