#include "Helper.h"


const QString Helper::HIGHT_CARD("Старшая карта");	// combPower = 1
const QString Helper::ONE_PAIR("Пара");				// combPower = 2
const QString Helper::TWO_PAIR("Две пары");			// combPower = 3
const QString Helper::THREE_OF_A_KIND("Тройка");	// combPower = 4
const QString Helper::STRAIGHT("Стрит");			// combPower = 5
const QString Helper::FLUSH("Флеш");				// combPower = 6
const QString Helper::FULL_HOUSE("Фулл хаус");		// combPower = 7
const QString Helper::FOUR_OF_A_KIND("Каре");		// combPower = 8
const QString Helper::STRAIGHT_FLUSH("Стрит-флеш");	// combPower = 9
const QString Helper::ROYAL_FLUSH("Флеш-рояль");	// combPower = 10
const QString Helper::SUITS[4] = { "spades", "clubs", "diamonds", "hearts" };
const int Helper::RANKS[13] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
Combination Helper::_combination;

const Combination& Helper::getCombination(const std::vector<Card>& table, const std::vector<Card>& hand)
{
	std::vector<Card> cards;
	//QMessageBox::information(0, "", "in getCombination");
	/*for (const Card& card : hand)
	{
		QMessageBox::information(0, "", card.get_image());
	}*/
	for (const auto& card : table)
		cards.push_back(std::move(card));

	for (const auto& card : hand)
		cards.push_back(std::move(card));

	std::sort(cards.begin(), cards.end(), [](const auto& c1, const auto& c2) {return c2.get_rank() < c1.get_rank(); });

	_combination.clearHand();
	
	if (checkForStraightFlushOrRoyalFlush(cards) ||
		checkForFourOfKind(cards) ||
		checkForFullHouse(cards) ||
		checkForFlush(cards) ||
		checkForStraight(cards) ||
		checkForThreeOfKind(cards) ||
		checkForTwoPair(cards) ||
		checkForOnePair(cards))
	{
		return _combination;
	}
	else
	{
		_combination.set_name(HIGHT_CARD);
		_combination.set_combPower(1);
		_combination.set_handPower(cards[0].get_rank());
		_combination.addCard(&cards[0]);
		return _combination;
	}
}

bool Helper::checkForOnePair(const std::vector<Card>& cards)
{
	_combination.clearHand();

	for (size_t i = 1; i < cards.size(); ++i)
	{
		if (cards[i].get_rank() == cards[i - 1].get_rank())
		{
			_combination.set_name(ONE_PAIR);
			_combination.set_combPower(2);
			_combination.addCard(&cards[i]);
			_combination.addCard(&cards[i - 1]);
			_combination.set_handPower(cards[i].get_rank());
			return true;
		}
	}
	return false;
}

bool Helper::checkForTwoPair(const std::vector<Card>& cards)
{
	_combination.clearHand();

	for (size_t i = 1; i < cards.size(); ++i)
	{
		if (cards[i].get_rank() == cards[i - 1].get_rank())
		{
			_combination.addCard(&cards[i]);
			_combination.addCard(&cards[i - 1]);
			if (_combination.get_hand().size() == 4)
				break;
		}
	}

	if (_combination.get_hand().size() != 4)
		return false;

	_combination.sortHand();
	_combination.set_name(TWO_PAIR);
	_combination.set_combPower(3);
	_combination.set_handPower(_combination.get_hand()[0].get_rank());
	return true;
}

bool Helper::checkForThreeOfKind(const std::vector<Card>& cards)
{
	_combination.clearHand();
	for (size_t i = 0; i < cards.size(); ++i)
	{
		if (i == 0)
			continue;
		if (cards[i].get_rank() == cards[i - 1].get_rank())
		{
			if (_combination.get_hand().size() == 0)
				_combination.addCard(&cards[i - 1]);
			_combination.addCard(&cards[i]);
			if (_combination.get_hand().size() == 3)
				break;
		}
		else
		{
			_combination.clearHand();
		}
	}

	if (_combination.get_hand().size() != 3)
		return false;

	_combination.sortHand();
	_combination.set_name(THREE_OF_A_KIND);
	_combination.set_combPower(4);
	_combination.set_handPower(_combination.get_hand()[0].get_rank());
	return true;
}

bool Helper::checkForStraight(const std::vector<Card>& cards)
{
	_combination.clearHand();
	for (size_t i = 0; i < cards.size(); ++i)
	{
		if (i == 0)
			continue;
		if (cards[i].get_rank() == cards[i - 1].get_rank() - 1)
		{
			if (_combination.get_hand().size() == 0)
				_combination.addCard(&cards[i - 1]);
			_combination.addCard(&cards[i]);
			if (_combination.get_hand().size() == 5)
				break;
		}
		else if (cards[i].get_rank() == cards[i - 1].get_rank())
			continue;
		else
			_combination.clearHand();
	}

	if (_combination.get_hand().size() != 5)
		return false;

	_combination.sortHand();
	_combination.set_name(STRAIGHT);
	_combination.set_combPower(5);
	_combination.set_handPower(_combination.get_hand()[0].get_rank());

	return true;
}

