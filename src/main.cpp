#include <iostream>
#include "log/log.h"
#include "renderer/renderer.hpp"
#include "model/common.h"

int main() {
	Log::log("main", "Hello, World!\n");
	Skill skill = Skill("Programming", "The art of programming", 0);
	skill.on("goalAdded", [](Event<SkillEvent> event) {
		Log::log("main", "Goal added to skill\n");
	});
	Goal goal = Goal("Learn C++", "Learn the C++ programming language", "Hours", 100);
	skill.addGoal(goal);
	

	return 0;
}
