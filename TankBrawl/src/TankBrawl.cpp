
#include "TankBrawl.h"

void TankBrawl::LoadTextures()
{
	maps.reserve(3);

	// change to proper exe-build paths later
	for (const auto& entry : std::filesystem::directory_iterator("C:/Users/Saad/source/repos/GraphicalGames/TankBrawl/resources/maps/"))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".png")
		{
			Texture2D texture = LoadTexture(entry.path().string().c_str());

			texture.width = static_cast<int>(texture.width * mapScale);
			texture.height = static_cast<int>(texture.height * mapScale);

			maps.push_back(texture);
		}
	}

	if (maps.empty())
	{
		// find reason : return with reason : return/close => enclose in try-catch
	}

	tanks.reserve(3);
	//tanksTex.reserve(3);

	int i = 0;
	float posDistance = 0;

	for (const auto& entry : std::filesystem::directory_iterator("C:/Users/Saad/source/repos/GraphicalGames/TankBrawl/resources/tanks/"))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".png")
		{
			Texture2D texture = LoadTexture(entry.path().string().c_str());

			texture.width = static_cast<int>(texture.width * tankScale);
			texture.height = static_cast<int>(texture.height * tankScale);

			Tank tank(i);

			tank.sprite = texture;
			tank.pos.x = 75 + posDistance;
			tank.pos.y = 55 + posDistance;
			tank.lastPos = tank.pos;

			//tank.color = DecideColor(entry.path().string().c_str());
			tank.color = WHITE;

			/*if (i == 0)
				tank.color = GREEN;
			else if (i == 1)
				tank.color = BLUE;
			else if (i == 2)
				tank.color = RED;*/

			tank.hitBox = Rectangle{ tank.pos.x - texture.width / 2.0f,
				tank.pos.y - texture.height / 2.0f,
				(float)texture.width,
				(float)texture.height
			};

			tanks.push_back(tank);

			//tanksTex.push_back(texture);

			i++;
			posDistance += 200;
		}
	}

	if (tanks.empty())
	{
		// find reason : return with reason : return/close => enclose in try-catch
	}
}

void TankBrawl::UnloadTextures()
{
	for (auto& tex : maps)
	{
		UnloadTexture(tex);
	}

	for (auto& tank : tanks)
	{
		UnloadTexture(tank.sprite);
	}

	//UnloadTexture(menu);
}

void TankBrawl::MainMenu()
{
	/*InitWindow(screenWidth, screenHeight, "TankBrawl");
	SetTargetFPS(60);*/

	menu = LoadTexture("C:/Users/Saad/source/repos/GraphicalGames/TankBrawl/resources/menu.png");
	float menuScale = 2.9f;

	float frameWidth = menu.width;
	float frameHeight = menu.height / 4;

	Rectangle twoPlayerButton = { 300, 230, 400, 90 };
	Rectangle threePlayerButton = { 300, 340, 400, 90 };
	Rectangle exitButton = { 300, 450, 400, 90 };

	int currentFrame = 0;

	TankBrawl::LoadTextures();

	bool start = false;

	while (!WindowShouldClose() && !start)
	{
		BeginDrawing();
		ClearBackground(BLACK);

		Rectangle srcRect = { 0.0f, (float)(currentFrame * frameHeight), (float)frameWidth, (float)frameHeight };
		Rectangle dstRect = { 135, 10, (float)frameWidth * menuScale, (float)frameHeight * menuScale };

		DrawTexturePro(menu,
			srcRect,
			dstRect,
			Vector2{ 0.0f, 0.0f },
			0.0f,
			WHITE
		);

		/*DrawRectangleRec(twoPlayerButton, Fade(LIGHTGRAY, 0.4f));
		DrawRectangleRec(threePlayerButton, Fade(LIGHTGRAY, 0.4f));
		DrawRectangleRec(exitButton, Fade(LIGHTGRAY, 0.4f));*/

		Vector2 mousePos = GetMousePosition();
		
		bool hoverTwoButton = CheckCollisionPointRec(mousePos, twoPlayerButton);
		bool hoverThreeButton = CheckCollisionPointRec(mousePos, threePlayerButton);
		bool hoverExitButton = CheckCollisionPointRec(mousePos, exitButton);

		if (hoverTwoButton)
		{
			currentFrame = 1;
		}
		else if (hoverThreeButton)
		{
			currentFrame = 2;
		}
		else if (hoverExitButton)
		{
			currentFrame = 3;
		}
		else {
			currentFrame = 0;
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if (hoverTwoButton)
			{
				playersCount = 2;
				start = true;
			}
			else if (hoverThreeButton)
			{
				playersCount = 3;
				start = true;
			}
			else if (hoverExitButton)
			{
				return;
			}
		}

		EndDrawing();
	}

	UnloadTexture(menu);
	
	RunGame();

	//TankBrawl::UnloadTextures();
	//CloseWindow();
}

