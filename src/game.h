#pragma once
#include <string>
#include "raylib.h"
#include "button.h"

//Turns WindowShouldClose() into a boolean
bool exitWindow = false;


//Initalises everything and boots the game up
void GameStartup();

//Loads a level from a text file (Really broken)
void loadFile();

//Updates one frame of the game
void GameUpdate();

//Renders one frame of the game
void GameRender();

//Cleans up everything and shuts down the game
void GameShutdown();

//Run the lua script that moves the Maus (Has to be inside workers or it'll not execute)
void StartMaus();

void StartGame();

//Sets both the player and cheese location
void StartLevel();

//Moves on to the next level
void StartNextLevel();

//Screen Resolution
const int screenWidth = 800;
const int screenHeight = 600;

//Camera
Camera2D camera;

//Check if the text file is empty
bool empty = false;

//Check if the maus started moving
bool started = false;

//Check if a script error happened
bool failed = false;

//Check if we touched the cheese
bool win = false;

//How many attempts we took for a level
int attempts = 0;

//Hard coded maximum attempts
int max_attempts = 0;

//How many attempts we took in total
int total_attempts = 0;

//How much score do we have
int score = 0;

//Buttons
Button Restart("Restart level", screenWidth / 2 + 120, screenHeight / 2 + 220);
Button Execute("Execute", screenWidth / 2 + 120, screenHeight / 2 + 220);
Button OpenScript("Open Script", screenWidth / 2 + 120, screenHeight / 2 + 140);
Button StopExec("Stop Executing", screenWidth / 2 + 120, screenHeight / 2 + 220);
Button NextLevel("Next level", screenWidth / 2 + 120, screenHeight / 2 + 220);
Button StartGameButton("Play Game", 3, screenHeight / 2 + 160);
Button Highscores("High Scores", 255, screenHeight / 2 + 160);
Button ReturnMenu("Return", 3, screenHeight / 2 + 200);
Button QuitGame("Exit Game", screenWidth / 2 + 110, screenHeight / 2 + 160);


//Background scrolling speed
float speedX = 1000.0f;
float speedY = 1024.0f;


//Boring Scene Switcher
enum GameScreen {
	TITLE = 0,
	MENU, //actually a "high" score menu
	GAME,
	ENDING
};

GameScreen currentScreen = TITLE;
GameScreen previous;

//Cinematic suspense
int frameCount = 0;
bool skipped = false;

//App Icon
Image icon;