#pragma once
#include <ctime>
#include "../events/events.h"

typedef struct Goal Goal;
typedef struct GoalEvent GoalEvent;

enum GoalEventType {
	GOAL_CREATED,
	GOAL_UPDATED,
	GOAL_COMPLETED,
	GOAL_DELETED,
};

struct GoalEvent {
	//the goal that the event is associated with
	Goal* goal;
	//the date the event was created
	time_t dateCreated;
	//the type of event
	GoalEventType type;
	//im sure there will be more to this...
};

class Goal : public Eventful<GoalEvent>{
	private:
		//id 
		unsigned long id = 0;
		//name of the goal
		const char* name;
		const char* description;
		//metric of the goal, how we are measuring it
		const char* metric;
		double goalValue;
		double currentValue;
		//reward for completing the goal
		double reward = 25;
		//date the goal was created
		static constexpr float percision = 0.0001;
		//date the goal was created
		time_t dateCreated;
		//date the goal was completed
		time_t dateCompleted;

	public:
		Goal() : id(0), name("Untitled"), description(""), metric("Hours"), goalValue(0.0), currentValue(0.0), dateCreated(time(0)), dateCompleted(0) {
		}
		Goal(const char* _name, const char* _description, const char* _metric, double _goalValue) : id(0), name(_name), description(_description), metric(_metric), goalValue(_goalValue) {
		}
		~Goal() {
		}

		const unsigned long getId() {
			return id;
		}
};
