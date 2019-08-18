#pragma once

#include <iostream>

#include "CharLiteral.h"
#include "Utils.h"

namespace BitPacking
{
	struct BitArray
	{
	public:
		unsigned char * ptr;
		int size;

	public:
		BitArray(int size)
		{
			if (!(size % 8))
			{
				ptr = new unsigned char[size / 8]{ 0 };
				this->size = size / 8;
			}
			else
			{
				ptr = new unsigned char[size / 8 + 1]{ 0 };
				this->size = size / 8 + 1;
			}
		}

		/*GETTERS AND SETTERS*/

		void assignByte(unsigned int pos, char val) {
			ptr[pos] = val;
		}

		char getByte(unsigned int pos) {
			return ptr[pos];
		}

		void setBit(int x) 
		{
			ptr[x / 8] |= (128 >> x % 8);
		}

		void setRange(int start, int end)
		{
			for (int i = start; i < end; i++)
			{
				ptr[i / 8] |= (128 >> i % 8);
			}
		}

		void flipBit(int x)
		{
			ptr[x / 8] ^= (128 >> x % 8);
		}

		void flipRange(int start, int end)
		{
			for (int i = start; i < end; i++)
			{
				if(!(i % 8) && (i + 8 < end))
				{
					ptr[i / 8] = ~ptr[i / 8];

					i += 7;
				}
				else
				{
					ptr[i / 8] ^= (128 >> i % 8);
				}
			}
		}

		void unsetBit(int x)
		{
			ptr[x / 8] &= (255 ^ (128 >> x % 8));
		}

		void unsetRange(int start, int end)
		{
			for (int i = start; i < end; i++)
			{
				ptr[i / 8] &= (255 ^ (128 >> i % 8));
			}
		}

		bool getBit(int x) 
		{
			return ptr[x / 8] & (128 >> x % 8);
		}

		BitArray getRange(int start, int end)
		{
			BitArray ret(end - start);

			int pos = 0;

			for (int i = start; i < end; i++)
			{
				if (getBit(i))
				{
					ret.setBit(pos);
				}

				pos++;
			}

			return ret;
		}

		/*OPERATORS*/

		bool operator==(BitArray& other)
		{
			if (other.size != size)
			{
				return false;
			}

			for (int i = 0; i < size; i++)
			{
				if (ptr[i] != other.ptr[i])
				{
					return false;
				}
			}

			return true;
		}

		bool operator!=(BitArray& other)
		{
			return !operator==(other);
		}

		BitArray operator>> (char shift) 
		{
			BitArray ret(*this);

			//if shift is more than a byte, do it recursively
			while (shift > 8)
			{
				ret >>= 8;
				shift -= 8;
			}

			unsigned char carry = 0;

			for (int i = 0; i < size; i++)
			{
				unsigned char oldCarry = carry;

				carry = ret.ptr[i] & (0xFF_uc >> (8 - shift));

				carry <<= 8 - shift;

				ret.ptr[i] >>= shift;

				ret.ptr[i] |= oldCarry;
			}

			return ret;
		}

		void operator>>=(char shift) 
		{
			//if shift is more than a byte, do it recursively
			while (shift > 8) 
			{
				this->operator>>=(8);
				shift -= 8;
			}

			unsigned char carry = 0;

			for (int i = 0; i < size; i++)
			{
				unsigned char oldCarry = carry;

				carry = ptr[i] & (0xFF_uc >> (8 - shift));

				carry <<= 8 - shift;

				ptr[i] >>= shift;

				ptr[i] |= oldCarry;
			}
		}

		void ror(char shift)
		{
			//if shift is more than a byte, do it recursively
			while (shift > 8)
			{
				ror(8);
				shift -= 8;
			}

			unsigned char carry = 0;

			for (int i = 0; i < size; i++)
			{
				unsigned char oldCarry = carry;

				carry = ptr[i] & (0xFF_uc >> (8 - shift));

				carry <<= 8 - shift;

				ptr[i] >>= shift;

				ptr[i] |= oldCarry;
			}

			ptr[0] |= carry;
		}

