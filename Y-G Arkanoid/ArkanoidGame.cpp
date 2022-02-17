#include "ArkanoidGame.h"

#include <string>
#include "Sprite.h"
#include "Block.h"
#include "BackBuffer.h"
#include "Circle.h"
#include "Rect.h"
#include "resource.h"
#include "Vec2.h"
#include <list>
#include <fstream>
#include <cassert>
using namespace std;

ArkanoidGame::ArkanoidGame(HINSTANCE hAppInst, HWND hMainWnd, Vec2 wndCenterPt)
	: MAX_BALL_SPEED(500.0f)
{
	// Save input parameters.
	mhAppInst = hAppInst;
	mhMainWnd = hMainWnd;
	mWndDownCenterPt.x = wndCenterPt.x;
	mWndDownCenterPt.y = 650;
	
	// The game is initially paused.
	 mPaused = true;
	// To load first level
	mfinished = false;
	//to start with 3 balls
	mNumBalls = 2;
	//start at the level 1
	mnumArch = 0;
	// Create the sprites:
	Rect br;
	Circle bc;
	Vec2   p0 = wndCenterPt;
	Vec2   v0(0.0f, 0.0f);

	mBack = new Sprite(hAppInst, IDB_BACK, IDB_BACKMASK , bc, br, p0, v0);

	bc.c.x = wndCenterPt.x;
	bc.c.y = 650 + 32;
	bc.r = 50;
	br.minPt.x = bc.c.x - 38;
	br.minPt.y = bc.c.y - bc.r;
	br.maxPt.x = bc.c.x + 38;
	br.maxPt.y = bc.c.y - 7; 
	p0.x = bc.c.x;
	p0.y = bc.c.y;

	mPaddle = new Sprite(hAppInst, IDB_PADDLE, IDB_BASEMASK , bc, br, p0, v0);

	bc.c.x = wndCenterPt.x;
	bc.c.y = bc.c.y - bc.r - 6;
	bc.r = 5;
	br = Rect(0,0,0,0);
	p0.x = bc.c.x;
	p0.y = bc.c.y;
	v0.x = 0.0f;
	v0.y = 300.0f;
	mBall = new Sprite(hAppInst, IDB_BALL, IDB_BALLMASK , bc, br, p0, v0);

	//initializing blocks
	bc.c = Vec2(0.0f, 0.0f);
	p0 = Vec2(0.0f, 0.0f);
	v0 = Vec2(0.0f, 0.0f);
	br = Rect(-25 , -10, 25, 10);
	mBlue = new Sprite(hAppInst, IDB_BLUE, IDB_BLOCKMASK , bc, br, p0, v0);
	mBlueBreak = new Sprite(hAppInst, IDB_BLUEBREAK, IDB_BLOCKMASK , bc, br, p0, v0);
	mGreen = new Sprite(hAppInst, IDB_GREEN, IDB_BLOCKMASK , bc, br, p0, v0);
	mGreenBreak = new Sprite(hAppInst, IDB_GREENBREAK, IDB_BLOCKMASK , bc, br, p0, v0);
	mOrange = new Sprite(hAppInst, IDB_ORANGE, IDB_BLOCKMASK , bc, br, p0, v0);
	mOrangeBreak = new Sprite(hAppInst, IDB_ORANGEBREAK, IDB_BLOCKMASK , bc, br, p0, v0);
	mPurple = new Sprite(hAppInst, IDB_PURPLE, IDB_BLOCKMASK , bc, br, p0, v0);
	mPurpleBreak = new Sprite(hAppInst, IDB_PURPLEBREAK, IDB_BLOCKMASK , bc, br, p0, v0);
	mYellow = new Sprite(hAppInst, IDB_YELLOW, IDB_BLOCKMASK , bc, br, p0, v0);
	mYellowBreak = new Sprite(hAppInst, IDB_YELLOWBREAK, IDB_BLOCKMASK , bc, br, p0, v0);
	

	// Initialize rect
	mBallBounds = Rect(29, 40, 596, 660);
	mPaddleBounds = Rect(29, 632, 596, 650);
	//

}


