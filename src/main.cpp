#include <iostream>
#include <string>
#include "log/log.h"
#include "renderer/renderer.hpp"
#include "renderer/view/box.h"
#include "model/common.h"

int main() {
	Log::log("main", "Hello, World!\n");
	Renderer renderer = Renderer();
	
	Skill skill = Skill("Programming", "The art of programming", 0);
	skill.on("goalAdded", [](Event<SkillEvent> event) {
		Log::log("main", "Goal added to skill\n");
	});
	Goal goal = Goal("Learn C++", "Learn the C++ programming language", "Hours", 100);
	skill.addGoal(goal);

	Box box = Box({renderer.getWidth() / 2 - 50, renderer.getHeight() / 2 - 50}, {100, 100}, {0, 0, 0, 255});

	renderer.on("onFrame", [skill, box](Event<WindowContext> event) {
			WindowContext* ctx = event.arg;

			TextSettings fps = TextSettings{ std::to_string(ctx->currentFramerate).c_str(), 24, {0, 0, 0, 255}, {100, 25}};
			draw_text(event.arg, Vector2(50, 0), &fps);
			((Box)box).draw(event.arg);
	});

	//start it!
	renderer.start();
	

	return 0;
}
