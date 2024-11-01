#include "box.h"

Box::Box()  {
	this->position = Vector2(0, 0);
	this->size = Vector2(100, 100);
	this->color = {0, 0, 0, 255};
}

Box::Box(Vector2 pos, Vector2 size, RGBA color) {
	this->position = pos;
	this->size = size;
	this->color = color;
}

Box::~Box() {
	//default destructor
}


void Box::draw(WindowContext *ctx) {
	this->ctx = ctx;
	draw_rect(ctx, this->position, this->size, this->color);
}
