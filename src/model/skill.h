#pragma once
#include "../events/events.h"
#include <ctime>
#include "goal.h"
#include "../log/log.h"

typedef struct Skill Skill;
typedef struct SkillEvent SkillEvent;

enum SkillEventType {
	SKILL_CREATED,
	SKILL_UPDATED,
	SKILL_DELETED,
	GOAL_ADDED,
	GOAL_REMOVED,
};

struct SkillEvent {
	//the skill that the event is associated with
	Skill* skill;
	//the date the event was created
	time_t dateCreated;
	//the type of event
	SkillEventType type;
	//im sure there will be more to this...
	
	SkillEvent(Skill* _skill, time_t _dateCreated, SkillEventType _type) : skill(_skill), dateCreated(_dateCreated), type(_type) {
		this->skill = _skill;
		this->dateCreated = _dateCreated;
		this->type = _type;
	}
};

class Skill : public Eventful<SkillEvent>{
	private:
		//id 
		unsigned long id;
		//name of the skill
		const char* name;
		const char* description;
		//oh i really hope a buffer underflow doesn't happen
		unsigned int level;
		//the experience required to level the skill
		double experienceRequired;
		double experienceValue;
		//date the skill was created
		static constexpr float percision = 0.0001;
		//date the skill was completed
		time_t dateCreated;
		//the goals
		std::vector<Goal> goals = {};
		
		inline static double calculateExperienceRequired(unsigned int level) {
			return 50 * level + ((level - 1) * (level - 1));
		}

	public:

		Skill() : id(0), name("Untitled"), description(""), level(0), experienceRequired(0.0), experienceValue(0.0), dateCreated(time(0)) {
			experienceRequired = calculateExperienceRequired(level);
		}

		Skill(const char* _name, const char* _description, unsigned int _level) : id(0), name(_name), description(_description), level(_level) {
			name = _name; 
			description = _description; 
			level = _level; 
			experienceRequired = calculateExperienceRequired(level);
		} 

		~Skill() {
		}		

		//add a goal to the skill
		void addGoal(Goal goal) {
			goals.push_back(goal);
			SkillEvent event = SkillEvent(this, time(0), GOAL_ADDED);
			this->emit("goalAdded", &event);
			Log::log("Skill::addGoal", "Adding goal to skill\n");
		}

		//remove a goal from the skill
		void removeGoal(Goal goal) {
			for (int i = 0; i < goals.size(); i++) {
				if (goals[i].getId() == goal.getId()) {
					goals.erase(goals.begin() + i);
					Log::log("Skill::removeGoal", "Removing goal from skill\n");
					return;
				}
			}
		}

		void removeGoal(unsigned long id) {
			for (int i = 0; i < goals.size(); i++) {
				if (goals[i].getId() == id) {
					goals.erase(goals.begin() + i);
					Log::log("Skill::removeGoal", "Removing goal from skill\n");
					return;
				}
			}
		}

		Goal* getGoal(unsigned long id) {
			for (int i = 0; i < goals.size(); i++) {
				if (goals[i].getId() == id) {
					return &goals[i];
				}
			}
			return NULL;
		}
};
