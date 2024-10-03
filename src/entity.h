#pragma once

enum Direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

struct sEntity {
	int x;
	int y;
	Direction dir;
};
