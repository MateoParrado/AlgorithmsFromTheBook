#pragma once

struct FlightStruct 
{
	unsigned char startCity, endCity;
	float startTime;

	FlightStruct(unsigned char startCity, unsigned char endCity, float startTime) : startCity(startCity), endCity(endCity), startTime(startTime)
	{

	}
};