void TankBrawl::DrawMap()
{
	Texture2D& map = maps[playedGames];

	DrawTexturePro(map,
		Rectangle{ 0.0f, 0.0f, (float)map.width, (float)map.height },
		Rectangle{ 35, 10, (float)map.width, (float)map.height },
		Vector2{ 0.0f, 0.0f },
		0.0f,
		WHITE
	);
}

Color TankBrawl::DecideColor(const std::string& fileName)
{
	size_t firstUnderscorePos = fileName.find('_');
	size_t secondUnderscorePos = fileName.find('_', firstUnderscorePos + 1);

	std::string colorName = fileName.substr(0, secondUnderscorePos);

	if (colorName == "tank_green")
		return GREEN;
	else if (colorName == "tank_blue")
		return BLUE;
	else if (colorName == "tank_red")
		return RED;
}

void TankBrawl::HandleTanks()
{
	for (int i = 0; i < playersCount; i++)
	{
		Tank& tank = tanks[i];
		tank.moved = false;

		if (tank.id == 0 || tank.id == 2)
		{
			if (tank.id == 0)
			{
				if (IsKeyDown(KEY_A))
				{
					tank.angle -= 2.0f;
					tank.moved = true;
				}

				else if (IsKeyDown(KEY_D))
				{
					tank.angle += 2.0f;
					tank.moved = true;
				}
			}

			else if (tank.id == 2)
			{
				if (IsKeyDown(KEY_LEFT))
				{
					tank.angle -= 2.0f;
					tank.moved = true;
				}

				else if (IsKeyDown(KEY_RIGHT))
				{
					tank.angle += 2.0f;
					tank.moved = true;
				}
			}

			tank.angle = fmod(tank.angle, 360.0f);
			if (tank.angle < 0)
				tank.angle += 360.0f;

			tank.dir.x = sin(tank.angle * DEG2RAD);
			tank.dir.y = -cos(tank.angle * DEG2RAD);

			if (tank.id == 0)
			{
				if (IsKeyDown(KEY_W))
				{
					tank.pos.x += tank.dir.x * tank.vel.x * tank.speed;
					tank.pos.y += tank.dir.y * tank.vel.y * tank.speed;
					tank.lastPos = tank.pos;
					tank.moved = true;
				}

				else if (IsKeyDown(KEY_S))
				{
					tank.pos.x -= tank.dir.x * tank.vel.x * tank.speed;
					tank.pos.y -= tank.dir.y * tank.vel.y * tank.speed;
					tank.lastPos = tank.pos;
					tank.moved = true;
				}
			}

			else if (tank.id == 2)
			{
				if (IsKeyDown(KEY_UP))
				{
					tank.pos.x += tank.dir.x * tank.vel.x * tank.speed;
					tank.pos.y += tank.dir.y * tank.vel.y * tank.speed;
					tank.lastPos = tank.pos;
					tank.moved = true;
				}

				else if (IsKeyDown(KEY_DOWN))
				{
					tank.pos.x -= tank.dir.x * tank.vel.x * tank.speed;
					tank.pos.y -= tank.dir.y * tank.vel.y * tank.speed;
					tank.lastPos = tank.pos;
					tank.moved = true;
				}
			}
		}

		else if (tank.id == 1)
		{
			Vector2 mousePos = GetMousePosition();

			Vector2 toMouse;
			toMouse.x = mousePos.x - tank.pos.x;
			toMouse.y = mousePos.y - tank.pos.y;

			float distance = sqrt(toMouse.x * toMouse.x + toMouse.y * toMouse.y);
			
			if (distance > 20.0f)
			{
				toMouse.x /= distance;
				toMouse.y /= distance;

				/*float desiredAngle = atan2f(toMouse.y, toMouse.x) * RAD2DEG + 90.0f;

				float angleDiff = fmodf(desiredAngle - tank.angle + 540.0f, 360.0f) - 180.0f;

				float rotationSpeed = 2.0f;

				if (fabs(angleDiff) < rotationSpeed)
				{
					tank.angle = desiredAngle;
				}
				else {
					tank.angle += (angleDiff > 0 ? rotationSpeed : -rotationSpeed);
				}*/

				tank.angle = atan2f(toMouse.y, toMouse.x) * RAD2DEG + 90.0f;

				if (tank.angle < 0)
					tank.angle += 360.0f;

				tank.dir.x = sin(tank.angle * DEG2RAD);
				tank.dir.y = -cos(tank.angle * DEG2RAD);
			
				tank.pos.x += toMouse.x * tank.speed * 3.5f;
				tank.pos.y += toMouse.y * tank.speed * 3.2f;
				tank.lastPos = tank.pos;
				tank.moved = true;
			}
		}

		Texture2D& texture = tanks[i].sprite;

		DrawTexturePro(texture,
			Rectangle{ 0.0f, 0.0f, (float)texture.width, (float)texture.height },
			Rectangle{ tank.pos.x, tank.pos.y, (float)texture.width, (float)texture.height },
			Vector2{ texture.width / 2.0f, texture.height / 2.0f },
			tank.angle,
			tank.color
		);

		tank.hitBox = Rectangle{ tank.pos.x - texture.width / 2.0f,
				tank.pos.y - texture.height / 2.0f,
				(float)texture.width,
				(float)texture.height
		};

		// On random but not black lines positions
		// Attach to Tank class

		/*
		#3d3d3d wall
		#777777 map surface
		#282827 tank edge
		*/
	}
}

