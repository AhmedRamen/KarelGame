#pragma once
#include <string>
#include "raylib.h"

class Button {
public:
	Button(std::string name, int posX, int posY) {
		this->X = posX;
		this->Y = posY;

		const int length = name.length();
		char* char_array = new char[length + 1];

		strcpy_s(char_array, 16 , name.c_str());

		this->name = char_array;
	}

	void Render(void (*_setup)()) {
		DrawRectangle(X + 25, Y, 250, 75, isHovered(GetMouseX(), GetMouseY()) ? RED : BLACK);

		DrawRectangle(X + 30, Y + 5, 240, 65, (isHovered(GetMouseX(), GetMouseY()) ? BLACK : WHITE));

		DrawText(name, X + (150 - MeasureText(name, 30) / 2), Y + 25, 30, (isHovered(GetMouseX(), GetMouseY()) ? RED : BLACK));

		if (isHovered(GetMouseX(), GetMouseY())) {
			if (IsMouseButtonPressed(0))
				onClick(_setup);
		}
		hovered = isHovered(GetMouseX(), GetMouseY());
	}

private:
	bool hovered;
	int X, Y;
	char* name;
	void onClick(void (*onclick)()) { onclick(); };

	int isHovered(int posX, int posY)
	{
		return (posX < X + 250 && posX > X && posY < Y + 75 && posY > Y);
	};
};