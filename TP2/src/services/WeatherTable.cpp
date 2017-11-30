#include "services/WeatherTable.h"

WeatherTable::WeatherTable() {
    // Cargo las configuraciones default.
    std::ifstream i(WEATHER_DB_FILE);
    this->table = json::parse(i);
};

Weather* WeatherTable::get(std::string city) {
	Weather* weather = new Weather();
    if (this->table.find(city) == this->table.end()) {
        log("WeatherTable: **WARNING** No se encontro la ciudad: " + city , WARNING);
    } else {
        weather->setTemperature(this->table[city]["temperature"]);
        weather->setPressure(this->table[city]["pressure"]);
        weather->setHumidity(this->table[city]["humidity"]);
    }
    return weather;
};

void WeatherTable::update(std::string city, Weather* newWeather) {
    if (this->table.find(city) == this->table.end()) {
        log("WeatherTable: Creando nueva ciudad " + city + " => ", newWeather, INFORMATION);
    } else {
        log("WeatherTable: Actualizando ciudad " + city + " => ", newWeather, INFORMATION);
    }
    this->table[city]["temperature"] = newWeather->getTemperature();
    this->table[city]["pressure"] = newWeather->getPressure();
    this->table[city]["humidity"] = newWeather->getHumidity();
}

void WeatherTable::erase(std::string city) {
    if (this->table.find(city) == this->table.end()) {
        log("WeatherTable: **WARNING** No se encontro la ciudad: " + city , WARNING);
    } else {
        log("WeatherTable: Eliminando " + city , INFORMATION);
        this->table.erase(city);
    }
}

void WeatherTable::saveChanges() {
    // Write prettified JSON to file. With four spaces
    log("WeatherTable: Saving JSON db", INFORMATION);
    std::ofstream o(WEATHER_DB_FILE, std::ios_base::trunc);
    o << std::setw(4) << this->table << std::endl;
    log("WeatherTable: JSON saved!", INFORMATION);
}
