#ifndef PLAYERRESULT_H
#define PLAYERRESULT_H

#include "../tools/Constants.h"
#include "../tools/logger.h"
#include <typeinfo>

class PlayerResult{

    const std::string PLAYER_RESULT_NAME = typeid(PlayerResult).name();

    private:
        int id;
        int score;

    public:
        PlayerResult(int id);
        void showScore();
        void addScore(int score);
        ~PlayerResult();

};

#endif // PLAYERRESULT_H
