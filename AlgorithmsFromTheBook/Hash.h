#pragma once

#include "BitPacking.h"
#include "Utils.h"

uint32_t intify(char a, char b, char c, char d) {
	return ((uint32_t)a << 24) + ((uint32_t)b << 16) + ((uint32_t)c << 8) + ((uint32_t)d);
}

uint32_t* SHA256(char * dat, uint32_t len) {

	//initialized hash values
	uint32_t h0 = 0x6a09e667;
	uint32_t h1 = 0xbb67ae85;
	uint32_t h2 = 0x3c6ef372;
	uint32_t h3 = 0xa54ff53a;
	uint32_t h4 = 0x510e527f;
	uint32_t h5 = 0x9b05688c;
	uint32_t h6 = 0x1f83d9ab;
	uint32_t h7 = 0x5be0cd19;

	static const uint32_t k[] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

	BitPacking::BitArray data(len + (512 - (len % 512)));

	//split the int into chars
	for (unsigned int i = 0; i < len; i++) {
		data.assignByte(i, dat[i]);
	}

	//put a single 1 bit
	data.assignByte(len * 4, 0b10000000);

	//put 8 0 bits in every subsequent spot until we have a multiple of 512 (64 * 8) bits
	for (unsigned int i = len * 4 + 2; i % 64 != 60; i++) {
		data.assignByte(i, 0);
	}

	data.assignByte(data.size - 4, (len >> 24) & 0xFF);
	data.assignByte(data.size - 3, (len >> 16) & 0xFF);
	data.assignByte(data.size - 2, (len >> 8) & 0xFF);
	data.assignByte(data.size - 1, (len) & 0xFF);

	//split message into 512 bit chunks
	for (int chunk = 0; chunk < data.size; chunk += 512) {
		uint32_t mess[64]{ 0 };

		//put the chunk into the first 16 ints of the message array
		for (int i = 0; i < 16; i++) {
			mess[i] = intify(data.getByte(i * 4), data.getByte(i * 4 + 1), data.getByte(i * 4 + 2), data.getByte(i * 4 + 3));
		}

		//extend into whole array
		for (int i = chunk + 16; i < chunk + 64; i++) {
			uint32_t shift = rotr(mess[i - 15], 7) ^ rotr(mess[i - 15], 18) ^ (mess[i - 15] >> 3);
			uint32_t shift1 = rotr(mess[i - 2], 17) ^ rotr(mess[i - 2], 19) ^ (mess[i - 2] >> 10);

			mess[i] = mess[i - 16] + shift + mess[i - 7] + shift1;
		}

		//intitialize working variables
		uint32_t a = h0;
		uint32_t b = h1;
		uint32_t c = h2;
		uint32_t d = h3;
		uint32_t e = h4;
		uint32_t f = h5;
		uint32_t g = h6;
		uint32_t h = h7;

		for (int i = 0; i < 64; i++) {
			uint32_t shift = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
			uint32_t ch = (e & f) ^ ((~e) & g);

			uint32_t temp = h + shift + ch + k[i] + mess[i];

			uint32_t shift1 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
			uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
			uint32_t temp1 = maj + shift1;

			h = g;
			g = f;
			f = e;
			e = d + temp;
			d = c;
			c = b;
			b = a;
			a = temp + temp1;
		}

		//add chunk to hash
		h0 = h0 + a;
		h1 = h1 + b;
		h2 = h2 + c;
		h3 = h3 + d;
		h4 = h4 + e;
		h5 = h5 + f;
		h6 = h6 + g;
		h7 = h7 + h;
	}

	return new uint32_t[8] {h0, h1, h2, h3, h4, h5, h6, h7};
}

#pragma deprecated(intify)