#pragma once
class Game
{
public:
	Game();
	~Game() = default;

	void set_bank(int value) { _bank = value; };
	void set_round(int round) { _round = round; };
	void set_bet(int bet) { _bet = bet; };
	void set_turn(int turn) { _turn = turn; };
	void set_raised(bool value) { _raised = value; };

	void addToBank(int value) { _bank += value; };
	void addToBet(int value) { _bet += value; };
	void incrRound() { ++_round; };
	void incrTurn(size_t playersCount);

	const int get_bank() { return _bank; };
	const int get_round() { return _round; };
	const int get_bet() { return _bet; };
	const int get_turn() { return _turn; };
	bool is_raised() { return _raised; };

private:
	int _bank;
	int _round;
	int _bet;
	int _turn;
	bool _raised;
};

