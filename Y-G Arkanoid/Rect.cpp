// Rect.cpp
// By Yosbi Alves
// Yosbito@gmail.com

#include "Rect.h"

Rect::Rect()
{
}

Rect::Rect(const Vec2& a, const Vec2& b)
: minPt(a), maxPt(b)
{
}

Rect::Rect(float x0, float y0, float x1, float y1)
: minPt(x0, y0), maxPt(x1, y1)
{
}


void Rect::forceInside(Circle& A)
{
	Vec2 p  = A.c;
	float r = A.r;

	// Modify coordinates to force inside.
	if(p.x - r < minPt.x)
		p.x = minPt.x + r;
	if(p.x + r > maxPt.x)
		p.x = maxPt.x - r;

	if(p.y - r < minPt.y)
		p.y = minPt.y + r;
	if(p.y + r > maxPt.y)
		p.y = maxPt.y - r;

	// Save forced position.
	A.c = p;
}

void Rect::forceInside(Rect& A)
{
	Vec2 maxp = A.maxPt;
	
	Vec2 minp = A.minPt;

	float help = 0.0f;
	// Modify coordinates to force inside.
	if(minp.x < minPt.x){
		help = minPt.x - minp.x;
		minp.x += help;
		maxp.x += help;
	}
	if(maxp.x > maxPt.x){
		help = maxp.x - maxPt.x;
		minp.x -= help;
		maxp.x -= help;
	}
	if(minp.y < minPt.y){
		help = minPt.y - minp.y;
		minp.y += help;
		maxp.y += help;
	}
	if(maxp.y > maxPt.y){
		help = maxp.y - maxPt.y;
		minp.y -= help;
		maxp.y -= help;
	
	}

	// Save forced position.
	A.maxPt = maxp;
	A.minPt = minp;
}

bool Rect::isPtInside(const Vec2& pt)
{
	return pt.x >= minPt.x && pt.y >= minPt.y &&
		pt.x <= maxPt.x && pt.y <= maxPt.y;
}

