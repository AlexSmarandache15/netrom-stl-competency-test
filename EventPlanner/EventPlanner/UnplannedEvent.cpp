#include "UnplannedEvent.h"

UnplannedEvent::UnplannedEvent(const std::string& _name, int _duration, int _priority)
{
	this->name = _name;
	this->duration = _duration;
	this->priority = _priority;
}

const int& UnplannedEvent::getDuration() const
{
	return this->duration;
}

const int& UnplannedEvent::getPriority() const
{
	return this->priority;
}

const std::string& UnplannedEvent::getName() const
{
	return this->name;
}

std::istream& operator>>(std::istream& in, UnplannedEvent& x)
{
	in >> x.name;
	in >> x.duration;
	in >> x.priority;
	return in;
}

bool operator>(const UnplannedEvent& x, const UnplannedEvent& y)
{
	return (x.priority > y.priority) || ((x.priority == y.priority) && (x.duration < y.duration));
}

bool operator<(const UnplannedEvent& x, const UnplannedEvent& y)
{
	return (x.priority < y.priority) || ((x.priority == y.priority) && (x.duration > y.duration));
}