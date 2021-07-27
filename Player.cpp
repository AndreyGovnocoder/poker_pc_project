#include "Player.h"

Player::Player(int id, bool ai, const QString& name, int money)
	: _id(id)
	, _ai(ai)
	,_name(name)
	, _money(money)
	, _bet(0)
	, _turn(0)
	, _dealer(false)
	, _showdown(false)
	, _winner(false)
	, _call(false)
	, _fold(false)
	, _raised(false)
	, _allIn(false)
	, _inGame(false)
	, _smallBlind(false)
	, _bigBlind(false)
	, _bluffer(false)
{
	_hand.reserve(2);
}

Player::Player()
{
	_id = -1;
	_name = "<Имя игрока>";
	_money = 0;
	_bet = 0;
	_turn = 0;
	_dealer = false;
	_showdown = false;
	_winner = false;
	_call = false;
	_raised = false;
	_allIn = false;
	_fold = false;
	_inGame = false;
	_ai = true;
	_smallBlind = false;
	_bigBlind = false;
	_bluffer = false;
	_hand.reserve(2);
	_nameUI = new QLabel();
}

void Player::setup_imageUI()
{
	if (_image.isEmpty())
		return;

	_imageUI->setPixmap(QPixmap(_image));
}

void Player::set_fold(bool value)
{
	_fold = value;
	_generalLayout->setEnabled(!value);
}

void Player::set_inGame(bool value)
{
	_inGame = value;
	if (_inGame)
	{
		_generalLayout->setEnabled(true);
		setup_imageUI();
	}
	else
	{
		_generalLayout->setEnabled(false);
		_imageUI->clear();
		_nameUI->clear();
		_moneyUI->clear();
		_moneySymbolUI->clear();
		_infoUI->clear();
		_betUI->clear();
		_dealerUI->clear();
	}
}

void Player::setCardImage(int cardIndex)
{
	if (cardIndex == 0 && _hand.size() >= 1)
		_card1UI->setPixmap(QPixmap(_hand[0].get_image()));
	else if (cardIndex == 1 && _hand.size() == 2)
		_card2UI->setPixmap(QPixmap(_hand[1].get_image()));
}

void Player::setCardShirtImage(int cardIndex)
{
	if (cardIndex == 0 && _hand.size() >= 1)
		_card1UI->setPixmap(QPixmap(_hand[0].get_shirtImage()));
	else if (cardIndex == 1 && _hand.size() == 2)
		_card2UI->setPixmap(QPixmap(_hand[1].get_shirtImage()));
}

void Player::setCardUIDisable(int index)
{
	if (index == 0)
		_card1UI->setEnabled(false);
	else if (index == 1)
		_card2UI->setEnabled(false);
}

QLabel*& Player::get_cardUI(int cardNumber)
{
	switch (cardNumber)
	{
	case 1:
		return _card1UI;
		break;
	case 2:
		return _card2UI;
		break;
	}
}