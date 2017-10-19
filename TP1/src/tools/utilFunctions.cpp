#include "tools/utilFunctions.h"

// Calcula un numero al azar entre min y max.
// Necesita que se ejecute "srand(time(NULL));" previamente.
int getRandomBetween(int min, int max) {
   return min + (rand() % static_cast<int>(max - min + 1));
}

initialParameter* loadInitialParameters(std::string path) {
    FILE* file = fopen(path.c_str(), "r");
    char readBuffer[65536];
    rapidjson::FileReadStream stream(file, readBuffer, sizeof(readBuffer));
    fclose(file);
    rapidjson::Document document;
    struct initialParameter* initialParameter = new struct initialParameter;
    if (document.ParseStream(stream).HasParseError()) {
        log(UTIL_FUNCTIONS_NAME + " : El archivo " + path + " no se ha parsiado correctamente", ERROR);
        log(UTIL_FUNCTIONS_NAME + " : Se toman datos default", INFORMATION);
        initialParameter->rows = F_DEFAULT;
        initialParameter->columns = C_DEFAULT;
        initialParameter->maxPlayer = MAX_PLAYERS_DEFAULT;
        initialParameter->maxGamesByPlayer = MAX_GAMES_PER_PLAYER_DEFAULT;
    } else {
        log(UTIL_FUNCTIONS_NAME + " : El archivo " + path + " se ha parsiado correctamente.", INFORMATION); 
        initialParameter->rows = document["rows"].GetInt();
        initialParameter->columns = document["columns"].GetInt();
        initialParameter->maxPlayer = document["maxPlayer"].GetInt();
        initialParameter->maxGamesByPlayer = document["maxGamesByPlayer"].GetInt();
    }
    return initialParameter;
}