bool Helper::checkForFlush(const std::vector<Card>& cards)
{
	for (const auto& suit : SUITS)
	{
		_combination.clearHand();
		for (const Card& card : cards)
		{
			if (card.get_suit() == suit)
				_combination.addCard(card);
		}
		if (_combination.get_hand().size() >= 5)
			break;
	}

	if (_combination.get_hand().size() < 5)
		return false;

	_combination.sortHand();
	
	_combination.set_name(FLUSH);
	_combination.set_combPower(6);
	_combination.set_handPower(_combination.get_hand()[0].get_rank());

	return true;
}

bool Helper::checkForFullHouse(const std::vector<Card>& cards)
{
	_combination.clearHand();
	std::vector<Card> three;
	three.reserve(3);
	std::vector<Card> two;
	three.reserve(2);
	
	for (size_t i = 0; i < cards.size(); ++i)
	{
		if (i == 0)
			continue;
		if (cards[i].get_rank() == cards[i - 1].get_rank())
		{
			if (three.size() == 0)
				three.push_back(std::move(&cards[i - 1]));
			three.push_back(std::move(&cards[i]));
			if (three.size() == 3)
				break;
		}
		else
			three.clear();
	}

	if (three.size() != 3)
		return false;

	for (size_t i = 0; i < cards.size(); ++i)
	{
		if (i == 0)
			continue;
		if (cards[i].get_rank() == cards[i - 1].get_rank() && cards[i].get_rank() != three[0].get_rank())
		{
			if (two.size() == 0)
				two.push_back(std::move(&cards[i - 1]));
			two.push_back(std::move(&cards[i]));
			if (two.size() == 2)
				break;
		}
		else
			two.clear();
	}

	if (two.size() != 2)
		return false;

	for (const Card& card : three)
		_combination.addCard(card);
	for (const Card& card : two)
		_combination.addCard(card);

	_combination.set_name(FULL_HOUSE);
	_combination.set_combPower(7);
	_combination.set_handPower(_combination.get_hand()[0].get_rank());
	return true;
}

bool Helper::checkForFourOfKind(const std::vector<Card>& cards)
{
	_combination.clearHand();
	for (size_t i = 0; i < cards.size(); ++i)
	{
		if (i == 0)
			continue;
		if (cards[i].get_rank() == cards[i - 1].get_rank())
		{
			if (_combination.get_hand().size() == 0)
				_combination.addCard(&cards[i - 1]);
			_combination.addCard(&cards[i]);
			if (_combination.get_hand().size() == 4)
				break;
		}
		else
		{
			_combination.clearHand();
		}
	}

	if (_combination.get_hand().size() != 4)
		return false;

	_combination.sortHand();
	_combination.set_name(FOUR_OF_A_KIND);
	_combination.set_combPower(8);
	_combination.set_handPower(_combination.get_hand()[0].get_rank());
	return true;
}

bool Helper::checkForStraightFlushOrRoyalFlush(const std::vector<Card>& cards)
{
	_combination.clearHand();
	if (!checkForStraight(cards))
		return false;

	for (size_t i = 0; i < _combination.get_hand().size(); ++i)
	{
		if (i == 0)
			continue;
		if (_combination.get_hand()[i].get_suit() != _combination.get_hand()[i - 1].get_suit())
			return false;
	}

	_combination.sortHand();
	if (_combination.get_hand()[0].get_rank() == 14)
	{
		_combination.set_name(ROYAL_FLUSH);
		_combination.set_combPower(10);
	}
	else
	{
		_combination.set_name(STRAIGHT_FLUSH);
		_combination.set_combPower(9);
	}
	_combination.set_handPower(_combination.get_hand()[0].get_rank());

	return true;
}

//const QPixmap& Helper::getShirtImage(const int var)
//{
//	switch (var)
//	{
//	case 1:
//		return QPixmap("cards/1_shirt.png");
//		break;
//	case 2:
//		return QPixmap("cards/2_shirt.png");
//		break;
//	}
//}
//
//const QString& Helper::getShirtString(const int var)
//{
//	qDebug() << "in getShirtString";
//	QString shirt = "";
//	switch (var)
//	{
//	case 1:
//		shirt = "cards/1_shirt.png";
//		break;
//	case 2:
//		shirt = "cards/2_shirt.png";
//		break;
//	}
//	return shirt;
//}

//const QPixmap& Helper::getDealerImage()
//{
//	return QPixmap("imgs/dealer1.png");
//}

const QSettings& Helper::getSettings()
{
	return QSettings(QDir::currentPath() + "/src/settings.ini", QSettings::IniFormat);	
}
