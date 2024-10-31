#include <iostream>
#include "log/log.h"
#include "renderer/renderer.hpp"
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

	renderer.on("onFrame", [skill](Event<WindowContext> event) {
			TextSettings txt = TextSettings{((Skill)skill).getGoal(0)->getName(), 24, {0, 0, 0, 255}, {100, 25}};
			draw_text_sdl(event.arg, Vector2(0, 0), &txt);
	});

	//start it!
	renderer.start();
	

	return 0;
}
