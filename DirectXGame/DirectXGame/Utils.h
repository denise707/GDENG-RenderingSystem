#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>

class Utils
{
public:
	float GetRandomFloat()
	{
		float a = 4.0;
		return (float(rand()) / float((RAND_MAX)) * a);
	}
};