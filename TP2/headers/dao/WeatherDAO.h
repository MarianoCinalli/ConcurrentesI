#include "tools/Messages.h"


#ifndef WEATHERDAO_H
#define WEATHERDAO_H

class WeatherDAO {
public:
    WeatherDAO();
    ~WeatherDAO();
    weather get(int cityId);
    int update(weather weather);
    int create(weather weather);
    int remove(int cityID);
};
#endif // WEATHERDAO_H