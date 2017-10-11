#ifndef PLAYER_H
#define PLAYER_H

class Player {

private:
	int points;

public:
	Player();
	~Player();
	void addPoints(int points);
	int getPoints();
};

#endif // PLAYER_H
