// AirHockeyGame.h
// By Yosbi Alves
// Yosbito@gmail.com

#ifndef AIR_HOCKEY_GAME_H
#define AIR_HOCKEY_GAME_H

#include <windows.h>
#include "Sprite.h"
#include "Rect.h"
#include "Circle.h"

class AirHockeyGame
{
public:
	AirHockeyGame(HINSTANCE hAppInst, HWND hMainWnd, Vec2 wndCenterPt);
	~AirHockeyGame();

	void pause();
	void unpause();

	void update(float dt);
	void draw(HDC hBackBufferDC, HDC hSpriteDC);

private:
	void updateBluePaddle(float dt);
	void updateRedPaddle(float dt);
	void updatePuck(float dt);
	bool paddlePuckCollision(Sprite* paddle);
	void increaseScore(bool blue);
public:
	float RED_SPEED;
	bool mPaused;

private:
	HINSTANCE mhAppInst;
	HWND mhMainWnd;
	Vec2 mWndCenterPt;

	int mBlueScore;
	int mRedScore;

	

	const float MAX_PUCK_SPEED;
	//for difficult change the red paddle speed must change
	//const float RED_SPEED;

	float mRedRecoverTime;

	Sprite* mGameBoard;
	Sprite* mBluePaddle;
	Sprite* mRedPaddle;
	Sprite* mPuck;

	POINT mLastMousePos;
	POINT mCurrMousePos;

	Rect mBlueBounds; 
	Rect mRedBounds;
	Rect mBoardBounds;
	Rect mBlueGoal;
	Rect mRedGoal;
};

#endif // AIR_HOCKEY_GAME_H