void TankBrawl::HandleCollisions()
{
	for (int i = 0; i < playersCount; i++)
	{
		Tank& tankA = tanks[i];

		TankWallCollisions(tankA);

		for (int j = i + 1; j < playersCount; j++)
		{
			Tank& tankB = tanks[j];

			if (CheckCollisionRecs(tankA.hitBox, tankB.hitBox))
			{
				Vector2 pushDir = {
					tankA.pos.x - tankB.pos.x,
					tankA.pos.y - tankB.pos.y
				};

				float dist = sqrtf(pushDir.x * pushDir.x + pushDir.y * pushDir.y);

				if (dist == 0.0f)
					dist = 0.01f;

				pushDir.x /= dist;
				pushDir.y /= dist;

				float pushAmount = 1.2f;

				bool tankAMoving = (tankA.pos.x != tankA.lastPos.x || tankA.pos.y != tankA.pos.y);

				if (tankA.moved)
				{
					float moveDirA = tankA.dir.x * pushDir.x + tankA.dir.y * pushDir.y;

					if (moveDirA > 0)
					{
						/*tankA.pos.x -= tankA.dir.x * tankA.speed * 0.5f;
						tankA.pos.y -= tankA.dir.y * tankA.speed * 0.5f;*/
						tankA.pos = tankA.lastPos;
					}

					tankA.pos.x += pushDir.x * pushAmount;
					tankA.pos.y += pushDir.y * pushAmount;
				}

				bool tankBMoving = (tankB.pos.x != tankB.lastPos.x || tankB.pos.y != tankB.pos.y);

				if (tankB.moved)
				{
					float moveDirB = tankB.dir.x * pushDir.x + tankB.dir.y * pushDir.y;

					if (moveDirB > 0)
					{
						/*tankB.pos.x -= tankB.dir.x * tankB.speed * 0.5f;
						tankB.pos.y -= tankB.dir.y * tankB.speed * 0.5f;*/
						tankB.pos = tankB.lastPos;
					}

					tankB.pos.x -= pushDir.x * pushAmount;
					tankB.pos.y -= pushDir.y * pushAmount;
				}

				tankA.lastPos = tankA.pos;
				tankB.lastPos = tankB.pos;
			}
		}
	}
}

void TankBrawl::TankWallCollision(Tank& tank)
{
	Rectangle hitBox = tank.hitBox;

	Image mapImage = LoadImageFromTexture(maps[playedGames]);

	float angleInRadius = tank.angle * DEG2RAD;

	float frontX = tank.pos.x + cos(angleInRadius) * hitBox.width / 2;
	float frontY = tank.pos.y + sin(angleInRadius) * hitBox.height / 2;

	float backX = tank.pos.x - cos(angleInRadius) * hitBox.width / 2;
	float backY = tank.pos.y - sin(angleInRadius) * hitBox.height / 2;

	Color frontColor = GetImageColor(mapImage, static_cast<int>(frontX), static_cast<int>(frontY));
	Color backColor = GetImageColor(mapImage, static_cast<int>(backX), static_cast<int>(backY));

	/*DrawPixel(frontX, frontY, RED);
	DrawPixel(backX, backY, BLUE);*/

	tank.color = frontColor;

	/*char frontColorText[50];
	sprintf(frontColorText, "Front Color: R:%d G:%d B:%d", frontColor.r, frontColor.g, frontColor.b);
	DrawText(frontColorText, 30, 30, 25, WHITE);

	char backColorText[50];
	sprintf(backColorText, "Back Color: R:%d G:%d B:%d", backColor.r, backColor.g, backColor.b);
	DrawText(backColorText, 30, 60, 25, WHITE);*/

	if ((frontColor.r == 61 && frontColor.g == 61 && frontColor.b == 61) ||
		backColor.r == 61 && backColor.g == 61 && backColor.b == 61)
	{
		tank.pos = tank.lastPos;
	}

	UnloadImage(mapImage);
}

