#pragma once
#include "./common.h"

class Box : Eventful<ElementEvent>{
	private:
		Vector2 position = {0, 0};
		Vector2 size = {0, 0};
		RGBA color = {0, 0, 0, 255};
		WindowContext* ctx;
	
	bool isMouseInside(Vector2 mousePos) {
		return (mousePos.x >= this->position.x && mousePos.x <= this->position.x + this->size.x) && (mousePos.y >= this->position.y && mousePos.y <= this->position.y + this->size.y);
	}

	bool checkMouseClick();
	
	public:
		Box();
		Box(Vector2 pos, Vector2 size, RGBA color);
		~Box();
		void draw(WindowContext* ctx);
};
