#pragma once
#include <string>
#include <nlohmann/json.hpp>

#ifndef EVENTS_H
#define EVENTS_H
using namespace nlohmann;


template <typename T>
struct Event {
	std::string name;
	T* arg;	
};

template <typename T>
class Eventful {
	//keys are event names, values are a vector of functions
	std::map<std::string, std::vector<std::function<void(Event<T>)>>> eventMap;
protected:

	void emit(std::string eventName, T* arg) {
		Event<T> event = {eventName, arg};
		if (eventMap.find(eventName) != eventMap.end()) {
			for (auto& func : eventMap[eventName]) {
				func(event);
			}
		}
	}

public:
	Eventful() {
		eventMap = std::map<std::string, std::vector<std::function<void(Event<T>)>>>();
	}
	~Eventful() {
		eventMap.clear();
	}
	
	unsigned int on(std::string eventName, std::function<void(Event<T>)> func) {
		if (eventMap.find(eventName) == eventMap.end()) {
			eventMap[eventName] = std::vector<std::function<void(Event<T>)>>();
		}
		eventMap[eventName].push_back(func);
		return eventMap[eventName].size() - 1;
	}

	unsigned int off(unsigned int args) {
		return 0;
	}	

};
#endif
