#include "../tools/Constants.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player{

private:
	// status valores adoctados de PCStatus en Constans.h 
	PCStatus status;
	int id;
	int gamesPlayed;

public:
	Player(int id, PCStatus status);
	Player(int id);
	~Player();

	//obtiene la cantida de juegos jugados por el jugador
	const int getGamesPlayed(){return this->gamesPlayed;}

	//obtiene id del jugador
	const int getId(){return this->id;}

	//incrementa en 1 la cantidad de juegos jugados por el jugador
	void addGamePlayed(){this->gamesPlayed++;}

	//pone al jugador en estado libre permaneciendo dentro del predio
	void endGame() {this->status = freeS;}

	//pone al jugador en estado desabilitado sacandolo fuera del predio
	void disable() {this->status = disableS;}

	//pone al jugador en estado ocupado, jugando un match en el predio
	void playGame() {this->status = busyS;}

	//pone al jugador en estado completado,no vuelve a jugar un partido
	void gameOver() {this->status = completedS;}

	//devuelve true si el jugador esta ocupado 
	bool isBusy() {return this->status == busyS;}

	//devuelve true si el jugador esta libre 
	bool isFree() {return this->status == freeS;}

	//devuelve true si el jugador esta disponible 
	bool isDisable() {return this->status == disableS;}

};

#endif // PLAYER_H
