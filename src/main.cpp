#include <iostream>
#include "log/log.h"
#include "renderer/renderer.hpp"

int main() {
	Log::log("main", "Hello, World!\n");
	WindowContext window = {};
	WindowSettings settings = {"Sourly", 1280, 720, {255, 255, 255, 255}};
	init_window(&settings, &window);
	init_renderer(&window);

	return 0;
}