void TankBrawl::DrawCircleFromMousePosition()
{
	Vector2 mousePos = GetMousePosition();

	RenderTexture2D captureTexture = LoadRenderTexture(screenWidth, screenHeight);

	BeginTextureMode(captureTexture);
	ClearBackground(BLACK);

	DrawMap();

	Rectangle testRec = { 500, 200, 100, 100 };
	Color testColor = BLUE;
	DrawRectangleRec(testRec, testColor);

	EndTextureMode();

	if (mousePos.x >= 0 && mousePos.x < screenWidth && mousePos.y >= 0 && mousePos.y < screenHeight)
	{
		Image captureImage = LoadImageFromTexture(captureTexture.texture);
		Color pixelColor = GetImageColor(captureImage, static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));

		DrawCircle(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y), 20, pixelColor);

		UnloadImage(captureImage);
	}

	UnloadRenderTexture(captureTexture);

	Image mapImage = LoadImageFromTexture(maps[playedGames]);

	int mapWidth = mapImage.width;
	int mapHeight = mapImage.height;

	/*if (mousePos.x >= 35 && mousePos.x < mapImage.width && mousePos.y >= 10 && mousePos.y < mapImage.height)
	{
		Color pixelColor = GetImageColor(mapImage, static_cast<int>(mousePos.x), static_cast<int>(mousePos.y));

		DrawCircle(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y), 20, pixelColor);
	}*/

	/*if (mousePos.x >= testRec.x && mousePos.x < testRec.x + testRec.width &&
		mousePos.y >= testRec.y && mousePos.y < testRec.y + testRec.height)
	{
		Color pixelColor = BLUE;

		DrawCircle(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y), 20, pixelColor);
	}*/

	UnloadImage(mapImage);
}

void TankBrawl::TankWallCollisions(Tank& tank)
{
	RenderTexture2D captureTexture = LoadRenderTexture(screenWidth, screenHeight);

	BeginTextureMode(captureTexture);
	//ClearBackground(BLACK);

	//DrawMap();

	/*for (const auto& other : tanks)
	{
		if (other.id != tank.id)
		{
			DrawTexturePro(other.sprite,
				Rectangle{ 0.0f, 0.0f, (float)other.sprite.width, (float)other.sprite.height },
				Rectangle{ other.pos.x, other.pos.y, (float)other.sprite.width, (float)other.sprite.height },
				Vector2{ other.sprite.width / 2.0f, other.sprite.height / 2.0f },
				other.angle,
				other.color
			);
		}
	}*/

	EndTextureMode();

	float angleInRadius = tank.angle * DEG2RAD;
	float frontX = tank.pos.x + cos(angleInRadius) * tank.hitBox.width / 2;
	float frontY = tank.pos.y + sin(angleInRadius) * tank.hitBox.height / 2;

	if (frontX >= 0 && frontX < screenWidth && frontX >= 0 && frontY < screenHeight)
	{
		Image captureImage = LoadImageFromTexture(captureTexture.texture);
		Color pixelColor = GetImageColor(captureImage, static_cast<int>(frontX), static_cast<int>(frontY));

		Color wallColor = Color{ 61,61,61 };

		if (pixelColor.r == wallColor.r && pixelColor.g == wallColor.g && pixelColor.b == wallColor.b)
		{
			tank.pos = tank.lastPos;
		}

		UnloadImage(captureImage);
	}

	UnloadRenderTexture(captureTexture);
}

void TankBrawl::RunGame()
{
	//SpawnTanks();

	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_ESCAPE))
		{
			//return;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		DrawMap();

		HandleTanks();

		HandleCollisions();

		//DrawCircleFromMousePosition();

		EndDrawing();
	}

	UnloadTextures();
	CloseWindow();
}