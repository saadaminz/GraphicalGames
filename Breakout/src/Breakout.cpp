
#include <iostream>

#include "Breakout.h"

void Breakout::InitBricks_Crystal()
{
	int center = bQuantity / 2;

	for (int i = 0; i < bQuantity; i++)
	{
		for (int j = 0; j < bQuantity; j++)
		{
			int distance = abs(i - center) + abs(j - center);

			if (distance <= center)
			{
				bBricks[i][j] = bPalette[distance];
			}
			else {
				bBricks[i][j] = BLACK;
			}
		}
	}
}

void Breakout::UpdatePositions()
{
	DrawRectangle(pPos.x, pPos.y, pWidth, pHeight, WHITE);

	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
	{
		pPos.x -= pVelX;

		if (pPos.x < 0)
		{
			pPos.x += pVelX;
		}
	}

	else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
	{
		pPos.x += pVelX;

		if (pPos.x > screenWidth - pWidth)
		{
			pPos.x -= pVelX;
		}
	}

	for (int i = 0; i < balls.size(); i++)
	{
		Ball& ball = balls[i];
		
		DrawCircle(ball.pos.x, ball.pos.y, ball.radius, WHITE);

		ball.pos.x += ball.vel.x;
		ball.pos.y += ball.vel.y;

		if (ball.pos.x < ball.radius || ball.pos.x >(screenWidth - ball.radius))
		{
			ball.vel.x *= -1;
		}

		if (ball.pos.y < ball.radius)
		{
			ball.vel.y *= -1;
		}

		else if (ball.pos.y > (screenHeight - ball.radius))
		{
			ball.vel.y *= -1;
		}
	}
}

void Breakout::DetectCollisions()
{
	bPos.x = bWidth;
	bPos.y = bHeight;

	for (int i = 0; i < bQuantity; i++)
	{
		for (int j = 0; j < bQuantity; j++)
		{
			if (ColorToInt(bBricks[i][j]) != ColorToInt(BLACK))
			{
				DrawRectangle(bPos.x, bPos.y, bWidth, bHeight, bBricks[i][j]);

				for (int k = 0; k < balls.size(); k++)
				{
					Ball& ball = balls[k];

					if (CheckCollisionCircleRec({ ball.pos.x, ball.pos.y }, ball.radius, { bPos.x, bPos.y, bWidth, bHeight }))
					{
						bBricks[i][j] = BLACK;

						float top = (ball.pos.y + ball.radius) - bPos.y;
						float bottom = (bPos.y + bHeight) - (ball.pos.y - ball.radius);

						float left = (ball.pos.x + ball.radius) - bPos.x;
						float right = (bPos.x + bWidth) - (ball.pos.x - ball.radius);

						if ((top < left && top < right) || (bottom < left && bottom < right))
						{
							ball.vel.y *= -1;
						}
						else {
							ball.vel.x *= -1;
						}
					}

					if (CheckCollisionCircleRec({ ball.pos.x, ball.pos.y }, ball.radius, { pPos.x, pPos.y, pWidth, pHeight }))
					{
						ball.pos.y = pPos.y - ball.radius;

						float section = pWidth / 5;

						if (ball.pos.x < pPos.x + section)
						{
							ball.vel.x = -std::abs(ball.vel.x);
						}

						else if (ball.pos.x > pPos.x + section * 4)
						{
							ball.vel.x = std::abs(ball.vel.x);
						}

						ball.vel.y *= -1;
					}
				}
			}
			bPos.x += bWidth + bSpace;
		}
		bPos.x = bWidth;
		bPos.y += bHeight + bSpace;
	}
}

void Breakout::RunLoop()
{
	InitWindow(screenWidth, screenHeight, "Breakout");
	SetTargetFPS(60);
	InitBricks_Crystal();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		UpdatePositions();
		DetectCollisions();

		EndDrawing();
	}

	CloseWindow();
}

int main()
{
	Breakout breakout(1280, 720);
	breakout.RunLoop();
}