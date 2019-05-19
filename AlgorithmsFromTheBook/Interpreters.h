#pragma once

#include <vector>

#define ADD -1//- is an operator, so these values are unused
#define SUBTRACT -2
#define MULTIPLY -3
#define DIVIDE -4

//turns the operators into specially constructed token values, based on the maximum integer values(look at defines above)
int tokenize(char c)
{
	switch (c)
	{
	case '+':
		return ADD;
	case '-':
		return SUBTRACT;
	case '*':
		return MULTIPLY;
	default:
		return DIVIDE;
	}
}

//takes in a astring representing an unparenthesized arithmetic expression using only +-* or / and returns the value of this expression
int arithmeticExpressionCalculator(char * s) {

	//tokenizer
	std::vector<int> tokens;

	int temp = 0;

	
	do {
		//allow whitespace
		if (*s == ' ')
		{
			continue;
		}
		//if *s is numeric, then add it to the end of temp
		else if (*s - '0' < 10 && *s - '0' >= 0)
		{
			temp *= 10;
			temp += *s - '0';
		}
		//else, it must be an operator, push temp onto vec and then push the operator
		else
		{
			tokens.push_back(temp);
			tokens.push_back(tokenize(*s));

			temp = 0;
		}
	} while (*++s);

	if (!tokens.size())
		return temp;

	tokens.push_back(temp);

	//parser
	int retVal = tokens[0], i = 1;

	while (i < tokens.size())
	{
		//these are top of the pecking order, no lookahead needed
		if (tokens[i] == MULTIPLY)
		{
			retVal *= tokens[++i];
		}
		else if (tokens[i] == DIVIDE)
		{
			retVal /= tokens[++i];
		}
		//now we need to look ahead, is there a multiply / divde next?
		else if (tokens[i] == ADD || tokens[i] == SUBTRACT)
		{
			bool isAdd = tokens[i] == ADD ? true : false;

			int val = tokens[i + 1];

			//used if there are chains of multiplies that need to be dealt with
		checkForMulAgain:

			if (i + 2 < tokens.size() && tokens[i + 2] == MULTIPLY)
			{
				val *= tokens[i + 3];

				i += 2;

				goto checkForMulAgain;
			}
			else if (i + 2 < tokens.size() && tokens[i + 2] == DIVIDE)
			{
				val /= tokens[i + 3];

				i += 2;

				goto checkForMulAgain;
			}

			retVal += isAdd ? val : -val;
			i++;
		}

		i++;
	}

	return retVal;
}

#pragma deprecated tokenize