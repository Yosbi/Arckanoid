// Vec2.cpp
// Version 1.1 by Yosbi Alves
// Yosbito@gmail.com

#include "Vec2.h"
#include <cmath>

Vec2::Vec2()
: x(0.0f), y(0.0f)
{
}

Vec2::Vec2(float s, float t)
: x(s), y(t)
{
}

Vec2::Vec2(float d[2])
: x(d[0]), y(d[1])
{
}

Vec2::Vec2(const POINT& p)
: x((float)p.x), y((float)p.y)
{
}

Vec2 Vec2::operator+(const Vec2& rhs)const
{
	Vec2 result;
	result.x = x + rhs.x;
	result.y = y + rhs.y;

	return result;
}

Vec2 Vec2::operator-(const Vec2& rhs)const
{
	Vec2 result;
	result.x = x - rhs.x;
	result.y = y - rhs.y;

	return result;
}

Vec2 Vec2::operator-()
{
	Vec2 result;
	result.x = -x;
	result.y = -y;

	return result;
}

Vec2::operator POINT()
{
	POINT p = {(int)x, (int)y};
	return p;
}

void Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator*=(float s)
{
	x *= s;
	y *= s;
}

void Vec2::operator/=(float s)
{
	// Assumes s != 0.
	s /= s;
	y /= s;
}

float Vec2::length()
{
	return sqrtf(x*x + y*y);
}

Vec2& Vec2::normalize()
{
	// Assumes len != 0.
	float len = length();

	x /= len;
	y /= len;

	return *this;
}

float Vec2::dot(const Vec2& rhs)
{
	return x*rhs.x + y*rhs.y;
}

Vec2& Vec2::reflect(const Vec2& normal)
{
	Vec2 I = *this;

	return *this = 2.0f * -I.dot(normal) * normal + I;
}

Vec2& Vec2::rotate(float t)
{
	// Assumes len != 0.
	// first y got the length of the old vector because rotate may 
	// causes the disminution of the length of the vector
	//(somtimes it does it)
	float len = length();
	// then y gt the new points at new angle
	x = x * cosf(t) - y * sinf(t);
	y = y * cosf(t) + x * sinf(t);
	// and verifi if the new length is correct, if its not
	// i normalize him and multiply by the old length
	float len2 = length();
	if(len2 < len){
		this->normalize() *= len;

	}

	return *this;
}

Vec2 operator*(const Vec2& v, float s)
{
	Vec2 result;

	result.x = v.x * s;
	result.y = v.y * s;

	return result;
}

Vec2 operator*(float s, const Vec2& v)
{
	Vec2 result;

	result.x = v.x * s;
	result.y = v.y * s;

	return result;
}

Vec2 operator/(const Vec2& v, float s)
{
	Vec2 result;

	// Assumes s != 0.
	result.x = v.x / s;
	result.y = v.y / s;

	return result;
}

Vec2 operator/(float s, const Vec2& v)
{
	Vec2 result;

	// Assumes s != 0.
	result.x = v.x / s;
	result.y = v.y / s;

	return result;
}