#pragma once
#ifndef BLOCK_H
#define BLOCK_H
#include "Vec2.h"
#include "Rect.h"

struct Block{
	Vec2 mCenterPoint;
	char mColor;
	bool mNumCollision;
	Rect mRect;
	bool mCountCollision;
};


#endif//BLOCK_H