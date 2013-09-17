//////////////////////////////////////////////////////////////////////////
#include "AIE.h"
#include "KeyCodes.h"
#include "PaddleHB.h"
#include <string>
#include <cmath>
#include <time.h>
#include <assert.h>
#include <crtdbg.h>
#include <iostream>

const int iSpeed = 4;
const int iScreenX = 1280;
const int iScreenY = 780;

const int iPlayerOneX = 100;
const int iPlayerOneW = 100;
const int iPlayerOneH = 144;

const int iPlayerTwoX = 1200;
const int iPlayerTwoW = 100;
const int iPlayerTwoH = 144;

const int iBallW = 64;
const int iBallH = 64;

unsigned int iBG = -1;
moveableObject oPlayerOne = {iPlayerOneX, 500, 0 ,0, -1, iPlayerOneW, iPlayerOneH};
moveableObject oPlayerTwo = {iPlayerTwoX, 500, 0 ,0, -1, iPlayerTwoW, iPlayerTwoH};
moveableObject oBall = {640, 390, 1+(rand()%40)/100, 1+(rand()%50)/100, -1, iBallW, iBallH};
int g_highScores[5] = {0};
bool g_gameOver = false;

int iPlayerOneScore = 0;
int iPlayerTwoScore = 0;
int iFrameCounter = 0;

vector vectorSubtract(vector &v, float fS)
{
	vector vResult = {v.fX - fS, v.fY - fS};
	return vResult;
}

vector vectorAdd(vector &v, float fS)
{
	vector vResult = {v.fX + fS, v.fY + fS};
	return vResult;
}

vector vectorScalar(vector &v, float fS)
{
	vector vResult = {v.fX * fS, v.fY * fS};
	return vResult;
}

vector vectorSubtract(vector &v, vector & v2)
{
	vector vResult = {v.fX - v2.fX, v.fY - v2.fY};
	return vResult;
}

vector vectorAdd(vector &v, vector & v2)
{
	vector vResult = {v.fX + v2.fX, v.fY + v2.fY};
	return vResult;
}

vector getNormal(vector &v)
{
	float fMag = sqrt(v.fX * v.fX + v.fY * v.fY);
	vector vResult = {v.fX / fMag, v.fY / fMag};
	return vResult;
}

float getMagnitude(vector &v)
{
	return sqrt(v.fX * v.fX + v.fY * v.fY);
}

void sort(int iArray[], int iSize)
{
	for(int i = 0; i < iSize; i++)
	{
		for(int j = 0; j < iSize; j++)
		{
			if(iArray[i] > iArray[j])
			{
				int tmp = iArray[i];
				iArray[i] = iArray[j];
				iArray[j] = tmp;
			}
		}
	}
}



void updateBallPosition(moveableObject &obj)
{
	obj.iPosition = vectorAdd(obj.iPosition, obj.iSpeed);
}



void seek(moveableObject &player, moveableObject& ball)
{
	player.iSpeed.fY = ball.iSpeed.fY;
}

void CollisionDetect(moveableObject &player, moveableObject &ball, int p)
{
	int xMin = player.iPosition.fX - player.iWidth/2;
	int xMax = player.iPosition.fX + player.iWidth/2;
	int yMin = player.iPosition.fY - player.iHeight/2;
	int yMax = player.iPosition.fY + player.iHeight/2;

	if(p == 1 && ball.iSpeed.fX < 0)
	{
		if(ball.iPosition.fX > xMin && ball.iPosition.fX < xMax)
		{
			if(ball.iPosition.fY > yMin && ball.iPosition.fY < yMax)
			{
				ball.iPosition.fX -= 2;
				ball.iSpeed.fX *= -1;
			}
		}
	}
	if(p == 2 && ball.iSpeed.fX > 0)
	{
		if(ball.iPosition.fX > xMin && ball.iPosition.fX < xMax)
		{
			if(ball.iPosition.fY > yMin && ball.iPosition.fY < yMax)
			{
				ball.iPosition.fX -= 2;
				ball.iSpeed.fX *= -1;
			}
		}
	}
}
void initGame()
{
	srand(time(0));

	iBG = CreateSprite("./images/halo.jpg", iScreenX, iScreenY, true);
	MoveSprite(iBG, iScreenX>>1, iScreenY>>1);

	oPlayerOne.iSprite = CreateSprite("./images/THEL-VADAM.png", iPlayerOneW, iPlayerOneH, true);
	oPlayerTwo.iSprite = CreateSprite("./images/Master_Chief_Halo_3.jpg", iPlayerTwoW, iPlayerTwoH, true);
	oBall.iSprite = CreateSprite("./images/343_Guilty_Spark-transparent.png", iBallW, iBallH, true);
}

void destroyGame()
{
		DestroySprite(iBG);
		DestroySprite(oPlayerOne.iSprite);
		DestroySprite(oPlayerTwo.iSprite);
		DestroySprite(oBall.iSprite);
}

