#pragma once
#include <algorithm>

#ifndef EPSILON
	#define EPSILON 
#endif
//Used to define maximum bounds of scene
//10,000 units in both directions on all axis
//Each axis is 20,000 units in length
#ifndef _INFINITY
	#define _INFINITY 10000.0f
#endif

class Math
{
public:

	//Modified quadratic equation to resolve floating point error
	//http://en.wikipedia.org/wiki/Loss_of_significance
	inline static bool solveQuadratic(float a, float b, float c, float& x0, float& x1)
	{
		/*
		float des = (b*b) - 4 * a*c;
		if(des < 0) return false;
		if(des == 0) x0 = x1 = 0.5f*-b / a;
		x0 = (-b + sqrtf(b*b - 4 * a*c)) / 2 * a;
		x1 = (-b - sqrtf(b*b - 4 * a*c)) / 2 * a;
		*/

		float des = (b*b) - 4 * a*c;
		if(des < 0) 
			return false; 

		//TODO use epsilon
		if(b == 0)
		{
			x0 = sqrtf(-4 * a*c) / (2 * a);
			if(des > 0)
				x1 = -sqrtf(-4 * a*c) / (2 * a);
			return true;
		}

		float val;
		if(b > 0)
			val = -b + sqrtf(des);
		else
			val = -b - sqrtf(des);

		x0 = 0.5f*val / a;
		if(des > 0)
			x1 = c / (a*x0);
		return true;
		
	}
};