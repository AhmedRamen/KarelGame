#pragma once
#include "entity.h"

//Include the player in here
sEntity player;

//Allows the mouse to move forwards
void MoveForward();

//Turns the mouse around from current direction
void TurnAround();

//Turns the mouse to the left
void TurnLeft();

//Turns the mouse to the right
void TurnRight();

//What actually changes the direction based on tile
void drawPlayerDirection();

//If we can move at all
bool canmove = false;

//Were actually moving
bool moving = false;


void CheckCanMove() {
    if (canmove) {
        return;
    }
    else {
        return;
    }
}


//Set the "sprite" accordingly to the players direction
void drawPlayerDirection() {
    switch (player.dir) {
    case NORTH:
        DrawTile(player.x, player.y, 4, 2);
        break;
    case EAST:
        DrawTile(player.x, player.y, 5, 2);
        break;
    case SOUTH:
        DrawTile(player.x, player.y, 5, 3);
        break;
    case WEST:
        DrawTile(player.x, player.y, 4, 3);
        break;
    }
}

void MoveForward() {
    CheckCanMove();
    int x = player.x;
    int y = player.y;
    switch (player.dir) {
    case NORTH:
        //"World" Boundary hack
        if (y < 0 + TILE_HEIGHT || isTouchingAir(player)) {
            std::cout << "Cannot move, blocked from this direction" << std::endl;
            canmove = false;
            return;
        }
        moving = true;
        y -= 1 * TILE_HEIGHT;
        //Check if we're on the air
        break;
    case EAST:
        //"World" Boundary hack
        if (x > 608 - TILE_WIDTH || isTouchingAir(player)) {
            std::cout << "Cannot move, blocked from this direction" << std::endl;
            canmove = false;
            return;
        }
        moving = true;
        x += 1 * TILE_WIDTH;
        //Check if we're on the air
        break;
    case SOUTH:
        //"World" Boundary hack
        if (y > 608 - TILE_HEIGHT || isTouchingAir(player)) {
            std::cout << "Cannot move, blocked from this direction" << std::endl;
            canmove = false;
            return;
        }
        moving = true;
        y += 1 * TILE_HEIGHT;
        //Check if we're on the air
        break;
    case WEST:
        //"World" Boundary hack
        if (x < 0 + TILE_WIDTH || isTouchingAir(player)) {
            std::cout << "Cannot move, blocked from this direction" << std::endl;
            canmove = false;
            return;
        }
        moving = true;
        x -= 1 * TILE_WIDTH;
        //Check if we're on the air
        break;
    }

    sound_manager::PlaySoundFromList(SOUND_MOUSE_TOUCH);
    player.x = x;
    player.y = y;
    moving = false;
}

void TurnLeft() {
    switch (player.dir) {
    case NORTH:
        player.dir = WEST;
        break;
    case EAST:
        player.dir = NORTH;
        break;
    case SOUTH:
        player.dir = EAST;
        break;
    case WEST:
        player.dir = SOUTH;
        break;
    }
    sound_manager::PlaySoundFromList(SOUND_MOUSE_TOUCH);
}

void TurnRight() {
    switch (player.dir) {
    case NORTH:
        player.dir = EAST;
        break;
    case EAST:
        player.dir = SOUTH;
        break;
    case SOUTH:
        player.dir = WEST;
        break;
    case WEST:
        player.dir = NORTH;
        break;
    }
    sound_manager::PlaySoundFromList(SOUND_MOUSE_TOUCH);
}

void TurnAround() {
    switch (player.dir) {
    case NORTH:
        player.dir = SOUTH;
        break;
    case EAST:
        player.dir = WEST;
        break;
    case SOUTH:
        player.dir = NORTH;
        break;
    case WEST:
        player.dir = EAST;
        break;
    }
    sound_manager::PlaySoundFromList(SOUND_MOUSE_TOUCH);
}
