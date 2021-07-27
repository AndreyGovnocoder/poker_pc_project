#include "Game.h"

Game::Game()
{
	_bank = 0;
	_round = -1;
	_bet = 0;
	_turn = 0;
}

void Game::incrTurn(size_t playersCount)
{
	++_turn;
	if (_turn > playersCount)
		_turn = 1;
}