		BitArray operator<<(char shift)
		{
			BitArray ret(*this);

			//if shift is more than a byte, do it recursively
			while (shift > 8)
			{
				ret <<= 8;
				shift -= 8;
			}

			unsigned char carry = 0;

			for (int i = size - 1; i >= 0; i--)
			{
				unsigned char oldCarry = carry;

				carry = ret.ptr[i] & (0xFF_uc << (8 - shift));

				carry >>= 8 - shift;

				ret.ptr[i] <<= shift;

				ret.ptr[i] |= oldCarry;
			}

			return ret;
		}

		void operator<<=(char shift)
		{
			//if shift is more than a byte, do it recursively
			while (shift > 8)
			{
				this->operator<<=(8);
				shift -= 8;
			}

			unsigned char carry = 0;

			for (int i = size - 1; i >= 0; i--)
			{
				unsigned char oldCarry = carry;

				carry = ptr[i] & (0xFF_uc << (8 - shift));

				carry >>= 8 - shift;

				ptr[i] <<= shift;

				ptr[i] |= oldCarry;
			}
		}

		void rol(char shift)
		{
			//if shift is more than a byte, do it recursively
			while (shift > 8)
			{
				rol(8);
				shift -= 8;
			}

			unsigned char carry = 0;

			for (int i = size - 1; i >= 0; i--)
			{
				unsigned char oldCarry = carry;

				carry = ptr[i] & (0xFF_uc << (8 - shift));

				carry >>= 8 - shift;

				ptr[i] <<= shift;

				ptr[i] |= oldCarry;
			}

			ptr[size - 1] |= carry;
		}

		BitArray operator|(BitArray& other)
		{
			if (other.size != size)
			{
				throw "Comparing bit arrays of different sizes";
			}

			BitArray ret(size * 8);

			for (int i = 0; i < size; i++)
			{
				ret.ptr[i] = ptr[i] | other.ptr[i];
			}

			return ret;
		}

		BitArray operator|(char c)
		{
			BitArray ret(*this);

			for (int i = 0; i < size; i++)
			{
				ret.ptr[i] |= c;
			}

			return ret;
		}

		void operator|=(BitArray& other)
		{
			if (other.size != size)
			{
				throw "Comparing bit arrays of different sizes";
			}

			for (int i = 0; i < size; i++)
			{
				ptr[i] |= other.ptr[i];
			}
		}

		void operator|=(char c)
		{
			for (int i = 0; i < size; i++)
			{
				ptr[i] |= c;
			}
		}

		void orRange(BitArray& other, int start)
		{
			int counter = start;

			for (int i = 0; i < other.size * 8; i++)
			{
				if (other.getBit(i))
				{
					setBit(counter);
				}
				counter++;
			}
		}

		//the char to start on and end on, not the bit
		void orRange(int start, int end, char c)
		{
			for (int i = start; i < end; i++)
			{
				ptr[i] ^= c;
			}
		}

		BitArray operator&(const BitArray& other)
		{
			if (other.size != size)
			{
				throw "Comparing bit arrays of different sizes";
			}

			BitArray ret(size * 8);

			for (int i = 0; i < size; i++)
			{
				ret.ptr[i] = ptr[i] & other.ptr[i];
			}

			return ret;
		}

		BitArray operator&(char c)
		{
			BitArray ret(*this);

			for (int i = 0; i < size; i++)
			{
				ret.ptr[i] &= c;
			}

			return ret;
		}

		void operator&=(BitArray& other)
		{
			if (other.size != size)
			{
				throw "Comparing bit arrays of different sizes";
			}

			for (int i = 0; i < size; i++)
			{
				ptr[i] &= other.ptr[i];
			}
		}

		void operator&=(char c)
		{
			for (int i = 0; i < size; i++)
			{
				ptr[i] &= c;
			}
		}

