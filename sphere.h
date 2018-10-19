#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "quadtile.h"
#include "pool.h"

class spheroid
{
	
public:
	int a, b;
	quadtile tiles;
	threadpool pool;

	spheroid(double _a, double _b);
};

#endif//__SPHERE_H__