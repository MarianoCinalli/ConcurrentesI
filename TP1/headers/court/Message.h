#include "tools/logger.h"
#include "tools/Messages.h"
#include <string>

#ifndef MESSAGE_H
#define MESSAGE_H

class Message {

private:
	int firstTeamFirstPlayer;
	int firstTeamSecondPlayer;
	int secondTeamFirstPlayer;
	int secondTeamSecondPlayer;
	int operation;

public:
	Message();
	Message(struct messageMatch message);
	~Message();
	int getFirstTeamFirstPlayer();
	int getFirstTeamSecondPlayer();
	int getSecondTeamFirstPlayer();
	int getSecondTeamSecondPlayer();
	int getOperation();
};

#endif // MESSAGE_H
