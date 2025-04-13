#pragma once

#include <vector>

#include "Raylib/raylib.h"

struct Ball
{
	float radius;
	Vector2 pos;
	Vector2 vel;
};

struct Breakout
{
	int screenWidth, screenHeight;

	float pWidth, pHeight;
	Vector2 pPos;
	int pVelX;

	static const int bQuantity = 27;
	float bWidth, bHeight;
	Vector2 bPos;
	float bSpace;

	Color bBricks[bQuantity][bQuantity];

	Color bPalette[14] = {
		MAROON,RED,ORANGE,GOLD,YELLOW,
		DARKGREEN,LIME,GREEN,DARKBLUE,
		BLUE,SKYBLUE,DARKPURPLE,VIOLET,PURPLE
	};

	std::vector<Ball> balls;

	void InitBricks_Crystal();

	void UpdatePositions();
	
	void DetectCollisions();

	void RunLoop();

	Breakout(const int _screenWidth, const int _screenHeight)
		: screenWidth(_screenWidth)
		, screenHeight(_screenHeight)
		, pWidth(screenWidth * 0.1f)
		, pHeight(screenHeight * 0.02f)
		, pPos({ (screenWidth / 2.0f) - (pWidth / 2.0f), screenHeight - (pHeight * 1.5f) })
		, pVelX(screenWidth * 0.008f)
		, bWidth((screenWidth * 0.81f) / bQuantity)
		, bHeight((screenHeight * 0.55f) / bQuantity)
		, bPos({ bWidth, bHeight })
		, bSpace((screenHeight / 4.0f) / bQuantity)
	{
		Ball ball;
		ball.radius = (bHeight + bWidth) / 8.0f;
		ball.pos.x = screenWidth / 2.0f;
		ball.pos.y = pPos.y - ball.radius;
		ball.vel.x = 3.0f;
		ball.vel.y = 3.0f;
		balls.reserve(15);
		balls.emplace_back(ball);
	}
};