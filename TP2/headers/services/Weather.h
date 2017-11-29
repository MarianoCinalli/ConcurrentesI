#ifndef WEATHER_H
#define WEATHER_H

#include <string>
#include "interfaces/Loggable.h"

class Weather : public Loggable {

	private:
		int temperature;
		int pressure;
		int humidity;

	public:
		Weather(int temperature, int pressure, int humidity);
		Weather();
		~Weather();
		int getTemperature();
		int getPressure();
		int getHumidity();
		void setTemperature(int temperature);
		void setPressure(int pressure);
		void setHumidity(int humidity);
		std::string logMemberVariables();
};

#endif // WEATHER_H
