#pragma once
#include <qstring.h>
#include <vector>
#include "Card.h"

class Combination
{
public:
	Combination() { _hand.reserve(7); };
	~Combination() = default;

	void set_name(const QString& name) { _name = name; };
	void set_handPower(int power) { _handPower = power; };
	void set_combPower(int combPower) { _combPower = combPower; };
	void addCard(const Card& card) { _hand.push_back(card); };

	const QString& get_name() const { return _name; };
	const std::vector<Card> get_hand() const { return _hand; };
	int get_handPower() const { return _handPower; };
	int get_combPower() const { return _combPower; };
	void clearHand() { _hand.clear(); };
	void sortHand() { std::sort(_hand.begin(), _hand.end(), [](const auto& c1, const auto& c2) {return c2.get_rank() < c1.get_rank(); }); };

private:
	QString _name;
	std::vector<Card> _hand;
	int _handPower;
	int _combPower;
};

