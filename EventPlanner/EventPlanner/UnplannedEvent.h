#pragma once
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <memory.h>

class UnplannedEvent
{
private:
	std::string name;
	int duration;
	int priority;
public:
	UnplannedEvent(const std::string & = "", int = 0, int = 0);
	const std::string& getName() const;
	const int& getDuration() const;
	const int& getPriority() const;
	friend std::istream& operator>>(std::istream&, UnplannedEvent&);
	friend bool operator>(const UnplannedEvent&, const UnplannedEvent&);
	friend bool operator<(const UnplannedEvent&, const UnplannedEvent&);
};
