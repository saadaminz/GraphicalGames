#pragma once

#include <vector>
#include <filesystem>
#include <fstream>

#include "Raylib/raylib.h"

class Tank
{
public:
	Texture2D sprite;
	Vector2 pos, lastPos, vel, dir;
	int id, victories;
	int frameHeight;
	Color color;
	float angle, speed;
	Rectangle hitBox;
	bool moved;

	Tank(int _id)
		: id(_id)
		, victories(0)
		, vel{3, 3}
		, angle(160.0f)
		, speed(0.45f)
		, frameHeight(0)
	{}
};

class TankBrawl
{
public:
	int screenWidth, screenHeight;

	int playedGames;

	int playersCount;

	std::vector<Texture2D> maps;
	//std::vector<Texture2D> tanksTex; // remove, load at Tank class directly
	float mapScale;
	float tankScale;

	std::vector<Tank> tanks;
	std::vector<Color> tankColors;

	Texture2D menu;

	void LoadTextures();

	void UnloadTextures();

	void HandleTanks(); // later have temp-move before actual movement

	void HandleCollisions();

	void TankWallCollision(Tank& tank);

	void MainMenu();

	void DrawMap();

	void RunGame();

	// Tests

	void PrintImageColors(Texture2D& texture);

	Color DecideColor(const std::string& fileName);

	void DrawCircleFromMousePosition();

	void TankWallCollisions(Tank& tank);

	TankBrawl(const int _screenWidth, const int _screenHeight)
		: screenWidth(_screenWidth)
		, screenHeight(_screenHeight)
		, mapScale(11.7f)
		, tankScale(2.11f)
		, playedGames(0)
		, playersCount(0)
	{
		InitWindow(screenWidth, screenHeight, "TankBrawl");
		SetTargetFPS(60);
	}

	~TankBrawl()
	{}
};