ArkanoidGame::~ArkanoidGame(void)
{
	delete mBack;
	delete mPaddle;
	delete mBall;
	delete mBlue;
	delete mBlueBreak;
	delete mGreen;
	delete mGreenBreak;
	delete mOrange;
	delete mOrangeBreak;
	delete mPurple;
	delete mPurpleBreak;
	delete mYellow;
	delete mYellowBreak;
}

void ArkanoidGame::pause(){
	if(!mPaused){
		mPaused = true;
		// Game is unpaused--release capture on mouse.
		ReleaseCapture();

		// Show the mouse cursor when paused.
		ShowCursor(true); }
}

void ArkanoidGame::unpause(){
	if(mPaused){
		// Fix cursor to paddle position.
		POINT p = mPaddle->mPosition;
		ClientToScreen(mhMainWnd, &p);

		SetCursorPos(p.x, p.y);
		GetCursorPos(&mLastMousePos);

		mPaused = false;
	
		// Capture the mouse when not paused.
		SetCapture(mhMainWnd);

		// Hide the mouse cursor when not paused.
		ShowCursor(false); 
	
	}
}

bool ArkanoidGame::update( float dt){
	if(!mPaused){
		updatePaddle(dt);
		//returns true if ball is out of bounds
		return updateBall(dt);
		return false;
	}
	return false;
}

void ArkanoidGame::updatePaddle(float dt){

	// Obtain cursor pos
	GetCursorPos(&mCurrMousePos);

	// Change in mouse position.
	int dx = (mCurrMousePos.x) - mLastMousePos.x;
	int dy = 0;

	Vec2 dp((float)dx, (float)dy);

	// Velocity is change in position with respect to time.
	mPaddle->mVelocity = dp / dt;
	
	mPaddle->update( dt );
	//the rect of the circle is sized with the width/height
	//of the bit map, we must to change that to the size of the paddle
	mPaddle->mBoundingRect.minPt.x = mPaddle->mBoundingCircle.c.x - 38;
	mPaddle->mBoundingRect.minPt.y = mPaddle->mBoundingCircle.c.y - mPaddle->mBoundingCircle.r;
	mPaddle->mBoundingRect.maxPt.x = mPaddle->mBoundingCircle.c.x + 38;
	mPaddle->mBoundingRect.maxPt.y = mPaddle->mBoundingCircle.c.y - 32;

//	// Make sure the paddle stays inbounds
	mPaddleBounds.forceInside(mPaddle->mBoundingRect);
//	//update position if changed
	mPaddle->mPosition.x = mPaddle->mBoundingRect.minPt.x + 38;
	mPaddle->mPosition.y = mPaddle->mBoundingRect.minPt.y + mPaddle->mBoundingCircle.r;
	mPaddle->mBoundingCircle.c = mPaddle->mPosition;

	// The current position is now the last mouse position.
	// plus 38 for the cursor stay in the client rect
	mLastMousePos.x = (long)mPaddle->mPosition.x;
	mLastMousePos.y = (long)mPaddle->mPosition.y - 32;

	// Keep mouse cursor fixed to paddle.
	ClientToScreen(mhMainWnd, &mLastMousePos);
	SetCursorPos(mLastMousePos.x, mLastMousePos.y);

}

