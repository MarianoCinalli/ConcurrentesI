#ifndef WEATHERTABLE_H
#define WEATHERTABLE_H

#include <string>
#include <fstream>

#include "tools/logger.h"
#include "tools/Constants.h"
#include "tools/json.hpp"
#include "services/Weather.h"

using json = nlohmann::json;

class WeatherTable {

private:
	json table;

public:
	WeatherTable();
	~WeatherTable();
	Weather* get(std::string city);
	void update(std::string city, Weather* newWeather);
	void erase(std::string city);
	void saveChanges();
};

#endif // WEATHERTABLE_H