void testOnScreen(moveableObject &obj)
{
	//if out of screen bounds move position to on screen 
	if(obj.iPosition.fX > iScreenX)
	{
		obj.iPosition.fX = 640;
		obj.iPosition.fY = 390;
		obj.iSpeed.fX *= -1;
	}
	if(obj.iPosition.fX < 0)
	{
		obj.iPosition.fX = 640;
		obj.iPosition.fY = 390;
		obj.iSpeed.fX *= -1;
	}
	if(obj.iPosition.fY > iScreenY)
	{
		obj.iPosition.fY = iScreenY;
		obj.iSpeed.fY *= -1;
	}
	if(obj.iPosition.fY < 0)
	{
		obj.iPosition.fY = 0;
		obj.iSpeed.fY *= -1;
	}
}

void updateGame()
{
	if(g_gameOver == false)
	{
		updateBallPosition(oBall);

		if(oBall.iPosition.fX > iScreenX)
		{
			iPlayerOneScore = iPlayerOneScore + 1;
		}
		if(oBall.iPosition.fX < 0)
		{
			iPlayerTwoScore = iPlayerTwoScore + 1;
		}

		if (oBall.iSpeed.fX > 0)
			seek(oPlayerTwo, oBall);
		//else
			//seek(oPlayerOne, oBall);
	
		testOnScreen(oPlayerOne);
		testOnScreen(oPlayerTwo);
		testOnScreen(oBall);

		if(IsKeyDown(GLFW_KEY_UP))
		{
			oPlayerOne.iPosition.fY = oPlayerOne.iPosition.fY - 2;
		}
		if(IsKeyDown(GLFW_KEY_DOWN))
		{
			oPlayerOne.iPosition.fY = oPlayerOne.iPosition.fY + 2;
		}
	
		//if(IsKeyDown(GLFW_KEY_KP_8))
		//{
			//oPlayerTwo.iPosition.fY = oPlayerTwo.iPosition.fY - 2;
		//}
	
		//if(IsKeyDown(GLFW_KEY_KP_2))
		//{
			//oPlayerTwo.iPosition.fY = oPlayerTwo.iPosition.fY + 2;
		//}

		CollisionDetect(oPlayerOne,oBall,1);
		CollisionDetect(oPlayerTwo,oBall,2);

		RotateSprite(oPlayerOne. iSprite, 0);
		oPlayerOne.iPosition.fY += oPlayerOne.iSpeed.fY;
		MoveSprite(oPlayerOne. iSprite, oPlayerOne.iPosition.fX, oPlayerOne.iPosition.fY);

		RotateSprite(oPlayerTwo.iSprite, 0);
		oPlayerTwo.iPosition.fY += oPlayerTwo.iSpeed.fY;
		MoveSprite(oPlayerTwo.iSprite, oPlayerTwo.iPosition.fX, oPlayerTwo.iPosition.fY);

		MoveSprite(oBall.iSprite, (int)oBall.iPosition.fX, (int)oBall.iPosition.fY);

		if(iPlayerOneScore > iPlayerTwoScore + 2 || iPlayerTwoScore > iPlayerOneScore + 2)
		{
			g_gameOver = true;
		}
	}
}

void drawGame()
{
	static char outScoreOne[15] = {'\n'};
	static char outScoreTwo[15] = {'\n'};
	
	if(g_gameOver == false)
	{
		DrawSprite(iBG);
		DrawSprite(oBall.iSprite);
		DrawSprite(oPlayerOne.iSprite);
		DrawSprite(oPlayerTwo.iSprite);

		sprintf(outScoreOne, "Player 1: %d", iPlayerOneScore);
		sprintf(outScoreTwo, "Player 2: %d", iPlayerTwoScore);

		DrawString(outScoreOne, 50, 50, SColour(255,0,0,255));
		DrawString(outScoreTwo, 1000, 50, SColour(0,255,0,255));
	}

	if(g_gameOver == true)
	{
		DrawString("Game Over", 560, 270);
		char score[15];
		int iY = 350;
		if(iPlayerOneScore > iPlayerTwoScore)
		{
			g_highScores[1] = iPlayerOneScore;
		}	
		if(iPlayerOneScore < iPlayerTwoScore)
		{
			g_highScores[1] = iPlayerTwoScore;
		}
		if(g_highScores[1] != 0)
		{
			sprintf(score, "High Score: %d", g_highScores[1]);
			DrawString(score, 550, iY);
		}
	}
}

int main( int argc, char* argv[] )
{	
	Initialise(iScreenX, iScreenY, false);

	initGame();
	
	do
	{
		iFrameCounter++;

		if(iFrameCounter > 5000)
			iFrameCounter = 0;

		ClearScreen();

		updateGame();

		drawGame();

	}while (FrameworkUpdate() == false);

		destroyGame();

	Shutdown();
}

