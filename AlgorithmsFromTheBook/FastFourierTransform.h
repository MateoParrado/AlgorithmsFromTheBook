#pragma once
#include <complex>

#define PI 3.14159265358979323846

//divide array of complex nums into even and odd parts
void separate(std::complex<double> * x, unsigned int size) {
	std::complex<double> * temp = new std::complex<double>[size / 2];

	for (unsigned int i = 0; i < size/2; i++) {
		temp[i] = x[2 * i + 1];
	}
	for (unsigned int i = 0; i < size / 2; i++) {
		x[i] = x[2 * i];
	}
	for (unsigned int i = 0; i < size / 2; i++) {
		x[i + size / 2] = temp[i];
	}

	delete[] temp;
}

//computes the fft on an array pf complex numbers, stores the return in the array it is provided
//only use power of two sizes and only look at first half of data
void FFT(std::complex<double> * x, unsigned int size) {
	if (size >= 2) {
		//divide it into the odd and even coeficients
		separate(x, size);

		FFT(x, size / 2);
		FFT(x + size / 2, size - size / 2);

		//this is the calculation of the twiddle factor used to rejoin the arrays
		for (unsigned int i = 0; i < size / 2; i++) {
			std::complex<double> even = x[i];
			std::complex<double> odd = x[i + 1];

			std::complex<double> twiddle = exp(std::complex<double>(0, -2.0 * PI * i / size));

			x[i] = even + twiddle * odd;
			x[i + size / 2] = even - twiddle * odd;
		}
	}
}

#pragma deprecated (separate)