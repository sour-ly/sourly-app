#include <iostream>
#include "log/log.h"
#include "renderer/renderer.hpp"

int main() {
	Log::log("main", "Hello, World!\n");
	WindowContext window = {};
	draw_window(&window);
	init_renderer(&window);

	return 0;
}
