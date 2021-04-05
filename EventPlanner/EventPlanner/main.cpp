/*
* author: Alex Smarandache
*/

#include "UnplannedEvent.h"
#include <limits.h>

bool solveA(const std::vector<std::vector<bool>>&, const UnplannedEvent&);
void readInputFromFile(std::vector<std::vector<bool>>&, std::multiset<UnplannedEvent, std::greater<>>&, std::ifstream&);
std::tuple<std::string, int, int> placeEvent(std::vector<std::vector<bool>>&, const UnplannedEvent&);
void solveB(std::vector<std::vector<bool>>&, std::multiset<UnplannedEvent, std::greater<>>&);

int main(int argc, char* argv[])
{
	auto calendar = std::make_unique<std::vector<std::vector<bool>>>();
	auto unplanned_events = std::make_unique<std::multiset<UnplannedEvent, std::greater<>>>();

	std::string input = "input.txt";
	std::ifstream fin(input);

	readInputFromFile((*calendar), (*unplanned_events), fin);
	fin.close();

	solveB((*calendar), (*unplanned_events));
	
	return 0;
}

void readInputFromFile(std::vector<std::vector<bool>>& calendar, std::multiset<UnplannedEvent, 
				std::greater<>>& unplanned_events, std::ifstream& in) {
	int no_days, no_planned_events, no_unplanned_events;
	in >> no_days;

	calendar.resize(no_days);
	for (auto& it : calendar) {
		it.resize(8);
	}
	
	in >> no_planned_events;
	std::string event;
	int day, start_event, duration;
	
	for (int i = 0; i < no_planned_events; i++) {
		in >> event;
		in >> day;
		--day;
		in >> start_event;
		start_event -= 9;
		in >> duration;
		std::fill(std::begin(calendar[day]) + start_event, std::begin(calendar[day]) + start_event + duration, true);
	}

	in >> no_unplanned_events;
	UnplannedEvent unplanned_event;
	in >> unplanned_event;
	unplanned_events.insert(unplanned_event);
	if (!solveA(calendar, unplanned_event)) {
		std::cout << "The first event cannot be planned\n";
	}
	for (int i = 1; i < no_unplanned_events; i++) {
		in >> unplanned_event;
		unplanned_events.insert(unplanned_event);
	}
}

bool solveA(const std::vector<std::vector<bool>>& calendar, const UnplannedEvent& event) {
	int day_counter = 0;
	for (const auto day : calendar) {
		int start = 0, current = 0;
		day_counter++;
		for (auto it = std::begin(day); it != std::end(day); it++) {
			if (!(*it)) {
				if (!current) {
					start = it - std::begin(day) + 9;
				}
				current++;
			}
			else {
				if (current >= event.getDuration()) {
					std::cout << "a) " << day_counter << " " << start << '\n';
					return true;
				}
				current = 0;
			}
		}
		if (current >= event.getDuration()) {
			std::cout << "a) " << day_counter << " " << start << '\n';
			return true;
		}
	}
	return false;
}

// it determines the most compact possible location of the event, or day 0 if it isn't possible to place the event
std::tuple<std::string, int, int> placeEvent(std::vector<std::vector<bool>>& calendar, const UnplannedEvent& event) {
	int day_counter = 0, start_event = 0, day_event = 0, best_place = INT_MAX;
	std::for_each(std::begin(calendar), std::end(calendar), [&](const auto& day) {
		int start = 0, current = 0;
		day_counter++;
		for (auto it = std::begin(day); it != std::end(day); it++) {
			if (!(*it)) {
				if (!current) {
					start = it - std::begin(day) + 9;
				}
				current++;
			}
			else {
				if (current >= event.getDuration()) {
					if (current < best_place) {
						best_place = current, day_event = day_counter, start_event = start;
					}
				}
				current = 0;
			}
		}
		if (current >= event.getDuration()) {
			if (current < best_place) {
				best_place = current, day_event = day_counter, start_event = start;
			}
		}
	});

	if (day_event) {
		int temp = start_event - 9;
		while (temp < (start_event -9 + event.getDuration())) {
			calendar[day_event - 1][temp] = true;
			temp++;
		}
	}
	return std::make_tuple(event.getName(), day_event, start_event);
}


void solveB(std::vector<std::vector<bool>>& calendar, std::multiset<UnplannedEvent, std::greater<>>& unplanned_events) {
	
	auto compare = [](const std::tuple<std::string, int, int>& a, std::tuple<std::string, int, int>& b) {
		return (std::get<1>(a) > std::get<1>(b))
			   || ((std::get<1>(a) ==  std::get<1>(b)) && (std::get<2>(a) > std::get<2>(b)))
			   || ((std::get<1>(a) == std::get<1>(b)) && (std::get<2>(a) == std::get<2>(b)) && (std::get<0>(a) == std::get<0>(b)));
	};
	
	// tuple -> {name_event, day, start_hour}
	std::priority_queue<std::tuple<std::string, int, int>, std::vector<std::tuple<std::string, int, int>>, 
		decltype(compare)> new_planned_events(compare);

	for (auto&& it : unplanned_events) {
		
		auto event_place = placeEvent(calendar, it);

		if (std::get<1>(event_place)) {
			new_planned_events.push(event_place);
		}
	}

	std::cout << new_planned_events.size() << std::endl;
	while (!new_planned_events.empty()) {
		std::cout << std::get<0>(new_planned_events.top()) << " " << std::get<1>(new_planned_events.top()) << " " <<
			std::get<2>(new_planned_events.top()) << std::endl;
		new_planned_events.pop();
	}
}