bool ArkanoidGame::updateBall(float dt){
	mBall->update(dt);

	paddleBallCollision();

	// Clamp puck speed to some maximum velocity; this provides
	// good stability.
	if( mBall->mVelocity.length() >= MAX_BALL_SPEED )
		mBall->mVelocity.normalize() *= MAX_BALL_SPEED;	

	// Did the ball hit a wall?  If so, reflect about edge.  
	Circle ballCircle = mBall->mBoundingCircle;
	if( ballCircle.c.x - ballCircle.r < mBallBounds.minPt.x )
		mBall->mVelocity.x *= -1.0f;
	if( ballCircle.c.x + ballCircle.r > mBallBounds.maxPt.x )
		mBall->mVelocity.x *= -1.0f;
	if( ballCircle.c.y - ballCircle.r < mBallBounds.minPt.y )
		mBall->mVelocity.y *= -1.0f;
	if( ballCircle.c.y >= mBallBounds.maxPt.y )
		return true;

	// Make sure ball stays inbounds of the gameboard.
	mBallBounds.forceInside(mBall->mBoundingCircle);
	mBall->mPosition = mBall->mBoundingCircle.c;

	// Did the ball hits a block?
	ballBlockCollision();



	return false;

}
void ArkanoidGame::paddleBallCollision(){
	Vec2 normal;
	if(mPaddle->mBoundingCircle.hits(mBall->mBoundingCircle, normal)){

		//hits updates the ball position  So update ball position as well
		// since the two correspond.
		mBall->mPosition = mBall->mBoundingCircle.c;

		// Compute the paddle's velocity relative to the ball's
		// velocity. But makin sure thar the paddle velocity 
		// in y axis is 0 because paddle is mouse handled and
		// iam fosing the mouse up
		Vec2 relVel = mPaddle->mVelocity - mBall->mVelocity;

		// Get the component of the relative velocity along the normal.
		float impulseMag = relVel.dot(normal);

		// Are the objects getting closer together?
		if( impulseMag >= 0.0f )
		{
			// Project the relative velocity onto the normal.
			Vec2 impulse = impulseMag * normal; 

			// Add the impulse to the puck.
			mBall->mVelocity += 2.0f * impulse;


		}
	}
}
bool ArkanoidGame::isPointCollisionLine(Vec2 &Lpt1, Vec2 &Lpt2, Vec2  &Pt){
	// this give me 10 Px of range of collision
	if(((Pt.x >= Lpt1.x) && (Pt.x <= Lpt2.x)) && ((Pt.y <= Lpt1.y + 6) && (Pt.y >= Lpt2.y - 6)))
		return true;
	else if(((Pt.y >= Lpt1.y) && (Pt.y <= Lpt2.y)) && ((Pt.x >= Lpt1.x - 6) && (Pt.x <= Lpt2.x + 6)))
		return true;
	else
		return false;

}


void ArkanoidGame::ballBlockCollision(){
	
	Circle ballCircle = mBall->mBoundingCircle;
	list<Block>::iterator i;
	// checking collision in the mBlock list
	for(i = mBlocks.begin(); i != mBlocks.end(); i++){
	
		if(isPointCollisionLine(i->mRect.minPt, Vec2(i->mRect.minPt.x, i->mRect.maxPt.y), Vec2(ballCircle.c.x + ballCircle.r, ballCircle.c.y))){
			//reassin position to the ball
			mBall->mPosition.x = i->mRect.minPt.x - mBall->mBoundingCircle.r - 6;
			mBall->mBoundingCircle.c = mBall->mPosition;
			mBall->mVelocity.x *= -1.0f;
			//mBall->mVelocity = Vec2(0,0);
			if((i->mNumCollision) && (!i->mCountCollision))
				i->mCountCollision = 1;
			else
				i = mBlocks.erase(i);
			break;			
		}
		
		if(isPointCollisionLine(Vec2(i->mRect.maxPt.x, i->mRect.minPt.y), i->mRect.maxPt, Vec2(ballCircle.c.x - ballCircle.r, ballCircle.c.y))){
			//reassin position to the ball
			mBall->mPosition.x = i->mRect.maxPt.x + mBall->mBoundingCircle.r + 6;
			mBall->mBoundingCircle.c = mBall->mPosition;
			mBall->mVelocity.x *= -1.0f;
			if((i->mNumCollision) && (!i->mCountCollision))
				i->mCountCollision = 1;
			else
				i = mBlocks.erase(i);
			break;			
		}
		
		if(isPointCollisionLine(Vec2(i->mRect.minPt.x, i->mRect.maxPt.y), i->mRect.maxPt, Vec2(ballCircle.c.x, ballCircle.c.y - ballCircle.r))){
			//reassin position to the ball
			mBall->mPosition.y = i->mRect.maxPt.y + mBall->mBoundingCircle.r + 6;
			mBall->mBoundingCircle.c = mBall->mPosition;
			mBall->mVelocity.y *= -1.0f;
			if((i->mNumCollision) && (!i->mCountCollision))
				i->mCountCollision = 1;
			else
				i = mBlocks.erase(i);
			break;
		}
		
		if(isPointCollisionLine(i->mRect.minPt, Vec2(i->mRect.maxPt.x, i->mRect.minPt.y), Vec2(ballCircle.c.x, ballCircle.c.y + ballCircle.r))){
			//reassin position to the ball
			mBall->mPosition.y = i->mRect.minPt.y - mBall->mBoundingCircle.r - 6;
			mBall->mBoundingCircle.c = mBall->mPosition;
			mBall->mVelocity.y *= -1.0f;
			if((i->mNumCollision) && (!i->mCountCollision))
				i->mCountCollision = 1;
			else
				i = mBlocks.erase(i);
			break;			
		}	
	}
}

