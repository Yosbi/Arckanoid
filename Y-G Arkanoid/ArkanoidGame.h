#pragma once
#ifndef ARKANOIDGAME_H
#define ARKANOIDGAME_H

#include <string>
#include "Sprite.h"
#include "Block.h"
#include "BackBuffer.h"
#include "Circle.h"
#include "Rect.h"
#include "Vec2.h"
#include <list>
#include <fstream>

using namespace std;

class ArkanoidGame
{
public:
	ArkanoidGame(HINSTANCE hAppInst, HWND hMainWnd, Vec2 wndCenterPt);
	~ArkanoidGame(void);

	void pause();
	void unpause();

	void ArkMain(float dt, HDC hBackBufferDC, HDC hSpriteDC, Vec2 wndCenterPt);		
	bool looser();
	
private:
	bool update( float dt);
	bool loadLevel( char* nameLevel);
	void draw(HDC hBackBufferDC, HDC hSpriteDC);
	bool levelOver();
	void updatePaddle(float dt);
	bool updateBall(float dt);
	void paddleBallCollision();
	void ballBlockCollision();
	bool isPointCollisionLine(Vec2 &Lpt1, Vec2  &Lpt2, Vec2  &Pt);
	bool levelComplete();	
	bool ballOutOfBounds();
	void resetPaddleBall(Vec2 wndCenterPt);
	

public:
	// Var to verify if the level is finished
	bool mfinished;
	bool mPaused;
	

private:
	HINSTANCE mhAppInst;
	HWND mhMainWnd;

	Vec2 mWndDownCenterPt;

	POINT mLastMousePos;
	POINT mCurrMousePos;

	Sprite* mBack;
	Sprite* mBlue;
	Sprite* mBlueBreak;
	Sprite* mGreen;
	Sprite* mGreenBreak;
	Sprite* mOrange;
	Sprite* mOrangeBreak;
	Sprite* mPurple;
	Sprite* mPurpleBreak;
	Sprite* mYellow;
	Sprite* mYellowBreak;
	Sprite* mBall;
	Sprite* mPaddle;
	//list of blocks
	list<Block> mBlocks;	

	const float MAX_BALL_SPEED;

	Rect mBallBounds;
	Rect mPaddleBounds;

	int mNumBalls;
	
	int mnumArch;
};

#endif //ARKANOIDGAME_H