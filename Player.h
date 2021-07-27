#pragma once
#include <qstring.h>
#include <vector>
#include <QGroupBox>
#include "ui_MainInterface.h"
#include "Card.h"
#include <QDebug>
#include "Combination.h"

class Player
{
public:
	Player(int id, bool ai, const QString& name, int money);
	Player();
	~Player() { qDebug() << "destroy " << _id; };

	void set_id(int id) { _id = id; };
	void set_name(const QString& name) { _name = name; };
	void set_image(const QString& image) { _image = image; };
	void setup_imageUI();
	void set_bet(int bet) { _bet = bet; };
	void set_winner(bool value) { _winner = value; };
	void set_showdown(bool value) { _showdown = value; };
	void set_check(bool value) { _check = value; };
	void set_call(bool value) { _call = value; };
	void set_raised(bool value) { _raised = value; };
	void set_allIn(bool value) { _allIn = value; };
	void set_fold(bool value);
	void set_turn(int value) { _turn = value; };
	void set_money(int money) { _money = money; };
	void set_inGame(bool value);
	void set_ai(bool value) { _ai = value; };
	void set_smallBlind(bool value) { _smallBlind = value; };
	void set_bigBlind(bool value) { _bigBlind = value; };
	void set_bluffer(bool value) { _bluffer = value; };
	void set_combination(const Combination& combination) { _combination = combination; };
	void set_dealer(bool value) { _dealer = value; }
	void addCardToHand(const Card& card) { _hand.push_back(card); };
	void addToMoney(int value) { _money += value; };
	void addCardToCombination(const Card& card) { _combination.addCard(card); };
	void takeFromMoney(int value) { _money -= value; };
	void clearHand() { _hand.clear(); };
	void updateMoneyUI() { _moneyUI->setText(QString::number(_money)); };
	void updateBetUI() { _betUI->setText(QString::number(_bet)); };
	void set_nameUI(QLabel*& label) { _nameUI = label; };
	void set_infoUI(QLabel*& label) { _infoUI = label; };
	void set_dealerUI(QLabel*& label) { _dealerUI = label; };
	void set_betUI(QLabel*& label) { _betUI = label; };
	void set_moneyUI(QLabel*& label) { _moneyUI = label; };
	void set_moneySymbolUI(QLabel*& label) { _moneySymbolUI = label; };
	void set_moneyChipsUI(QLabel*& label) { _moneyChipsUI = label; };
	void set_imageUI(QLabel*& label) { _imageUI = label; };
	void set_chipUI(QLabel*& label) { _chipUI = label; };
	void set_card1UI(QLabel*& label) { _card1UI = label; };
	void set_card2UI(QLabel*& label) { _card2UI = label; };
	void set_generalLayout(QFrame*& layout) { _generalLayout = layout; }
	void setCardImage(int cardIndex);
	void setCardShirtImage(int cardIndex);
	void setCardUIDisable(int index);

	int get_id() const { return _id; };
	const QString& get_name() const { return _name; };
	const QString& get_image() const { return _image; };
	int get_bet() const { return _bet; };
	bool is_showdown() const { return _showdown; };
	bool is_winner() const { return _winner; };
	bool is_check() const { return _check; };
	bool is_call() const { return _call; };
	bool is_raised() const { return _raised; };
	bool is_allIn() const { return _allIn; };
	bool is_fold() const { return _fold; };
	int get_turn() const { return _turn; };
	int get_money() const { return _money; };
	bool inGame() const { return _inGame; };
	bool is_ai() const { return _ai; };
	bool is_smallBlind() const { return _smallBlind; };
	bool is_bigBlind() const { return _bigBlind; };
	bool is_bluffer() const { return _bluffer; };
	bool is_dealer() const { return _dealer; };
	const std::vector<Card>& get_hand() const { return _hand; };
	void sortHand() { std::sort(_hand.begin(), _hand.end(), [](const auto& c1, const auto& c2) {return c2.get_rank() < c1.get_rank(); }); };
	const Combination& get_combination() const { return _combination; };
	QLabel*& get_nameUI() { return _nameUI; };
	QLabel*& get_infoUI() { return _infoUI; };
	QLabel*& get_dealerUI() { return _dealerUI; };
	QLabel*& get_betUI() { return _betUI; };
	QLabel*& get_moneyUI() { return _moneyUI; };
	QLabel*& get_moneySymbolUI() { return _moneySymbolUI; };
	QLabel*& get_moneyChipsUI() { return _moneyChipsUI; };
	QLabel*& get_imageUI() { return _imageUI; };
	QLabel*& get_chipUI() { return _chipUI; };
	QLabel*& get_card1UI() { return _card1UI; };
	QLabel*& get_card2UI() { return _card2UI; };
	QLabel*& get_cardUI(int cardNumber);
	QFrame*& get_generalLayout() { return _generalLayout; };

private:
	int _id;
	QString _name;
	QString _image;
	QLabel* _nameUI;
	QLabel* _infoUI;
	QLabel* _dealerUI;
	QLabel* _betUI;
	QLabel* _moneyUI;
	QLabel* _moneySymbolUI;
	QLabel* _moneyChipsUI;
	QLabel* _imageUI;
	QLabel* _chipUI;
	QLabel* _card1UI;
	QLabel* _card2UI;
	QFrame* _generalLayout;
	int _bet;
	bool _showdown;
	bool _winner;
	bool _check;
	bool _call;
	bool _raised;
	bool _allIn;
	bool _fold;
	int _turn;
	bool _dealer;
	int _money;
	bool _inGame;
	bool _smallBlind;
	bool _bigBlind;
	bool _bluffer;
	bool _ai;
	Combination _combination;
	std::vector<Card> _hand;
};