bool ArkanoidGame::loadLevel(char* nameLevel){
	/*ofstream ot(nameLevel);
	ot<< "hola";
	ot.close();*/
	ifstream infile(nameLevel);
	if(infile){
		while(!infile.eof()){
		
			Block bl;

			//reading 
			infile >> bl.mCenterPoint.x;
			infile >> bl.mCenterPoint.y;
			infile >> bl.mColor;
			infile >> bl.mNumCollision;

			bl.mRect = Rect(bl.mCenterPoint.x - 25,
							bl.mCenterPoint.y - 10,
							bl.mCenterPoint.x + 25,
							bl.mCenterPoint.y + 10);	
			bl.mCountCollision = 0;

			mBlocks.push_back(bl);			
		}
		//mBlocks.pop_back();
		infile.close();
		return true;
	}
	return false;
}

void ArkanoidGame::draw(HDC hBackBufferDC, HDC hSpriteDC){
//	// Draw the sprites.
	mBack->draw(hBackBufferDC, hSpriteDC);
	mPaddle->draw(hBackBufferDC, hSpriteDC);
	mBall->draw(hBackBufferDC, hSpriteDC);	
	
	list<Block>::iterator i;
	for(i = mBlocks.begin(); i != mBlocks.end(); i++){
		if(i->mColor == 'b'){
			if((i->mNumCollision) && (i->mCountCollision)){
				mBlueBreak->mPosition = i->mCenterPoint;
				mBlueBreak->draw(hBackBufferDC, hSpriteDC);
			}
			else{
				mBlue->mPosition = i->mCenterPoint;
				mBlue->draw(hBackBufferDC, hSpriteDC);
				
			}
			
		}
		
		else if(i->mColor == 'g'){
			if((i->mNumCollision) && (i->mCountCollision)){
				mGreenBreak->mPosition = i->mCenterPoint;
				mGreenBreak->draw(hBackBufferDC, hSpriteDC);
			}
			else{
				mGreen->mPosition = i->mCenterPoint;
				mGreen->draw(hBackBufferDC, hSpriteDC);
				
			}
			
		}

		else if(i->mColor == 'o'){
			if((i->mNumCollision) && (i->mCountCollision)){
				mOrangeBreak->mPosition = i->mCenterPoint;
				mOrangeBreak->draw(hBackBufferDC, hSpriteDC);
			}
			else{
				mOrange->mPosition = i->mCenterPoint;
				mOrange->draw(hBackBufferDC, hSpriteDC);
				
			}
			
		}

		else if(i->mColor == 'p'){
			if((i->mNumCollision) && (i->mCountCollision)){
				mPurpleBreak->mPosition = i->mCenterPoint;
				mPurpleBreak->draw(hBackBufferDC, hSpriteDC);
			}
			else{
				mPurple->mPosition = i->mCenterPoint;
				mPurple->draw(hBackBufferDC, hSpriteDC);
				
			}
			
		}

		else if(i->mColor == 'y'){
			if((i->mNumCollision) && (i->mCountCollision)){
				mYellowBreak->mPosition = i->mCenterPoint;
				mYellowBreak->draw(hBackBufferDC, hSpriteDC);
			}
			else{
				mYellow->mPosition = i->mCenterPoint;
				mYellow->draw(hBackBufferDC, hSpriteDC);
				
			}
			
		}
	
	}
	char var[32];
	sprintf_s(var, "Level = %d", mnumArch);

	SetBkMode(hBackBufferDC, TRANSPARENT);
	TextOut(hBackBufferDC, 47, 17, var, (int)strlen(var));

	sprintf_s(var, "lives = %d", mNumBalls);
	TextOut(hBackBufferDC, 374, 17, var, (int)strlen(var));
//
}

