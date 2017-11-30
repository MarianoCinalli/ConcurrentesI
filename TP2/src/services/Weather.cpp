#include "services/Weather.h"


Weather::Weather(int temperature, int pressure, int humidity) {
    this->temperature = temperature;
    this->pressure = pressure;
    this->humidity = humidity;
};

Weather::Weather() {
    this->temperature = -1;
    this->pressure = -1;
    this->humidity = -1;
};

Weather::~Weather() {
};

int Weather::getTemperature() {
    return this->temperature;
};

int Weather::getPressure() {
    return this->pressure;
};

int Weather::getHumidity() {
    return this->humidity;
};

void Weather::setTemperature(int temperature) {
    this->temperature = temperature;
};

void Weather::setPressure(int pressure) {
    this->pressure = pressure;
};

void Weather::setHumidity(int humidity) {
    this->humidity = humidity;
};

std::string Weather::logMemberVariables() {
    return " Temperatura: " + std::to_string(this->temperature) +
        " - Presion: " + std::to_string(this->pressure) +
        " - Humedad: " + std::to_string(this->humidity);
};
