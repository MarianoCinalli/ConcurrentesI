#ifndef UTILFUNCTIONS_H
#define UTILFUNCTIONS_H

#include "Constants.h"
#include <string>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <functional>

#include "tools/logger.h"
#include "../rapidjson/document.h"
#include "../rapidjson/filereadstream.h"

const std::string UTIL_FUNCTIONS_NAME = "UtilFunctions";

int getRandomBetween(int min, int max);
initialParameter* loadInitialParameters(std::string path);

#endif
