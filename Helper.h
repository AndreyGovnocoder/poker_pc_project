#pragma once
#include <qstring.h>
#include <QtWidgets\qmessagebox.h>
#include <vector>
#include "Card.h"
#include "Combination.h"
#include "MainInterface.h"

class Helper
{
public:
	Helper() = default;
	~Helper() = default;

    static const QString HIGHT_CARD;
    static const QString ONE_PAIR;
    static const QString TWO_PAIR;
    static const QString THREE_OF_A_KIND;
    static const QString STRAIGHT;
    static const QString FLUSH;
    static const QString FULL_HOUSE;
    static const QString FOUR_OF_A_KIND;
    static const QString STRAIGHT_FLUSH;
    static const QString ROYAL_FLUSH;
    static const QString SUITS[4];
    static const int RANKS[13];

	static const Combination& getCombination(const std::vector<Card>& table, const std::vector<Card>& hand);
    static bool checkForOnePair(const std::vector<Card>& cards);
    static bool checkForTwoPair(const std::vector<Card>& cards);
    static bool checkForThreeOfKind(const std::vector<Card>& cards);
    static bool checkForStraight(const std::vector<Card>& cards);
    static bool checkForFlush(const std::vector<Card>& cards);
    static bool checkForFullHouse(const std::vector<Card>& cards);
    static bool checkForFourOfKind(const std::vector<Card>& cards);
    static bool checkForStraightFlushOrRoyalFlush(const std::vector<Card>& cards);
    /*static const QPixmap& getShirtImage(const int var);
    static const QString& getShirtString(const int var);*/
    //static const QPixmap& getDealerImage();
    static const QSettings& getSettings();

private:
    static Combination _combination;

};

