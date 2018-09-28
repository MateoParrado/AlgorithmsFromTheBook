#pragma once
#include "boost\multi_array.hpp"

enum RnaBase {
	Guanine, Adenine, Cytosine, Urasil
};

struct RNA {
	RnaBase * bases = nullptr;
	unsigned int size;

	//construct from array
	RNA(RnaBase * b, unsigned int size) : size(size){
		bases = new RnaBase[size];
		for (unsigned int i = 0; i < size; i++) {  
			bases[i] = b[i];
		}
	}
	//construct from initializer list
	RNA(std::initializer_list<RnaBase> list) : size(list.size()){
		bases = new RnaBase[list.size()];
		for (unsigned int i = 0; i < list.size(); i++) {
			bases[i] = list.begin()[i];
		}
	}
	//construct from string of uppercase letters
	RNA(std::string str) : size(str.size()){
		bases = new RnaBase[size];

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
				tempBase = Urasil;
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
			if (bases[j] == Urasil) return true;
			break;
		case Guanine:
			if (bases[j] == Cytosine) return true;
			break;
		case Cytosine:
			if (bases[j] == Guanine) return true;
			break;
		case Urasil:
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

	//if j opt i, j is unpaired, recur until it is
	while ((*opt)[i][j] == (*opt)[i][j - 1]) {
		j--;
		if (j <= i) return;
	}

	//find maximum for every value between k and i
	for (unsigned int k = i; k < j - 4; k++) {
		if (!rna->canBePaired(k, j)) continue;

		//if k is zero only consider the forwards part
		if (!k) {
			if ((*opt)[i][j] == (*opt)[k + 1][j - 1] + 1) {
				vec->push_back(std::make_pair(k, j));
				tracebackNussinov(k + 1, j - 1, opt, vec, rna);
				return;
			}
		}
		//otherwise consider the forwards and backwards part
		else if ((*opt)[i][j] == (*opt)[i][k - 1] + (*opt)[k + 1][j - 1] + 1) {
			vec->push_back(std::make_pair(k, j));

			tracebackNussinov(i, k - 1, opt, vec, rna);
			tracebackNussinov(k + 1, j - 1, opt, vec, rna);
			return;
		}
	}
}

#pragma warning (disable : 4018)

//find the maximum pairing of a sequence of rna nucleotides where no two i and j are paired where i is less than or equal to four from j
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
					unsigned int temp = 1 + (t ? opt[i][t - 1] : 0) + opt[t + 1][j - 1];
					if (temp > maxPairVal) maxPairVal = temp;
				}
			}

			//i dont understand why i need both (i, j) and (j, i) but i do
			opt[i][j] = std::max(unpairedVal, maxPairVal);
			opt[j][i] = opt[i][j];
		}
	}

	std::vector<std::pair<unsigned int, unsigned int>> * retVec = new std::vector<std::pair<unsigned int, unsigned int>>;

	//traceback through opt and find the maximum
	tracebackNussinov(0, rna->size - 1, &opt, retVec, rna);

	return retVec;
}

#pragma warning (default: 4018)

#pragma deprecated (tracebackNussinov)