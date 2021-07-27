#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qmessagebox.h>
#include <QRandomGenerator>
#include <QTimer>
#include <qdatetime.h>
#include <QDebug>
#include <QFileDialog>
#include <QShortcut>
#include <QSettings>
#include <QPropertyAnimation>
#include <qpen.h>
#include <qpainter.h>
#include "ui_MainInterface.h"
#include "MainMenu.h"
#include "Player.h"
#include "Card.h"
#include "Helper.h"
#include "Game.h"
#include "SettingsForm.h"
#include "RulesForm.h"
#include "WinLoseForm.h"
#include <QParallelAnimationGroup>
#include <QMediaPlayer>

class MainInterface : public QMainWindow
{
    Q_OBJECT

public:
    MainInterface(QWidget *parent = Q_NULLPTR);

private:
    Ui::MainInterfaceClass ui;
    std::vector<int> _deck;
    std::vector<Card> _cards;
    std::vector<Card> _table;
    std::vector<Player> _players;
    Game _game;
    int _dealDelay;// 500
    int _compActionDelay = 800; //800
    int _chipsDuration; //400
    int _whoWinDelay = 3000;
    int _dealerChipDuration = 800;
    int _bigBlind;
    int _deposit;
    int _city;
    int _plrDeposit;
    int _cardImage;
    int _shirtImage;
    QString _shirtImageStr;
    QString _background;
    QTimer* _dealToPlayersTimer;
    QTimer* _dealToTableTimer;
    QTimer* _blindsTimer;
    QTimer* _compActionTimer;
    //QMediaPlayer* _soundPlayer;
    QString _message;
    QString _playerName;
    QShortcut* keyCtrlM;
    QShortcut* keyCtrlT;

    void setBackground();
    void resizeEvent(QResizeEvent* evt);
    void set_cardImage(int var) { _cardImage = var; };
    void set_shirtImage(int var);
    void init_cards(int var);
    void init_deck();
    void init_players();
    void updatePlayerUI(Player& player);
    void dealCardsToTable();
    void clearTable();
    void cancelPlayers();
    const Card* getCardFromDeck(int randomCard);
    const Card* findCard(int cardId);
    Player* findPlayer(int playerId);
    Player* findDealer();
    QLabel* findChipLbl(int playerId);
    void giveCardToPlayer(Player& player, int cardNumber);
    bool checkToWin();
    void getWinner();
    void setTurnsToDeal(int turnCount);
    void setNextDealer();
    void setNameRound(const QString& nameRound) { ui.round_lbl->setText(nameRound); };
    void clearPlayerUI(Player& player);
    void clearPlayersBetUI();
    void updateBankUI();
    void compAction(Player& player);
    void compCheck(Player& player);
    void compRaise(Player& player, bool allIn, int minBet, int maxBet);
    void compCall(Player& player);
    void compFold(Player& player);
    void setPlayersCheckFalse();
    void setPlayersCallFalse(int besidesPlayerId);
    void setPlayersCallFalse();
    void setEnabledActionBtns(bool value);
    void setCardImageToTable(int index); 
    void setDisabledCardOnTable(int index);
    void setBorderOnTableWinnerCard(int index);
    bool checkGameOnAllIn();
    void showWinner(Player& player);
    void setEnabledBetsBtns(bool value);
    void increaseBet(int value);
    void saveData();
    void updateSettings();
    void closeEvent(QCloseEvent* event) { saveData(); };
    void setData();
    void setChipImage(QLabel* label, const int bet);
    void dealToPlayersAnimation(int recipientId, int cardNumber);
    void dealToTableAnimation(int cardNumber);
    void chipSingleAnimation(int playerId);
    void chipGroupAnimation();
    void setVisibleChips(bool value);
    void foldAnimation(const int playerId);
    void winAnimation(const int playerId);
    void dealerChipAnimation(const int previousDealerId, const int nextDealerId);
    void setBankChipsImage();
    int getNotFoldCount();
    void clearPlayerImageBorder();
    const QString getTurnBorder();
    void updatePlayersChips(int playerId);
    void setVisibleFoldCards(bool value);

private slots:

    void testBtnSlot();

    void startNewGame();
    void nextRound();
    void startGame();    
    void dealCardsToPlayersSlot();
    void dealCardsToTableSlot();
    void getBlindsSlot();
    void playerActionSlot();
    void compActionSlot();
    void checkBtnSlot();
    void setBetSliderSlot(int value);
    void raiseBtnSlot(bool checked);
    void callBtnSlot();
    void foldBtnSlot();
    void acceptBetBtnSlot();
    void showWinnerSlot();
    void cancelBetSlot();
    void plus10Slot() { increaseBet(_bigBlind / 100); };
    void plus50Slot() { increaseBet(_bigBlind / 10 / 2); };
    void plus100Slot() { increaseBet(_bigBlind / 10); };
    void plus200Slot() { increaseBet(_bigBlind / 10 * 2); };
    void setAllInBetSlot();
    void leaveTableSlot();
    void showCombinationsHelpSlot();
    void setImagePlayerCardSlot();
    void setImageTableCardSlot();
    void setImagePlayerChipSlot();
    void setImageDealerSlot();
    void setBankImageSlot();
    void endWinAnimationSlot();
    void endFoldAnimationSlot() { setVisibleFoldCards(false); };
    void winAnimationSlot();
    void showDownCardsSlot();
    void showSettingsFormSlot();
    void showRulesFormSlot();
    void cheatBtnSlot();
    void showCheatBtnSlot();
    void showTestBtnSlot();
};
