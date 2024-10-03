#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "entity.h"

//Create a world
const int WORLD_WIDTH = 20; // 20 * TILE_WIDTH
const int WORLD_HEIGHT = 20; // 20 * TILE_HEIGHT

int TILE_WIDTH = 32;
int TILE_HEIGHT = 32;
int TILE_TYPE = 0;

//Draws a single tile
void DrawTile(int pos_x, int pos_y, int texture_index_x, int texture_index_y);

//Load a level
void loadLevel(std::string fileName);

//Textures
#define MAX_TEXTURES 2
enum texture_asset {
	TEXTURE_TILEMAP = 0,
    TEXTURE_BACKGROUND
};

Texture2D textures[MAX_TEXTURES];
Texture2D background;
Texture2D title;

//World type
enum tile_type {
	//Top of the tile
	TILE_TYPE_TOPLEFT,			// x = 1, y = 0 (0
	TILE_TYPE_UP,				// x = 2, y = 0 (1
	TILE_TYPE_TOPRIGHT,			// x = 3, y = 0 (2
	//Middle of the tile
	TILE_TYPE_RIGHT,			// x = 1, y = 1 (3
	TILE_TYPE_MIDDLE,			// x = 2, y = 1 (4
	TILE_TYPE_LEFT,				// x = 3, y = 1 (5
	//Bottom of the tile
	TILE_TYPE_BOTTOMRIGHT,		// x = 1, y = 2 (6
	TILE_TYPE_BOTTOM,			// x = 2, y = 2 (7
	TILE_TYPE_BOTTOMLEFT,		// x = 3, y = 2 (8
	TILE_TYPE_AIR				// x = 0, y = 0 (9
};


struct sTile {
	int x;
	int y;
	tile_type type;
};

//Level """design"""
int level = 1;

//For trying to load a level
bool failedtoLoad = false;

sEntity cheese;

void RenderWorld();

sTile world[WORLD_WIDTH][WORLD_HEIGHT];

//Init var
std::vector<int> tileMap;

void DrawTile(int pos_x, int pos_y, int texture_index_x, int texture_index_y) {
    Rectangle source = { (float)TILE_WIDTH * texture_index_x, (float)TILE_HEIGHT * texture_index_y, (float)TILE_WIDTH, (float)TILE_HEIGHT };
    Rectangle dest = { (float)pos_x,(float)pos_y , (float)TILE_WIDTH, (float)TILE_HEIGHT };
    Vector2 origin = { 0,0 };
    DrawTexturePro(textures[TEXTURE_TILEMAP], source, dest, origin, 0.0f, WHITE);
}

#pragma region Load a Level
void loadLevel(std::string fileName) {

    //Open file
    std::ifstream file(fileName + ".txt");
    if (!file) {
        std::cout << fileName << " failed to load." << std::endl;
        failedtoLoad = true;
        return;
    }

    //Parse the file
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // Replace newlines with spaces
    std::string remNewLines;
    for (char c : content) {
        if (c == '\n') {
            remNewLines += ' ';
        }
        else {
            remNewLines += c;
        }
    }

    //Split by spaces
    std::istringstream iss(remNewLines);
    std::string token;
    //Iterate everytime
    while (iss >> token) {
        int m = std::stoi(token); //Convert to an integer
        tileMap.push_back(m);
    }
    if (tileMap.size() > static_cast<size_t>(WORLD_WIDTH * WORLD_HEIGHT)) {
        tileMap.resize(WORLD_WIDTH * WORLD_HEIGHT);
    }

    //Finally populate the world based 
    //on the tile set from the file...
    int k = 0;
    try {
        for (int i = 0; i < WORLD_WIDTH; i++) {
            for (int j = 0; j < WORLD_HEIGHT; j++) {
                //Setting the tiles to the approriate tile lol
                world[i][j] = sTile{ i, j, static_cast<tile_type>(tileMap.at(k)) };
                k++;
            }
        }
    }
    //Stop the game from crashing by catching the out of range error
    catch (const std::out_of_range& e) {
        std::cout << fileName << " failed to load. Reason: " << e.what() << std::endl;
        failedtoLoad = true;
        std::cout << "You have missing numbers. If you know what you are doing, you can safely close this. Otherwise..." << std::endl;
        //Print the nines so it can be copied with your terminal
            for (int i = 0; i < WORLD_WIDTH; i++) {
                for (int j = 0; j < WORLD_HEIGHT; j++) {
                    //Setting the tiles to the approriate tile lol
                    std::cout << "9 ";
                }
                std::cout << std::endl;
            }
        std::cout << "If you don't know what you are doing, please copy the nines from the logs and put it as a text file so you can build your own level." << std::endl;
        //Cleanup
        tileMap.clear();
        k = 0;
        return;
    }


    //Cleanup
    tileMap.clear();
    k = 0;
    std::cout << fileName << " loaded sucessfully." << std::endl;
}