void ArkanoidGame::ArkMain(float dt, HDC hBackBufferDC, HDC hSpriteDC, Vec2 wndCenterPt){
	int liveLess;
	char fileName[32];
	static bool archRead = false;
	if(!looser()){
		if(levelComplete()){
			//set next arch to read
			mnumArch ++;
			sprintf_s(fileName,"level%d.txt", mnumArch);
			////reseting paddle/ball status
			resetPaddleBall(wndCenterPt);
			//one extra ball
			mNumBalls++;
			archRead = loadLevel(fileName);
			pause();
		
		}
		if(archRead){
			//assert(!ballOutOfBounds());
			if(ballOutOfBounds()){
				resetPaddleBall(wndCenterPt);
				mNumBalls--;
				pause();
			}
			else{
				//assert(levelComplete());
				// Update the game and draw everything.
				liveLess = update(dt);
				// Draw every frame.
				draw(hBackBufferDC, hSpriteDC);
			}
		}
		//if you win
		else{
			char var[255];
			SetBkMode(hBackBufferDC, TRANSPARENT);
			sprintf_s(var, "YOU WIN!!!");
			TextOut(hBackBufferDC, (int)wndCenterPt.x - 25, (int)wndCenterPt.y, var, (int)strlen(var));
			sprintf_s(var, "Press L-Mouse button to reestart");
			TextOut(hBackBufferDC, (int)wndCenterPt.x - 100, (int)wndCenterPt.y + 25, var, (int)strlen(var));
			mfinished = true;
			pause();
		}
	}
	else{
		char var[255];
		SetBkMode(hBackBufferDC, TRANSPARENT);
		sprintf_s(var, "GAME OVER");
		TextOut(hBackBufferDC, (int)wndCenterPt.x - 25, (int)wndCenterPt.y, var, (int)strlen(var));
		sprintf_s(var, "Press L-Mouse button to reestart");
		TextOut(hBackBufferDC, (int)wndCenterPt.x - 100, (int)wndCenterPt.y + 25, var, (int)strlen(var));
		sprintf_s(var, "lives = %d", mNumBalls);
		TextOut(hBackBufferDC, 374, 17, var, (int)strlen(var));
		pause();
	}
}
bool ArkanoidGame::levelComplete(){
	return mBlocks.empty();
}
bool ArkanoidGame::looser(){
	return mNumBalls == 0;
}
bool ArkanoidGame::ballOutOfBounds(){
	return mBall->mPosition.y >= mBallBounds.maxPt.y - 7;
}
void ArkanoidGame::resetPaddleBall(Vec2 wndCenterPt){

	mPaddle->mBoundingCircle.c.x = wndCenterPt.x;
	mPaddle->mBoundingCircle.c.y = 650 + 32;
	mPaddle->mBoundingRect.minPt.x = mPaddle->mBoundingCircle.c.x - 38;
	mPaddle->mBoundingRect.minPt.y = mPaddle->mBoundingCircle.c.y - mPaddle->mBoundingCircle.r;
	mPaddle->mBoundingRect.maxPt.x = mPaddle->mBoundingCircle.c.x + 38;
	mPaddle->mBoundingRect.maxPt.y = mPaddle->mBoundingCircle.c.y - 7; 
	mPaddle->mPosition.x = mPaddle->mBoundingCircle.c.x;
	mPaddle->mPosition.y = mPaddle->mBoundingCircle.c.y;

	mBall->mBoundingCircle.c.x = wndCenterPt.x;
	mBall->mBoundingCircle.c.y = mPaddle->mBoundingCircle.c.y - mPaddle->mBoundingCircle.r - 6;
	mBall->mPosition.x = mBall->mBoundingCircle.c.x;
	mBall->mPosition.y = mBall->mBoundingCircle.c.y;
	mBall->mVelocity.x = 0.0f;
	mBall->mVelocity.y = 300.0f;
}