		void andRange(BitArray& other, int start)
		{
			int counter = start;

			for (int i = 0; i < other.size * 8; i++)
			{
				if (other.getBit(i) && this->getBit(counter))
				{
					setBit(counter);
				}
				else
				{
					unsetBit(counter);
				}
				counter++;
			}
		}

		//the char to start on and end on, not the bit
		void andRange(int start, int end, char c)
		{
			for (int i = start; i < end; i++)
			{
				ptr[i] &= c;
			}
		}

		BitArray operator^(BitArray& other)
		{
			BitArray ret(*this);

			if (other.size != size)
			{
				throw "Comparing bit arrays of different sizes";
			}

			for (int i = 0; i < size; i++)
			{
				ret.ptr[i] ^= other.ptr[i];
			}

			return ret;
		}

		BitArray operator^(char c)
		{
			BitArray ret(*this);

			for (int i = 0; i < size; i++)
			{
				ret.ptr[i] ^= c;
			}

			return ret;
		}

		void operator^=(char c)
		{
			for (int i = 0; i < size; i++)
			{
				ptr[i] ^= c;
			}
		}

		void operator^=(BitArray& other)
		{
			if (other.size != size)
			{
				throw "Comparing bit arrays of different sizes";
			}

			for (int i = 0; i < size; i++)
			{
				ptr[i] ^= other.ptr[i];
			}
		}

		void xorRange(BitArray& other, int start)
		{
			int counter = start;

			for (int i = 0; i < other.size * 8; i++)
			{
				if ((other.getBit(i) && !this->getBit(counter)) || (!other.getBit(i) && this->getBit(counter)))
				{
					setBit(counter);
				}
				else
				{
					unsetBit(counter);
				}
				counter++;
			}
		}

		//the char to start on and end on, not the bit
		void xorRange(int start, int end, char c)
		{
			for (int i = start; i < end; i++)
			{
				ptr[i] |= c;
			}
		}

		BitArray operator~()
		{
			BitArray ret(size * 8);

			for (int i = 0; i < size; i++)
			{
				ret.ptr[i] = ~ptr[i];
			}

			return ret;
		}

		void negate()
		{
			for (int i = 0; i < size; i++)
			{
				this->ptr[i] = ~ptr[i];
			}
		}

		void assign(BitArray& other, int start)
		{
			int counter = start;

			for (int i = 0; i < other.size * 8; i++)
			{
				if (other.getBit(i))
				{
					this->setBit(counter);
				}
				else
				{
					this->unsetBit(counter);
				}

				counter++;
			}
		}

		/*UTILITY*/
		void print() 
		{
			for (int i = 0; i < size; i++) 
			{
				char temp = ptr[i];

				for (char j = 0; j < 8; j++) 
				{
					if (temp < 0) 
					{
						std::cout << 1;
					}
					else
					{
						std::cout << 0;
					}

					//to see some weird compiler bugs, delete the _c, and then the shift is applied not only to temp but also to size
					temp <<= 1_c;
				}
				std::cout << ' ';
			}
		}

		/*RULE OF FIVE*/
		BitArray(const BitArray& old) 
		{
			this->size = old.size;
			this->ptr = new unsigned char[size];

			for (int i = 0; i < size; i++)
			{
				ptr[i] = old.ptr[i];
			}

		}

		BitArray(BitArray&& old) noexcept
		{
			this->size = old.size;
			this->ptr = old.ptr;

			old.size = 0;
			old.ptr = nullptr;
		}

		BitArray& operator=(const BitArray& old)
		{
			BitArray ret(old);

			return ret;
		}

		BitArray& operator=(BitArray&& old) noexcept
		{
			if (this != &old)
			{
				delete[] ptr;

				ptr = old.ptr;
				size = old.size;

				old.ptr = nullptr;
				old.size = 0;
			}
		}

		~BitArray()
		{
			if (this->ptr)
			{
				delete[] this->ptr;
			}
		}
	};
}