void RenderWorld() {
    //Render a tile
    sTile tile;
    int texture_index_x = 0;
    int texture_index_y = 0;
    //Iterate through both world width and height
    for (int i = 0; i < WORLD_WIDTH; i++) {
        for (int j = 0; j < WORLD_HEIGHT; j++) {
            tile = world[i][j];

#pragma region Tile types
            switch (tile.type) {
                //TOP OF THE TILESET
            case TILE_TYPE_TOPLEFT:
                texture_index_x = 1;
                texture_index_y = 0;
                break;
            case TILE_TYPE_UP:
                texture_index_x = 2;
                texture_index_y = 0;
                break;
            case TILE_TYPE_TOPRIGHT:
                texture_index_x = 3;
                texture_index_y = 0;
                break;

                //MIDDLE OF THE TILESET
            case TILE_TYPE_RIGHT:
                texture_index_x = 1;
                texture_index_y = 1;
                break;
            case TILE_TYPE_MIDDLE:
                texture_index_x = 2;
                texture_index_y = 1;
                break;
            case TILE_TYPE_LEFT:
                texture_index_x = 3;
                texture_index_y = 1;
                break;

                //BOTTOM OF THE TILESET
            case TILE_TYPE_BOTTOMRIGHT:
                texture_index_x = 1;
                texture_index_y = 2;
                break;
            case TILE_TYPE_BOTTOM:
                texture_index_x = 2;
                texture_index_y = 2;
                break;
            case TILE_TYPE_BOTTOMLEFT:
                texture_index_x = 3;
                texture_index_y = 2;
                break;
                // AIR
            default:
                texture_index_x = 0;
                texture_index_y = 0;
                break;
            }
#pragma endregion

            //Now draw them
            DrawTile(tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, texture_index_x, texture_index_y);
        }
    }

    //Draw cheese
    DrawTile(cheese.x, cheese.y, 3, 3);
}

//Check if this entity is on a tile or not
tile_type GetTileAtPosition(int x, int y) {
    int tile_x = x / TILE_WIDTH;
    int tile_y = y / TILE_HEIGHT;

    //Check if we're in bounds
    if (tile_x >= 0 && tile_x < WORLD_WIDTH && tile_y >= 0 && tile_y < WORLD_HEIGHT) {

        return world[tile_x][tile_y].type;
    }

    //Return out of bounds
    return TILE_TYPE_AIR;
}

//Check if entity (or player) is touching the air
bool isTouchingAir(const sEntity& entity) {
    tile_type tileType;
    // Check the four surrounding tiles
    switch (entity.dir) {
        case NORTH:
            tileType = GetTileAtPosition(entity.x, entity.y - TILE_HEIGHT); // Check the tile above
            break;
        case EAST:
            tileType = GetTileAtPosition(entity.x + TILE_WIDTH, entity.y); // Check the tile to the right
            break;
        case SOUTH:
            tileType = GetTileAtPosition(entity.x, entity.y + TILE_HEIGHT); // Check the tile below
            break;
        case WEST:
            tileType = GetTileAtPosition(entity.x - TILE_WIDTH, entity.y); // Check the tile to the left
            break;
        default:
            return false; // Invalid direction; consider handling this case as needed
        }

    // Return true if any of the surrounding tiles are AIR
    return (tileType == TILE_TYPE_AIR);
}

#pragma endregion