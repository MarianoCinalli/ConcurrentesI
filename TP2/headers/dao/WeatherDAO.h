#include "tools/Messages.h"


#ifndef WEATHERDAO_H
#define WEATHERDAO_H

class WeatherDAO {
public:
    WeatherDAO();
    ~WeatherDAO();
    weatherDAO get(int cityID);
    int update(weatherDAO city);
    int create(weatherDAO city);
    int remove(int cityID);
};
#endif // WEATHERDAO_H