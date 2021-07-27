#include "MainInterface.h"

MainInterface::MainInterface(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setVisibleFoldCards(false);
    setVisibleChips(false);
    ui.cheat_btn->setVisible(false);
    ui.test_btn->setVisible(false);
    ui.bankChips->setVisible(false);
    ui.dealerChip->setPixmap(QPixmap("imgs/dealer1.png"));
    ui.dealerChip->setVisible(false);
    ui.dealerChip->setParent(this);
    ui.card_1->setParent(this);
    ui.card_2->setParent(this);
    ui.chip_1->setParent(this);
    ui.chip_2->setParent(this);
    ui.chip_3->setParent(this);
    ui.chip_4->setParent(this);
    ui.chip_5->setParent(this);
    ui.bankChips->setParent(this);
    _deck.reserve(52);
    _cards.reserve(52);
    _table.reserve(5);
    _players.reserve(5);
    
    _dealToPlayersTimer = new QTimer(this);
    _dealToTableTimer = new QTimer(this);
    _blindsTimer = new QTimer(this);
    _compActionTimer = new QTimer(this);

    //_soundPlayer = new QMediaPlayer(this);

    keyCtrlM = new QShortcut(this);
    keyCtrlM->setKey(Qt::CTRL + Qt::Key_M);
    connect(keyCtrlM, SIGNAL(activated()), this, SLOT(showCheatBtnSlot()));

    keyCtrlT = new QShortcut(this);
    keyCtrlT->setKey(Qt::CTRL + Qt::Key_T);
    connect(keyCtrlT, SIGNAL(activated()), this, SLOT(showTestBtnSlot()));
    
    connect(_dealToPlayersTimer, SIGNAL(timeout()), this, SLOT(dealCardsToPlayersSlot()));
    connect(_dealToTableTimer, SIGNAL(timeout()), this, SLOT(dealCardsToTableSlot()));
    connect(_blindsTimer, SIGNAL(timeout()), this, SLOT(getBlindsSlot()));
    setData();
}

void MainInterface::setBackground()
{
    QPixmap bkgnd(_background);
    //bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatioByExpanding);
    //bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    palette.setColor(QPalette::WindowText, Qt::white);
    this->setPalette(palette);
}

void MainInterface::resizeEvent(QResizeEvent* evt)
{
    QPixmap bkgnd(_background);
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette p = palette(); 
    p.setBrush(QPalette::Background, bkgnd);
    setPalette(p);

    QMainWindow::resizeEvent(evt); //call base implementation
}

void MainInterface::set_shirtImage(int var)
{
    switch (var)
    {
    case 1:
        _shirtImageStr = "cards/1_shirt.png";
        break;
    case 2:
        _shirtImageStr = "cards/2_shirt.png";
        break;
    }
}

void MainInterface::init_cards(int var)
{
    _cards.clear();
    QString path = "cards/";
    
    switch (var)
    {
    case 1:
        path += "1_";
        break;
    case 2:
        path += "2_";
        break;
    }

    int id = 0;
    for (auto& suit : Helper::SUITS)
    {
        for (auto& rank : Helper::RANKS)
        {
            QString imagePath = path + suit[0] + QString::number(rank) + ".png";
            Card card(id, suit, rank, imagePath, _shirtImageStr);
            _cards.push_back(std::move(card));
            ++id;
        }
    }
}

void MainInterface::init_deck()
{
    _deck.clear();
    for (const auto& card : _cards)
        _deck.push_back(std::move(card.get_id()));
}

void MainInterface::init_players()
{
    _players.clear();
    Player player(0, false, _playerName, _plrDeposit);
    player.set_nameUI(ui.playerName_lbl);
    player.set_infoUI(ui.playerInfo_lbl);
    player.set_dealerUI(ui.playerDealer_lbl);
    player.set_betUI(ui.plrBet_lbl);
    player.set_moneyUI(ui.plrMoneyValue_lbl);
    player.set_moneySymbolUI(ui.plrMoney_lbl);
    player.set_moneyChipsUI(ui.playerChips_lbl);
    player.set_imageUI(ui.plrImage);
    player.set_chipUI(ui.playerChip_lbl);
    player.set_card1UI(ui.plrCard_1);
    player.set_card2UI(ui.plrCard_2);
    player.set_generalLayout(ui.playerFrame);
    player.set_image("imgs/compImage_1.png");
    player.set_inGame(true);
    updatePlayerUI(player);
    _players.push_back(std::move(player));

    Player comp1(1, true, "Игрок 1", _deposit);
    comp1.set_nameUI(ui.comp1Name_lbl);
    comp1.set_infoUI(ui.comp1Info_lbl);
    comp1.set_dealerUI(ui.comp1dealer_lbl);
    comp1.set_betUI(ui.comp1Bet_lbl);
    comp1.set_moneyUI(ui.comp1MoneyValue_lbl);
    comp1.set_moneySymbolUI(ui.comp1Money_lbl);
    comp1.set_moneyChipsUI(ui.comp1Chips_lbl);
    comp1.set_imageUI(ui.comp1_image);
    comp1.set_chipUI(ui.comp1Chip_lbl);
    comp1.set_card1UI(ui.comp1Card_1);
    comp1.set_card2UI(ui.comp1Card_2);
    comp1.set_generalLayout(ui.comp1Frame);
    comp1.set_image("imgs/compImage_1.png");
    comp1.set_inGame(true);
    updatePlayerUI(comp1);
    _players.push_back(std::move(comp1));

    Player comp2(2, true, "Игрок 2", _deposit);
    comp2.set_nameUI(ui.comp2Name_lbl);
    comp2.set_infoUI(ui.comp2Info_lbl);
    comp2.set_dealerUI(ui.comp2dealer_lbl);
    comp2.set_betUI(ui.comp2Bet_lbl);
    comp2.set_moneyUI(ui.comp2MoneyValue_lbl);
    comp2.set_moneySymbolUI(ui.comp2Money_lbl);
    comp2.set_moneyChipsUI(ui.comp2Chips_lbl);
    comp2.set_imageUI(ui.comp2_image);
    comp2.set_chipUI(ui.comp2Chip_lbl);
    comp2.set_card1UI(ui.comp2Card_1);
    comp2.set_card2UI(ui.comp2Card_2);
    comp2.set_generalLayout(ui.comp2Frame);
    comp2.set_image("imgs/compImage_1.png");
    comp2.set_inGame(true);
    updatePlayerUI(comp2);
    _players.push_back(std::move(comp2));

    Player comp3(3, true, "Игрок 3", _deposit);
    comp3.set_nameUI(ui.comp3Name_lbl);
    comp3.set_infoUI(ui.comp3Info_lbl);
    comp3.set_dealerUI(ui.comp3dealer_lbl);
    comp3.set_betUI(ui.comp3Bet_lbl);
    comp3.set_moneyUI(ui.comp3MoneyValue_lbl);
    comp3.set_moneySymbolUI(ui.comp3Money_lbl);
    comp3.set_moneyChipsUI(ui.comp3Chips_lbl);
    comp3.set_imageUI(ui.comp3_image);
    comp3.set_chipUI(ui.comp3Chip_lbl);
    comp3.set_card1UI(ui.comp3Card_1);
    comp3.set_card2UI(ui.comp3Card_2);
    comp3.set_generalLayout(ui.comp3Frame);
    comp3.set_image("imgs/compImage_1.png");
    comp3.set_inGame(true);
    updatePlayerUI(comp3);
    _players.push_back(std::move(comp3));

    Player comp4(4, true, "Игрок 4", _deposit);
    comp4.set_dealer(true);
    comp4.set_nameUI(ui.comp4Name_lbl);
    comp4.set_infoUI(ui.comp4Info_lbl);
    comp4.set_dealerUI(ui.comp4dealer_lbl);
    comp4.set_betUI(ui.comp4Bet_lbl);
    comp4.set_moneyUI(ui.comp4MoneyValue_lbl);
    comp4.set_moneySymbolUI(ui.comp4Money_lbl);
    comp4.set_moneyChipsUI(ui.comp4Chips_lbl);
    comp4.set_imageUI(ui.comp4_image);
    comp4.set_chipUI(ui.comp4Chip_lbl);
    comp4.set_card1UI(ui.comp4Card_1);
    comp4.set_card2UI(ui.comp4Card_2);
    comp4.set_generalLayout(ui.comp4Frame);
    comp4.set_image("imgs/compImage_1.png");
    comp4.set_inGame(true);
    updatePlayerUI(comp4);
    _players.push_back(std::move(comp4));

    updatePlayersChips(0);
    updatePlayersChips(1);
    updatePlayersChips(2);
    updatePlayersChips(3);
    updatePlayersChips(4);
}

void MainInterface::updatePlayerUI(Player& player)
{
    player.get_nameUI()->setText(player.get_name());
    player.get_moneyUI()->setText(QString::number(player.get_money()));    
}

void MainInterface::dealCardsToTable()
{
    int random;
    int highest = _deck.size() - 1;
    switch (_game.get_round())
    {
    case 1:
        for (int i = 0; i < 3; ++i)
        {
            random = QRandomGenerator::global()->bounded(0, highest);
            _table.push_back(std::move(getCardFromDeck(random)));
        }

        ui.tableCard_1->setPixmap(QPixmap(_table[0].get_image()));
        ui.tableCard_2->setPixmap(QPixmap(_table[1].get_image()));
        ui.tableCard_3->setPixmap(QPixmap(_table[2].get_image()));        
        break;

    case 2:
        random = QRandomGenerator::global()->bounded(0, highest);
        _table.push_back(std::move(getCardFromDeck(random)));
        ui.tableCard_4->setPixmap(QPixmap(_table[3].get_image()));
        break;

    case 3:
        random = QRandomGenerator::global()->bounded(0, highest);
        _table.push_back(getCardFromDeck(random));
        ui.tableCard_5->setPixmap(QPixmap(_table[4].get_image()));
        break;
    }
}

void MainInterface::clearTable()
{
    _table.clear();
    ui.bankValue_lbl->setText(QString::number(0));
    ui.tableCard_1->clear();
    ui.tableCard_2->clear();
    ui.tableCard_3->clear();
    ui.tableCard_4->clear();
    ui.tableCard_5->clear();
    ui.tableCard_1->setStyleSheet("");
    ui.tableCard_2->setStyleSheet("");
    ui.tableCard_3->setStyleSheet("");
    ui.tableCard_4->setStyleSheet("");
    ui.tableCard_5->setStyleSheet("");
    ui.tableCard_1->setEnabled(true);
    ui.tableCard_2->setEnabled(true);
    ui.tableCard_3->setEnabled(true);
    ui.tableCard_4->setEnabled(true);
    ui.tableCard_5->setEnabled(true);
    ui.plrCard_1->clear();
    ui.plrCard_2->clear();
    ui.plrCard_1->setStyleSheet("");
    ui.plrCard_2->setStyleSheet("");
    ui.plrCard_1->setEnabled(true);
    ui.plrCard_2->setEnabled(true);
    ui.comp1Card_1->clear();
    ui.comp1Card_2->clear();
    ui.comp2Card_1->clear();
    ui.comp2Card_2->clear();
    ui.comp3Card_1->clear();
    ui.comp3Card_2->clear();
    ui.comp4Card_1->clear();
    ui.comp4Card_2->clear();
    ui.comp1Card_1->setStyleSheet("");
    ui.comp1Card_2->setStyleSheet("");
    ui.comp2Card_1->setStyleSheet("");
    ui.comp2Card_2->setStyleSheet("");
    ui.comp3Card_1->setStyleSheet("");
    ui.comp3Card_2->setStyleSheet("");
    ui.comp4Card_1->setStyleSheet("");
    ui.comp4Card_2->setStyleSheet("");
    ui.comp1Card_1->setEnabled(true);
    ui.comp1Card_2->setEnabled(true);
    ui.comp2Card_1->setEnabled(true);
    ui.comp2Card_2->setEnabled(true);
    ui.comp3Card_1->setEnabled(true);
    ui.comp3Card_2->setEnabled(true);
    ui.comp4Card_1->setEnabled(true);
    ui.comp4Card_2->setEnabled(true);
}

void MainInterface::cancelPlayers()
{
    for (auto& player : _players)
    {
        player.clearHand();
        player.set_bet(0);
        player.set_combination(Combination());
        player.set_allIn(false);
        player.set_check(false);
        player.set_call(false);
        player.set_raised(false);
        player.set_fold(false);
        player.set_showdown(false);
        player.set_winner(false);
        player.set_smallBlind(false);
        player.set_bigBlind(false);
    }
}

const Card* MainInterface::getCardFromDeck(int randomCard)
{
    int cardId = _deck[randomCard];
    _deck.erase(std::remove(_deck.begin(), _deck.end(), cardId), _deck.end());
    return findCard(cardId);
}

const Card* MainInterface::findCard(int cardId)
{
    auto it = std::find_if(_cards.begin(), _cards.end(), [&cardId](const Card& card) {return card.get_id() == cardId; });
    return (it == _cards.end()) ? nullptr : &(*it);
}

Player* MainInterface::findPlayer(int playerId)
{
    auto it = std::find_if(_players.begin(), _players.end(), [&playerId](const Player& player) {return player.get_id() == playerId; });
    return (it == _players.end()) ? nullptr : &(*it);
}

Player* MainInterface::findDealer()
{
    auto it = std::find_if(_players.begin(), _players.end(), [](const Player& player) {return player.is_dealer(); });
    return (it == _players.end()) ? nullptr : &(*it);
}

QLabel* MainInterface::findChipLbl(int playerId)
{
    switch (playerId)
    {
    case 0:
        return ui.chip_5;
        break;
    case 1:
        return ui.chip_1;
        break;
    case 2:
        return ui.chip_2;
        break;
    case 3:
        return ui.chip_3;
        break;
    case 4:
        return ui.chip_4;
        break;
    default:
        return nullptr;
    }
}

void MainInterface::giveCardToPlayer(Player& player, int cardNumber)
{
    if (cardNumber == 1)
        player.get_card1UI()->setPixmap(QPixmap(player.get_hand()[0].get_image()));
    else if (cardNumber == 2)
        player.get_card2UI()->setPixmap(QPixmap(player.get_hand()[1].get_image()));
}

bool MainInterface::checkToWin()
{
    int winnerId = -1;
    int notFoldCount = 0;
    for (Player& player : _players)
    {
        if (player.is_fold())
            continue;

        ++notFoldCount;
        winnerId = player.get_id();
    }

    if (notFoldCount > 1)
    {
        return false;
    }
    else if (notFoldCount == 0)
    {
        startGame();
        return true;
    }
    if (winnerId < 0)
        return false;

    Player* winner = findPlayer(winnerId);

    winner->addToMoney(_game.get_bank());
    //_game.set_bank(0);
    winner->set_winner(true);
    //winner.get_imageUI()->setStyleSheet("border: 2px outset rgb(0, 255, 0); border-radius: 4px;");
    
    QTimer::singleShot(1500, this, SLOT(showWinnerSlot()));
    return true;
}

void MainInterface::getWinner()
{
    std::vector<Player> players;
    players.reserve(4);
    for (auto& player : _players)
    {
        if (player.is_fold())
            continue;
        player.set_combination(Helper::getCombination(_table, player.get_hand()));
        players.push_back(std::move(player));
        player.set_showdown(true);
        QTimer::singleShot(1500, this, SLOT(showDownCardsSlot()));
    }
    
    std::sort(players.begin(), players.end(),
        [](const Player& p1, const Player& p2) {return p2.get_combination().get_combPower() < p1.get_combination().get_combPower(); });

    int maxCombPower = players[0].get_combination().get_combPower();
    players.erase(std::remove_if(players.begin(), players.end(),
        [maxCombPower](Player& player) {return player.get_combination().get_combPower() < maxCombPower; }), players.end());
    
    if (players.size() == 1)
    {
        Player* winner = findPlayer(players[0].get_id());
        winner->set_winner(true);
        //winner->get_infoUI()->setText("Победитель");
        winner->addToMoney(_game.get_bank());
        _message = "Выиграл " + winner->get_name() + ". У него " + winner->get_combination().get_name();
        //ui.test_lbl->setText(ui.test_lbl->text() + "\n" + message);
        //_game.set_bank(0);
        QTimer::singleShot(_whoWinDelay, this, SLOT(showWinnerSlot()));
        return;
    }

    std::sort(players.begin(), players.end(),
        [](const Player& p1, const Player& p2) {return p2.get_combination().get_handPower() < p1.get_combination().get_handPower(); });

    int maxHandPower = players[0].get_combination().get_handPower();
    players.erase(std::remove_if(players.begin(), players.end(),
        [maxHandPower](Player& player) {return player.get_combination().get_handPower() < maxHandPower; }), players.end());

    if (players.size() == 1)
    {
        Player* winner = findPlayer(players[0].get_id());
        winner->set_winner(true);
        //winner->get_infoUI()->setText("Победитель");
        winner->addToMoney(_game.get_bank());
        _message = "Выиграл " + winner->get_name() + ". У него " + winner->get_combination().get_name() + ". Комбинации равны, но достоинство его комбинации старше";
        //ui.test_lbl->setText(ui.test_lbl->text() + "\n" + message);
        //_game.set_bank(0);
        QTimer::singleShot(_whoWinDelay, this, SLOT(showWinnerSlot()));
        return;
    }
    else if (players.size() == 2 && maxCombPower == 7)
    {
        if (players[0].get_combination().get_hand()[3].get_rank() > players[1].get_combination().get_hand()[3].get_rank())
        {
            Player* winner = findPlayer(players[0].get_id());
            winner->set_winner(true);
            //winner->get_infoUI()->setText("Победитель");
            winner->addToMoney(_game.get_bank());
            _message = "Выиграл " + winner->get_name() + ". У него пара в фулл хаусе старше";
            //ui.test_lbl->setText(ui.test_lbl->text() + "\n" + message);
            //_game.set_bank(0);
            QTimer::singleShot(_whoWinDelay, this, SLOT(showWinnerSlot()));
            return;
        }
        else if (players[0].get_combination().get_hand()[3].get_rank() < players[1].get_combination().get_hand()[3].get_rank())
        {
            Player* winner = findPlayer(players[1].get_id());
            winner->set_winner(true);
            //winner->get_infoUI()->setText("Победитель");
            winner->addToMoney(_game.get_bank());
            _message = "Выиграл " + winner->get_name() + ". У него пара в фулл хаусе старше";
            //ui.test_lbl->setText(ui.test_lbl->text() + "\n" + message);
            //_game.set_bank(0);
            QTimer::singleShot(_whoWinDelay, this, SLOT(showWinnerSlot()));
            return;
        }
    }

    for (Player& player : players)
        player.sortHand();
    
    std::sort(players.begin(), players.end(),
        [](const Player& p1, const Player& p2) {return p2.get_hand()[0].get_rank() < p1.get_hand()[0].get_rank(); });

    int maxRank = players[0].get_hand()[0].get_rank();
    players.erase(std::remove_if(players.begin(), players.end(),
        [maxRank](Player& player) {return player.get_hand()[0].get_rank() < maxRank; }), players.end());

    if (players.size() == 1)
    {
        Player* winner = findPlayer(players[0].get_id());
        winner->set_winner(true);
        //winner->get_infoUI()->setText("Победитель");
        winner->addToMoney(_game.get_bank());
        winner->addCardToCombination(winner->get_hand()[0]);
        _message = "Выиграл " + winner->get_name() + ". Комбинации равны, но у него старше карта в руке";
        //ui.test_lbl->setText(ui.test_lbl->text() + "\n" + message);
        //_game.set_bank(0);
        QTimer::singleShot(_whoWinDelay, this, SLOT(showWinnerSlot()));
        return;
    }

    std::sort(players.begin(), players.end(),
        [](const Player& p1, const Player& p2) {return p2.get_hand()[1].get_rank() < p1.get_hand()[1].get_rank(); });
    maxRank = players[0].get_hand()[1].get_rank();
    players.erase(std::remove_if(players.begin(), players.end(),
        [maxRank](Player& player) {return player.get_hand()[1].get_rank() < maxRank; }), players.end());

    if (players.size() == 1)
    {
        Player* winner = findPlayer(players[0].get_id());
        winner->set_winner(true);
        //winner->get_infoUI()->setText("Победитель");
        winner->addCardToCombination(winner->get_hand()[1]);
        winner->addToMoney(_game.get_bank());
        _message = "Выиграл " + winner->get_name() + ". Комбинации равны, но у него старше вторая карта в руке";
        //ui.test_lbl->setText(ui.test_lbl->text() + "\n" + message);
        //_game.set_bank(0);
        QTimer::singleShot(_whoWinDelay, this, SLOT(showWinnerSlot()));
        return;
    }

    _message = "Комбинации равнозначны. Банк делится между игроками: ";
    for (const Player& player : players)
    {
        Player* winner = findPlayer(player.get_id());
        winner->set_winner(true);
        //winner->get_infoUI()->setText("Победитель");
        winner->addToMoney(_game.get_bank() / players.size());
        
        _message += player.get_name() + ", ";
    }
    //_game.set_bank(0);
    _message.remove(_message.size() - 2, _message.size() - 1);
    //ui.test_lbl->setText(ui.test_lbl->text() + "\n" + message);
    QTimer::singleShot(_compActionDelay, this, SLOT(showWinnerSlot()));
    return;
}

void MainInterface::setTurnsToDeal(int turnCount)
{
    //if (turnCount == _players.size())
    if (turnCount > _players.size())
    {
        std::sort(_players.begin(), _players.end(), [](const Player& p1, const Player& p2) {return p1.get_turn() < p2.get_turn(); });
        return;
    }

    for (Player& player : _players)
    {
        if (!player.is_dealer())
            continue;
        player.set_turn(_players.size());
        //player.get_imageUI()->setText("turn " + QString::number(_players.size()));
        break;
    }

    for (size_t i = 0; i < _players.size(); ++i)
    {
        if (!_players[i].is_dealer())
            continue;

        int index = i + turnCount;
        if (index >= _players.size())
            index -= _players.size();
        _players[index].set_turn(turnCount);
        //_players[index].get_imageUI()->setText("turn " + QString::number(turnCount));
        if (turnCount == 1)
        {
            _players[index].set_smallBlind(true);
        }
        else if (turnCount == 2)
        {
            _players[index].set_bigBlind(true);
        }
        setTurnsToDeal(++turnCount);
        break;
    }
}

void MainInterface::setNextDealer()
{
    int currDealerId = -1;
    int nextDealerId = -1;
    int newDealerIndex = -1;
    std::sort(_players.begin(), _players.end(), [](const Player& p1, const Player& p2) {return p1.get_id() < p2.get_id(); });
    for (size_t i = 0; i < _players.size(); ++i)
    {
        if (_players[i].is_dealer())
        {
            currDealerId = _players[i].get_id();
            _players[i].set_dealer(false);
            newDealerIndex = i + 1;
        }
    }
    
    if (newDealerIndex >= _players.size())
        newDealerIndex -= _players.size();

    _players[newDealerIndex].set_dealer(true);
    if (!_players[newDealerIndex].inGame())
    {
        setNextDealer();
        return;
    }
    nextDealerId = _players[newDealerIndex].get_id();
    dealerChipAnimation(currDealerId, nextDealerId);
    _players.erase(std::remove_if(_players.begin(), _players.end(),
        [](Player& player) {return !player.inGame(); }), _players.end());
    
    //_players[newDealerIndex].get_dealerUI()->setPixmap(Helper::getDealerImage());
}

void MainInterface::clearPlayerUI(Player& player)
{
    player.get_imageUI()->setStyleSheet("");
    player.get_nameUI()->setStyleSheet("");
    player.get_betUI()->clear();
    player.get_infoUI()->clear();
    player.get_infoUI()->setStyleSheet("");
    player.get_chipUI()->clear();
    player.get_dealerUI()->clear();
    player.get_moneyUI()->clear();
    player.get_card1UI()->clear();
    player.get_card2UI()->clear();
}

void MainInterface::clearPlayersBetUI()
{
    for (auto& player : _players)
        player.get_betUI()->clear();
}

void MainInterface::updateBankUI()
{
    ui.bankValue_lbl->setText(QString::number(_game.get_bank()));
}

void MainInterface::compAction(Player& player)
{
    player.set_combination(Helper::getCombination(_table, player.get_hand()));
    int combPower = player.get_combination().get_combPower();
    int randomAction = QRandomGenerator::global()->bounded(0, 3);
    int playerMoney = player.get_money();
    if (checkGameOnAllIn() && !_game.is_raised())
    {
        player.set_check(true);
        _game.incrTurn(_players.size());
        //playerActionSlot();
        QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
        return;
    }
    else if (player.is_allIn())
    {
        player.set_check(true);
        player.set_call(true);
        _game.incrTurn(_players.size());
        QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
        return;
    }

    if (_game.is_raised())
    {
        playerMoney -= _game.get_bet();
        if (_game.get_round() == 0)
        {
            if (player.is_bigBlind() || player.is_smallBlind())
            {
                compCall(player);
                return;
            }
            if (combPower == 2)
            {
                if (randomAction == 2)
                {
                    compRaise(player, false, 1, playerMoney / 2 / 10);
                    //compRaise(player, false, 1, playerMoney / 10);
                    return;
                }
                compCall(player);
                return;
            }
            else if (_game.get_bet() > (playerMoney / 1.5))
            {
                compFold(player);
                return;
            }

            randomAction = QRandomGenerator::global()->bounded(0, 4);
            if (randomAction == 0 && !player.is_bluffer())
            {
                compFold(player);
                return;
            }
            else if (randomAction == 1)
            {
                player.set_bluffer(true);
                player.get_nameUI()->setStyleSheet("background-color: gray;");
            }

            compCall(player);
            return;
        }

        randomAction = QRandomGenerator::global()->bounded(0, 4);
        if (_game.get_bet() > playerMoney && combPower < 7 && !player.is_bluffer() && randomAction != 0)
        {
            compFold(player);
            return;
        }
        else if (_game.get_bet() > playerMoney / 1.5 && combPower < 5 && !player.is_bluffer() && randomAction != 0)
        {
            compFold(player);
            return;
        }

        if (randomAction == 0 && !player.is_bluffer())
        {
            compFold(player);
            return;
        }
    }

    randomAction = QRandomGenerator::global()->bounded(0, 4);
    if (player.is_bluffer())
    {
        if (randomAction == 1)
        {
            compRaise(player, false, 1, playerMoney / 2 / 10);
            //compRaise(player, false, 1, playerMoney / 10);
            return;
        }
        else if (_game.is_raised())
        {
            compCall(player);
            return;
        }
    }

    if (combPower <= 2 && _game.get_bet() > (playerMoney / 2) && (randomAction == 0 || randomAction == 3))
    {
        compFold(player);
        return;
    }

    if (combPower > 2 && combPower < 5 && randomAction != 3)
    {
        //compRaise(player, false, playerMoney / 4 / 10, playerMoney / 1.6 / 10);
        compRaise(player, false, 1, playerMoney / 10);
        return;
    }

    if (combPower >= 5 && combPower < 8 && randomAction != 1)
    {
        compRaise(player, false, playerMoney / 2 / 10, playerMoney / 10);
        //compRaise(player, false, 1, playerMoney / 10);
        return;
    }

    if (combPower >= 8)
    {
        if (randomAction == 1 || randomAction == 3)
        {
            compRaise(player, false, playerMoney / 1.7 / 10, playerMoney / 10);
            //compRaise(player, false, 1, playerMoney / 10);
        }
        else if (randomAction == 2)
        {
            compRaise(player, true, 1, playerMoney / 10);
        }
        else
            _game.is_raised() ? compCall(player) : compCheck(player);
        return;
    }

    randomAction = QRandomGenerator::global()->bounded(0, 5);
    if (randomAction == 0)
    {
        compRaise(player, false, 1, playerMoney / 3 / 10);
        //compRaise(player, false, 1, playerMoney / 10);
        return;
    }

    _game.is_raised() ? compCall(player) : compCheck(player);
}

void MainInterface::compCheck(Player& player)
{
    player.set_check(true);
    player.get_infoUI()->setText("Чек");
    player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 60, 0, 255), stop:1 rgba(0, 143, 0, 200));");
    _game.incrTurn(_players.size());
    //playerActionSlot();
    QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
}

void MainInterface::compRaise(Player& player, bool allIn, int minBet, int maxBet)
{
    ++maxBet;
    if (maxBet < 10 && maxBet > 0)
        maxBet *= 10;
    else if (maxBet < 0 || maxBet == minBet)
    {
        compCall(player);
        return;
    }

    if (checkGameOnAllIn())
    {
        if (_game.is_raised())
        {
            compCall(player);
            return;
        }
        else
        {
            compCheck(player);
            return;
        }
    }

    int bet = _game.get_bet() - player.get_bet();
    player.takeFromMoney(bet);
    if (player.get_money() <= 0)
    {
        bet += player.get_money();
        player.set_money(0);
        player.set_allIn(true);
    }
    player.set_bet(bet);
    _game.addToBank(bet);
    if (player.is_allIn())
    {
        player.set_call(true);
        player.get_infoUI()->setText("Ва-банк");
        player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(90, 0, 90, 255), stop:1 rgba(230, 0, 230, 200));");
        player.updateBetUI();
        player.updateMoneyUI();
        //updateBankUI();
        _game.incrTurn(_players.size());
        //playerActionSlot();
        QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
        return;
    }

    int raisedBet = QRandomGenerator::global()->bounded(minBet, maxBet);
    raisedBet *= 10;
    if (allIn || raisedBet >= player.get_money())
    {
        raisedBet = player.get_money();
    }
    player.takeFromMoney(raisedBet);
    if (player.get_money() <= 0)
    {
        player.set_money(0);
        player.get_infoUI()->setText("Ва-банк");
        player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(90, 0, 90, 255), stop:1 rgba(230, 0, 230, 200));");
        player.set_allIn(true);
    }
    else 
    {
        if (_game.is_raised())
        {
            player.get_infoUI()->setText("Колл, райз");
            player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 105, 255), stop:1 rgba(200, 0, 0, 200));");
        }
        else
        {
            player.get_infoUI()->setText("Райз");
            player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(100, 0, 0, 255), stop:1 rgba(200, 0, 0, 200));");
        }
    }
    
    _game.addToBank(raisedBet);
    _game.set_raised(true);
    player.set_raised(true);
    player.set_call(true);
    player.set_bet(_game.get_bet() + raisedBet);
    //setChipImage(player.get_chipUI(), player.get_bet());
    chipSingleAnimation(player.get_id());
    updatePlayersChips(player.get_id());
    player.updateBetUI();
    player.updateMoneyUI();
    _game.set_bet(raisedBet + _game.get_bet());
    ui.bankBetValue_lbl->setText(QString::number(_game.get_bet()));
    //updateBankUI();
    setPlayersCallFalse(player.get_id());
    
    //_game.incrTurn(_players.size());
    //playerActionSlot();
    QTimer::singleShot(_chipsDuration, this, SLOT(setImagePlayerChipSlot()));
}

void MainInterface::compCall(Player& player)
{
    if ((player.is_smallBlind() || player.is_bigBlind()) && _game.get_bet() <= _bigBlind)
    {
        player.get_infoUI()->setText("Чек");
        player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 60, 0, 255), stop:1 rgba(0, 143, 0, 200));");
        player.set_call(true);
        _game.incrTurn(_players.size());
        QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
        return;
    }
    int bet = _game.get_bet() - player.get_bet();

    player.takeFromMoney(bet);
    if (player.get_money() <= 0)
    {
        bet += player.get_money();
        player.set_money(0);
        player.set_allIn(true);
    }
    player.set_bet(bet + player.get_bet());
    _game.addToBank(bet);
    player.updateBetUI();
    player.updateMoneyUI();
    //updateBankUI();
    player.set_call(true);

    if (player.is_allIn())
    {
        player.get_infoUI()->setText("Ва-банк");
        player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(90, 0, 90, 255), stop:1 rgba(230, 0, 230, 200));");
    }
    else
    {
        player.get_infoUI()->setText("Колл");
        player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 105, 255), stop:1 rgba(0, 0, 200, 200));");
    }
    
    //setChipImage(player.get_chipUI(), player.get_bet());
    chipSingleAnimation(player.get_id());
    updatePlayersChips(player.get_id());
    //_game.incrTurn(_players.size());
    QTimer::singleShot(_chipsDuration, this, SLOT(setImagePlayerChipSlot()));
    //playerActionSlot();
    /*
    ________________
    int bet = _game.get_bet() - player.get_bet();

    player.takeFromMoney(bet);
    if (player.get_money() <= 0)
    {
        bet += player.get_money();
        player.set_money(0);
        player.set_allIn(true);
    }
    player.set_bet(bet);
    _game.addToBank(bet);
    player.updateBetUI();
    player.updateMoneyUI();
    updateBankUI();
    player.set_call(true);
    if (player.is_allIn())
        player.get_infoUI()->setText("Ва-банк");
    else
        player.get_infoUI()->setText("Колл");
    _game.incrTurn(_players.size());
    playerActionSlot();*/
}

void MainInterface::compFold(Player& player)
{
    player.set_fold(true);
    player.get_card1UI()->clear();
    player.get_card2UI()->clear();
    player.get_infoUI()->setText("Пас");
    player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(90, 90, 90, 255), stop:1 rgba(200, 200, 200, 200));");
    player.get_chipUI()->clear();
    _game.incrTurn(_players.size());
    //playerActionSlot();
    foldAnimation(player.get_id());
    QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
}

void MainInterface::setPlayersCheckFalse()
{
    for (Player& player : _players)
    {
        if (player.is_allIn())
            continue;
        player.set_check(false);
    }
}

void MainInterface::setPlayersCallFalse(int besidesPlayerId)
{
    for (Player& player : _players)
    {
        if (player.get_id() == besidesPlayerId)
            continue;

        player.set_call(false);
    }
}

void MainInterface::setPlayersCallFalse()
{
    for (Player& player : _players)
    {
        if (player.is_allIn())
            continue;
        player.set_call(false);
    }
}

void MainInterface::setEnabledActionBtns(bool value)
{
    ui.check_btn->setVisible(value);
    ui.call_btn->setVisible(value);
    ui.raise_btn->setVisible(value);
    ui.fold_btn->setVisible(value);
}

void MainInterface::setCardImageToTable(int index)
{
    switch (index)
    {
    case 0:
        ui.tableCard_1->setPixmap(QPixmap(_table[0].get_image()));
        break;
    case 1:
        ui.tableCard_2->setPixmap(QPixmap(_table[1].get_image()));
        break;
    case 2:
        ui.tableCard_3->setPixmap(QPixmap(_table[2].get_image()));
        break;
    case 3:
        ui.tableCard_4->setPixmap(QPixmap(_table[3].get_image()));
        break;
    case 4:
        ui.tableCard_5->setPixmap(QPixmap(_table[4].get_image()));
        break;
    }
}

void MainInterface::setDisabledCardOnTable(int index)
{
    switch (index)
    {
    case 0:
        ui.tableCard_1->setEnabled(false);
        break;
    case 1:
        ui.tableCard_2->setEnabled(false);
        break;
    case 2:
        ui.tableCard_3->setEnabled(false);
        break;
    case 3:
        ui.tableCard_4->setEnabled(false);
        break;
    case 4:
        ui.tableCard_5->setEnabled(false);
        break;
    }
}

void MainInterface::setBorderOnTableWinnerCard(int index)
{
    QString border = "border: 4px solid rgb(0, 255, 0); border-radius: 8px;";
    switch (index)
    {
    case 0:
        ui.tableCard_1->setStyleSheet(border);
        break;
    case 1:
        ui.tableCard_2->setStyleSheet(border);
        break;
    case 2:
        ui.tableCard_3->setStyleSheet(border);
        break;
    case 3:
        ui.tableCard_4->setStyleSheet(border);
        break;
    case 4:
        ui.tableCard_5->setStyleSheet(border);
        break;
    }
}

bool MainInterface::checkGameOnAllIn()
{
    int count = 0;
    for (const auto& player : _players)
    {
        if (player.is_fold() || player.is_allIn())
            continue;

        ++count;
    }

    return (count > 1) ? false : true;
}

void MainInterface::showWinner(Player& player)
{
    const std::vector<Card>& winnerCards = player.get_combination().get_hand();
    //player.get_imageUI()->setStyleSheet("border: 2px outset rgb(0, 255, 0); border-radius: 4px;");
    player.get_generalLayout()->setEnabled(true);
    //player.updateMoneyUI();
    player.get_betUI()->clear();
    player.get_chipUI()->clear();
    player.get_infoUI()->clear();
    player.get_infoUI()->setStyleSheet("");
    
    //winAnimation(player.get_id());
    QTimer::singleShot(2000, this, SLOT(winAnimationSlot()));

    if (_game.get_round() != 4)       
        return;
    else
    {
        if (getNotFoldCount() <= 1)
            return;
    }

    for (size_t i = 0; i < _table.size(); ++i)
    {
        bool inWinComb = false;
        for (const auto& card : winnerCards)
        {
            if (_table[i].get_id() != card.get_id())
                continue;
            inWinComb = true;
            break;
        }
        if (inWinComb)
        {
            setBorderOnTableWinnerCard(i);
            continue;
        }
        setDisabledCardOnTable(i);
    }

    for (const auto& card : winnerCards)
    {
        if (player.get_hand()[0].get_id() == card.get_id())
        {
            player.get_card1UI()->setStyleSheet("border: 4px solid rgb(0, 255, 0); border-radius: 8px;");
            player.get_card1UI()->setEnabled(true);
        }
        else if (player.get_hand()[1].get_id() == card.get_id())
        {
            player.get_card2UI()->setStyleSheet("border: 4px solid rgb(0, 255, 0); border-radius: 8px;");
            player.get_card2UI()->setEnabled(true);
        }
    }

    for (auto& player : _players)
    {
        player.get_betUI()->clear();
        if (player.is_fold() || !player.inGame())
            continue;
        player.get_infoUI()->setText(player.get_combination().get_name());
        player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 90, 90, 255), stop:1 rgba(0, 230, 230, 200));");
    }

    ui.message_lbl->setText(_message);
}

void MainInterface::setEnabledBetsBtns(bool value)
{
    ui.allIn_btn->setEnabled(value);
    ui.plus200_btn->setEnabled(value);
    ui.plus100_btn->setEnabled(value);
    ui.plus50_btn->setEnabled(value);
    ui.plus10_btn->setEnabled(value);
    ui.cancelBet_btn->setEnabled(value);
}

void MainInterface::startNewGame()
{
    ui.startGame_btn->setText("Начать игру");
    ui.message_lbl->clear();
    if (_dealToPlayersTimer->isActive())
        _dealToPlayersTimer->stop();
    if (_dealToTableTimer->isActive())
        _dealToTableTimer->stop();
    if (_blindsTimer->isActive())
        _blindsTimer->stop();
    if (_compActionTimer->isActive())
        _compActionTimer->stop();
    ui.deck->clear();
    
    init_players();
    for (auto& player : _players)
    {
        clearPlayerUI(player);
        player.get_moneyUI()->setText(QString::number(player.get_money()));
    }
    
    //std::sort(_players.begin(), _players.end(), [](const Player& p1, const Player& p2) {return p1.get_turn() < p2.get_turn(); });
    clearTable();
    _game.set_round(-1);
    _game.set_bank(0);
    _game.set_bet(0);
    ui.bankBetValue_lbl->setText(QString::number(_game.get_bet()));
    setNameRound("Новая игра");
    setEnabledActionBtns(false);
    //ui.nextRound_btn->setEnabled(false);
    ui.startGame_btn->setEnabled(true);
    ui.leaveTable_btn->setEnabled(true);
    //ui.test_lbl->clear();
}

void MainInterface::nextRound()
{
    chipGroupAnimation();
    _game.incrRound();
    _game.set_bet(0);
    _game.set_raised(false);
    _game.set_turn(1);
    for (Player& player : _players)
        player.set_bet(0);
    
    //ui.nextRound_btn->setEnabled(false);
    switch (_game.get_round())
    {
    case 0:
        //_blindsTimer->start();
        _dealToPlayersTimer->start();
        break;
    case 1:
        if (checkToWin())
            break;
        setNameRound("Флоп");
        for (auto& player : _players)
        {
            if (player.is_smallBlind())
                player.set_smallBlind(false);
            else if (player.is_bigBlind())
                player.set_bigBlind(false);
        }
        ui.bankBetValue_lbl->setText(QString::number(_game.get_bet()));
        setPlayersCheckFalse();
        setPlayersCallFalse();
        _dealToTableTimer->start();
        //dealCardsToTable();
        //_compActionTimer->start();
        break;
    case 2:
        if (checkToWin())
            break;
        setNameRound("Терн");
        ui.bankBetValue_lbl->setText(QString::number(_game.get_bet()));
        setPlayersCheckFalse();
        setPlayersCallFalse();
        _dealToTableTimer->start();
        //dealCardsToTable();
        //_compActionTimer->start();
        break;
    case 3:
        if (checkToWin())
            break;
        setNameRound("Ривер");
        ui.bankBetValue_lbl->setText(QString::number(_game.get_bet()));
        setPlayersCheckFalse();
        setPlayersCallFalse();
        _dealToTableTimer->start();
        //dealCardsToTable();
        //_compActionTimer->start();
        break;
    case 4:
        if (checkToWin())
            break;
        setNameRound("Шоудаун");
        ui.bankBetValue_lbl->setText(QString::number(_game.get_bet()));
        getWinner();
        //ui.nextRound_btn->setEnabled(false);
        break;
    default:
        break;
    }
}

void MainInterface::startGame()
{
    ui.startGame_btn->setText("Играть дальше");
    ui.leaveTable_btn->setEnabled(false);
    ui.message_lbl->clear();
    ui.deck->clear();
    init_deck();
    _game.set_round(-1);
    _game.set_bank(0);
    _game.set_bet(0);
    ui.bankBetValue_lbl->setText(QString::number(_game.get_bet()));
    _game.set_turn(1);
    clearTable();
    cancelPlayers();
    for (auto& player : _players)
    {
        clearPlayerUI(player);
        if (player.get_money() <= 0)
        {
            player.set_inGame(false);
            continue;
        }
        player.get_moneyUI()->setText(QString::number(player.get_money()));
    }
    setNextDealer();
    if (_players.size() == 1)
    {
        bool playerWin;
        if (_players[0].get_id() == 0)
            playerWin = true;
        else
            playerWin = false;
        WinLoseForm winLoseForm(playerWin, _city);
        winLoseForm.setWindowFlags(Qt::FramelessWindowHint);
        if (winLoseForm.exec())
            startNewGame();
        else
            leaveTableSlot();
        ui.leaveTable_btn->setEnabled(true);
        return;
    }
    //setDealerImage();
    setTurnsToDeal(1);
    _blindsTimer->start();
    ui.round_lbl->setText("Раздача карт");
    ui.startGame_btn->setEnabled(false);
}

void MainInterface::increaseBet(int value)
{
    int currValue = ui.betVSlider->value();
    currValue += value;
    ui.betVSlider->setValue(currValue);
}

void MainInterface::saveData()
{
    QSettings settings("Goose", "Poker");
    int money = 1000;
    if (const Player* player = findPlayer(0))
    {
        if (player->get_money() > 1000)
            money = player->get_money();
    }

    settings.beginGroup("game");
    settings.setValue("city", _city);
    settings.setValue("deposit", money);
    settings.endGroup();
    settings.beginGroup("settings");
    settings.setValue("cardImage", _cardImage);
    settings.setValue("shirtImage", _shirtImage);
    settings.setValue("dealDelay", _dealDelay);
    settings.setValue("chipsDuration", _chipsDuration);
    settings.setValue("playerName", _playerName);
    //settings.setValue("soundVolume", _soundPlayer->volume());
}

void MainInterface::updateSettings()
{
    QSettings settings("Goose", "Poker");
    settings.beginGroup("settings");
    _cardImage = settings.value("cardImage", 1).toInt();
    _shirtImage = settings.value("shirtImage", 1).toInt();
    _dealDelay = settings.value("dealDelay", 500).toInt();
    _chipsDuration = settings.value("chipsDuration", 400).toInt();
    _playerName = settings.value("playerName", "Игрок").toString();

    _dealToPlayersTimer->setInterval(_dealDelay);
    _dealToTableTimer->setInterval(_dealDelay);
    _blindsTimer->setInterval(_compActionDelay);
    _compActionTimer->setInterval(_compActionDelay);
    set_shirtImage(_shirtImage);
    set_cardImage(_cardImage);
    ui.card_1->setPixmap(QPixmap(_shirtImageStr));
    ui.card_2->setPixmap(QPixmap(_shirtImageStr));

    init_cards(_cardImage);
    
    for (auto& player : _players)
    {
        if (player.get_id() == 0)
        {
            player.set_name(_playerName);
            player.get_nameUI()->setText(_playerName);
        }
        if (player.is_fold())
            continue;
        int card1Id = -1;
        int card2Id = -1;
        if (player.get_hand().size() == 1)
            card1Id = player.get_hand()[0].get_id();
        else if (player.get_hand().size() == 2)
        {
            card1Id = player.get_hand()[0].get_id();
            card2Id = player.get_hand()[1].get_id();
        }
        if (!player.get_hand().empty())
            player.clearHand();
        if (card1Id >= 0)
            player.addCardToHand(findCard(card1Id));
        if (card2Id >= 0)
            player.addCardToHand(findCard(card2Id));
        if (player.get_id() == 0 || player.is_showdown())
        {
            player.setCardImage(0);
            player.setCardImage(1);
        }
        else
        {
            player.setCardShirtImage(0);
            player.setCardShirtImage(1);
        }
    }

    if (ui.deck->pixmap())
        ui.deck->setPixmap(QPixmap(_shirtImageStr));

    std::vector<int> tableCardsIds;
    tableCardsIds.reserve(5);
    for (const auto& card : _table)
        tableCardsIds.push_back(std::move(card.get_id()));
    _table.clear();
    for (size_t i = 0; i < tableCardsIds.size(); ++i)
    {
        _table.push_back(std::move(findCard(tableCardsIds[i])));
        setCardImageToTable(i);
    }
}

void MainInterface::updatePlayersChips(int playerId)
{
    Player* player = findPlayer(playerId);
    int playerMoney = player->get_money();
    if (playerMoney > 0 && playerMoney <= 100)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_1.png"));
    else if (playerMoney > 100 && playerMoney <= 500)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_2.png"));
    else if (playerMoney > 500 && playerMoney <= 1000)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_3.png"));
    else if (playerMoney > 1000 && playerMoney <= 10000)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_4.png"));
    else if (playerMoney > 10000 && playerMoney <= 3000)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_5.png"));
    else if (playerMoney > 3000 && playerMoney <= 5000)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_6.png"));
    else if (playerMoney > 5000 && playerMoney <= 10000)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_7.png"));
    else if (playerMoney > 10000 && playerMoney <= 30000)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_8.png"));
    else if (playerMoney > 30000 && playerMoney <= 50000)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_9.png"));
    else if (playerMoney > 50000 && playerMoney <= 100000)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_10.png"));
    else if (playerMoney > 100000 && playerMoney <= 200000)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_11.png"));
    else if (playerMoney > 200000 && playerMoney <= 400000)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_12.png"));
    else if (playerMoney > 400000 && playerMoney <= 500000)
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_13.png"));
    else if (playerMoney > 500000 )
        player->get_moneyChipsUI()->setPixmap(QPixmap("imgs/plrChips_14.png"));
    else
        player->get_moneyChipsUI()->clear();
}

void MainInterface::setVisibleFoldCards(bool value)
{
    ui.card_1->setVisible(value);
    ui.card_2->setVisible(value);
}

void MainInterface::testBtnSlot()
{
    //QMessageBox::information(this, "test", "test");
    //init_players();
    //ui.plrCard_1->setText(getCardFromDeck(random).get_image());
    //QTimer::singleShot(_chipsDuration, this, SLOT(setBankImageSlot()));
    if (Player* player = findPlayer(0))
    {
        player->set_money(0);
        player->updateMoneyUI();
    }
}

void MainInterface::setData()
{
    //QSettings settings(QDir::currentPath() + "/settings.ini", QSettings::IniFormat);
    QSettings settings("Goose", "Poker");
    settings.beginGroup("game");
    _city = settings.value("city", 1).toInt();
    _plrDeposit = settings.value("deposit", 1000).toInt();
    settings.endGroup();
    settings.beginGroup("settings");
    _cardImage = settings.value("cardImage", 1).toInt();
    _shirtImage = settings.value("shirtImage", 1).toInt();
    _dealDelay = settings.value("dealDelay", 500).toInt();
    _chipsDuration = settings.value("chipsDuration", 400).toInt();
    _playerName = settings.value("playerName", "Игрок").toString();
    //_soundPlayer->setVolume(settings.value("soundVolume", 50).toInt());

    _dealToPlayersTimer->setInterval(_dealDelay);
    _dealToTableTimer->setInterval(_dealDelay);
    _blindsTimer->setInterval(_compActionDelay);
    _compActionTimer->setInterval(_compActionDelay);
    set_shirtImage(_shirtImage);
    set_cardImage(_cardImage);
    ui.card_1->setPixmap(QPixmap(_shirtImageStr));
    ui.card_2->setPixmap(QPixmap(_shirtImageStr));

    init_cards(_cardImage);
    switch (_city)
    {
    case 1:
        _deposit = 1000;
        _bigBlind = 100;
        ui.currCity_lbl->setText("Сочи");
        _background = "imgs/sochiFon.png";
        break;
    case 2:
        _deposit = 5000;
        _bigBlind = 500;
        ui.currCity_lbl->setText("Сингапур");
        _background = "imgs/singaporeFon.png";
        break;
    case 3:
        _deposit = 30000;
        _bigBlind = 3000;
        ui.currCity_lbl->setText("Макао");
        _background = "imgs/makaoFon.png";
        break;
    case 4:
        _deposit = 100000;
        _bigBlind = 10000;
        ui.currCity_lbl->setText("Лас-Вегас");
        _background = "imgs/lasVegasFon.png";
        break;
    }

    setBackground();
    ui.plus10_btn->setText("+" + QString::number(_bigBlind / 10) + " $");
    ui.plus50_btn->setText("+" + QString::number(_bigBlind / 2) + " $");
    ui.plus100_btn->setText("+" + QString::number(_bigBlind) + " $");
    ui.plus200_btn->setText("+" + QString::number(_bigBlind * 2) + " $");
    ui.betVSlider->setTickInterval(_bigBlind / 10 * 5);
    ui.setBetFrame->setVisible(false);
    ui.combsImage_lbl->clear();

    startNewGame();
}


void MainInterface::setChipImage(QLabel* label, const int bet)
{
    if (!label)
        return;

    if (bet <= 100)
        label->setPixmap(QPixmap("imgs/chip1.png"));
    else if (bet > 100 && bet <= 1000)
        label->setPixmap(QPixmap("imgs/chip2.png"));
    else if (bet > 1000 && bet <= 5000)
        label->setPixmap(QPixmap("imgs/chip3.png"));
    else if (bet > 5000 && bet <= 10000)
        label->setPixmap(QPixmap("imgs/chip4.png"));
    else if (bet > 10000 && bet <= 50000)
        label->setPixmap(QPixmap("imgs/chip5.png"));
    else if (bet > 50000)
        label->setPixmap(QPixmap("imgs/chip6.png"));
    else
        label->setPixmap(QPixmap("imgs/chip1.png"));

}
void MainInterface::dealToPlayersAnimation(int recipientId, int cardNumber)
{
    ui.card_1->setVisible(true);
    QPropertyAnimation* _dealAnimation = new QPropertyAnimation(ui.card_1, "geometry", this);
    _dealAnimation->setDuration(_dealDelay - 100);
    Player* dealer = findDealer();
    Player* recipient = findPlayer(recipientId);

    QRect startRect(dealer->get_generalLayout()->geometry());
    
    if (dealer->get_id() == 0)
    {
        startRect.setX(ui.playerGeneralFrame->geometry().x() + dealer->get_generalLayout()->geometry().x() + dealer->get_dealerUI()->geometry().x() + 55);
        startRect.setY(ui.playerGeneralFrame->geometry().y() + dealer->get_generalLayout()->geometry().y() + dealer->get_dealerUI()->geometry().y() + 105);
    }
    else
    {
        startRect.setX(startRect.x() + dealer->get_imageUI()->geometry().x());
        startRect.setY(startRect.y() + dealer->get_imageUI()->geometry().y());
    }

    QRect endRect(recipient->get_generalLayout()->geometry());

    if (recipient->get_id() == 0)
    {
        endRect.setX(ui.playerGeneralFrame->geometry().x() + recipient->get_generalLayout()->geometry().x());
        endRect.setY(ui.playerGeneralFrame->geometry().y() + recipient->get_generalLayout()->geometry().y());
    }

    switch (cardNumber)
    {
    case 0:
        endRect.setX(endRect.x() + recipient->get_card1UI()->geometry().x());
        endRect.setY(endRect.y() + recipient->get_card1UI()->geometry().y());
        break;
    case 1:
        endRect.setX(endRect.x() + recipient->get_card2UI()->geometry().x());
        endRect.setY(endRect.y() + recipient->get_card2UI()->geometry().y());
        break;
    }
    

    _dealAnimation->setStartValue(startRect);
    _dealAnimation->setEndValue(endRect);
    //_dealAnimation->setEasingCurve(QEasingCurve::InOutCirc);
    //_dealAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    _dealAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainInterface::dealToTableAnimation(int cardNumber)
{
    ui.card_1->setVisible(true);
    QPropertyAnimation* _dealAnimation = new QPropertyAnimation(ui.card_1, "geometry", this);
    _dealAnimation->setDuration(_dealDelay -120);
    QRect startRect(ui.tableCardsFrame->geometry());
    startRect.setX(startRect.x() + ui.deck->geometry().x());

    QRect endRect(ui.tableCardsFrame->geometry());
    switch (cardNumber)
    {
    case 1:
        endRect.setX(endRect.x() + ui.tableCard_1->geometry().x());
        endRect.setY(endRect.y() + ui.tableCard_1->geometry().y());
        break;
    case 2:
        endRect.setX(endRect.x() + ui.tableCard_2->geometry().x());
        endRect.setY(endRect.y() + ui.tableCard_2->geometry().y());
        break;
    case 3:
        endRect.setX(endRect.x() + ui.tableCard_3->geometry().x());
        endRect.setY(endRect.y() + ui.tableCard_3->geometry().y());
        break;
    case 4:
        endRect.setX(endRect.x() + ui.tableCard_4->geometry().x());
        endRect.setY(endRect.y() + ui.tableCard_4->geometry().y());
        break;
    case 5:
        endRect.setX(endRect.x() + ui.tableCard_5->geometry().x());
        endRect.setY(endRect.y() + ui.tableCard_5->geometry().y());
        break;
    }
    
    _dealAnimation->setStartValue(startRect);
    _dealAnimation->setEndValue(endRect);
    _dealAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainInterface::chipSingleAnimation(int playerId)
{
    Player* player = findPlayer(playerId);
    QLabel* chip = findChipLbl(playerId);
    setChipImage(chip, player->get_bet());
    updatePlayersChips(playerId);
    chip->setVisible(true);

    QPropertyAnimation* animation;

    switch (playerId)
    {
    case 0:
        animation = new QPropertyAnimation(ui.chip_5, "geometry", this);
        break;
    case 1:
        animation = new QPropertyAnimation(ui.chip_1, "geometry", this);
        break;
    case 2:
        animation = new QPropertyAnimation(ui.chip_2, "geometry", this);
        break;
    case 3:
        animation = new QPropertyAnimation(ui.chip_3, "geometry", this);
        break;
    case 4:
        animation = new QPropertyAnimation(ui.chip_4, "geometry", this);
        break;
    }

    animation->setDuration(_chipsDuration);
    
    QRect startRect(player->get_generalLayout()->geometry());
    startRect.setX(startRect.x() + player->get_moneyChipsUI()->geometry().x() + player->get_moneyChipsUI()->geometry().width() / 2);
    startRect.setY(startRect.y() + player->get_moneyChipsUI()->geometry().y() + player->get_moneyChipsUI()->geometry().height() / 2);

    if (player->get_id() == 0)
    {
        startRect.setX(ui.playerGeneralFrame->geometry().x() + player->get_generalLayout()->geometry().x() + player->get_moneyChipsUI()->geometry().x());
        startRect.setY(ui.playerGeneralFrame->geometry().y() + player->get_generalLayout()->geometry().y() + player->get_moneyChipsUI()->geometry().y());
    }

    QRect endRect(player->get_generalLayout()->geometry());
    endRect.setX(endRect.x() + player->get_chipUI()->geometry().x());
    endRect.setY(endRect.y() + player->get_chipUI()->geometry().y());

    if (player->get_id() == 0)
    {
        endRect.setX(ui.playerGeneralFrame->geometry().x() + player->get_generalLayout()->geometry().x() + player->get_chipUI()->geometry().x());
        endRect.setY(ui.playerGeneralFrame->geometry().y() + player->get_generalLayout()->geometry().y() + player->get_chipUI()->geometry().y());
    }

    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    //_dealAnimation->setEasingCurve(QEasingCurve::InOutCirc);
    //_dealAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainInterface::chipGroupAnimation()
{
    QPropertyAnimation* _comp1Animation = new QPropertyAnimation(ui.chip_1, "geometry", this);
    QPropertyAnimation* _comp2Animation = new QPropertyAnimation(ui.chip_2, "geometry", this);
    QPropertyAnimation* _comp3Animation = new QPropertyAnimation(ui.chip_3, "geometry", this);
    QPropertyAnimation* _comp4Animation = new QPropertyAnimation(ui.chip_4, "geometry", this);
    QPropertyAnimation* _playerAnimation = new QPropertyAnimation(ui.chip_5, "geometry", this);
    QParallelAnimationGroup* _groupChipsAnimation = new QParallelAnimationGroup(this);

    _groupChipsAnimation->addAnimation(_comp1Animation);
    _groupChipsAnimation->addAnimation(_comp2Animation);
    _groupChipsAnimation->addAnimation(_comp3Animation);
    _groupChipsAnimation->addAnimation(_comp4Animation);
    _groupChipsAnimation->addAnimation(_playerAnimation);

    QRect endRect(ui.bankFrame->geometry());
    endRect.setX(endRect.x() + ui.bankImage_lbl->geometry().x() + 15);
    endRect.setY(endRect.y() + ui.bankImage_lbl->geometry().y() + 15);


    for (auto& player : _players)
    {
        if (player.get_bet() <= 0 || player.is_fold() || !player.inGame())
            continue;
        player.get_chipUI()->clear();
        QLabel* chip = findChipLbl(player.get_id());
        setChipImage(chip, player.get_bet());
        chip->setVisible(true);
        QRect startRect(player.get_generalLayout()->geometry());
        startRect.setX(startRect.x() + player.get_chipUI()->geometry().x());
        startRect.setY(startRect.y() + player.get_chipUI()->geometry().y());
        if (player.get_id() == 0)
        {
            startRect.setX(ui.playerGeneralFrame->geometry().x() + player.get_generalLayout()->geometry().x() + player.get_moneyChipsUI()->geometry().x());
            startRect.setY(ui.playerGeneralFrame->geometry().y() + player.get_generalLayout()->geometry().y() + player.get_moneyChipsUI()->geometry().y());
        }

        switch (player.get_id())
        {
        case 0:
            _playerAnimation->setDuration(_chipsDuration);
            _playerAnimation->setStartValue(startRect);
            _playerAnimation->setEndValue(endRect);
            break;
        case 1:
            _comp1Animation->setDuration(_chipsDuration);
            _comp1Animation->setStartValue(startRect);
            _comp1Animation->setEndValue(endRect);
            break;
        case 2:
            _comp2Animation->setDuration(_chipsDuration);
            _comp2Animation->setStartValue(startRect);
            _comp2Animation->setEndValue(endRect);
            break;
        case 3:
            _comp3Animation->setDuration(_chipsDuration);
            _comp3Animation->setStartValue(startRect);
            _comp3Animation->setEndValue(endRect);
            break;
        case 4:
            _comp4Animation->setDuration(_chipsDuration);
            _comp4Animation->setStartValue(startRect);
            _comp4Animation->setEndValue(endRect);
            break;
        }
    }

    _groupChipsAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(_chipsDuration, this, SLOT(setBankImageSlot()));
}

void MainInterface::setVisibleChips(bool value)
{
    ui.chip_1->setVisible(value);
    ui.chip_2->setVisible(value);
    ui.chip_3->setVisible(value);
    ui.chip_4->setVisible(value);
    ui.chip_5->setVisible(value);
}

void MainInterface::foldAnimation(const int playerId)
{
    ui.card_1->setVisible(true);
    ui.card_2->setVisible(true);
    QPropertyAnimation* _cardAnimation_1 = new QPropertyAnimation(ui.card_1, "geometry", this);
    QPropertyAnimation* _cardAnimation_2 = new QPropertyAnimation(ui.card_2, "geometry", this);
    QParallelAnimationGroup* _groupCardsAnimation = new QParallelAnimationGroup(this);
    _groupCardsAnimation->addAnimation(_cardAnimation_1);
    _groupCardsAnimation->addAnimation(_cardAnimation_2);
    _cardAnimation_1->setDuration(450);
    _cardAnimation_2->setDuration(400);
    Player* player = findPlayer(playerId);
    QRect startRect_1(player->get_generalLayout()->geometry());
    QRect startRect_2(player->get_generalLayout()->geometry());

    startRect_1.setX(startRect_1.x() + player->get_card1UI()->geometry().x());
    startRect_1.setY(startRect_1.y() + player->get_card1UI()->geometry().y());
    startRect_2.setX(startRect_2.x() + player->get_card2UI()->geometry().x());
    startRect_2.setY(startRect_2.y() + player->get_card2UI()->geometry().y());

    if (player->get_id() == 0)
    {
        startRect_1.setX(ui.playerGeneralFrame->geometry().x() + player->get_generalLayout()->geometry().x() + player->get_card1UI()->geometry().x());
        startRect_1.setY(ui.playerGeneralFrame->geometry().y() + player->get_generalLayout()->geometry().y() + player->get_card1UI()->geometry().y());
        startRect_2.setX(ui.playerGeneralFrame->geometry().x() + player->get_generalLayout()->geometry().x() + player->get_card2UI()->geometry().x());
        startRect_2.setY(ui.playerGeneralFrame->geometry().y() + player->get_generalLayout()->geometry().y() + player->get_card2UI()->geometry().y());
    }
    else
    {
        startRect_1.setX(startRect_1.x() + 35);
        startRect_1.setY(startRect_1.y() + 10);
        startRect_2.setX(startRect_2.x() + 35);
        startRect_2.setY(startRect_2.y() + 10);
    }

    QRect endRect(geometry());
    if (player->get_id() == 1 || player->get_id() == 2)
    {
        endRect.setX(endRect.x() - 150);
        endRect.setY(endRect.y() + geometry().height() + 100);
    }
    else
    {
        endRect.setX(endRect.x() + geometry().width() + 100);
        endRect.setY(endRect.y() + geometry().height());
    }
    
    _cardAnimation_1->setStartValue(startRect_1);
    _cardAnimation_1->setEndValue(endRect);

    _cardAnimation_2->setStartValue(startRect_2);
    _cardAnimation_2->setEndValue(endRect);

    _cardAnimation_1->setEasingCurve(QEasingCurve::InOutCirc);
    _cardAnimation_2->setEasingCurve(QEasingCurve::InOutCubic);
    _groupCardsAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    //QAbstractAnimation::DeleteWhenStopped
    QTimer::singleShot(450, this, SLOT(endFoldAnimationSlot()));
}

void MainInterface::winAnimation(const int playerId)
{
    Player* winner = findPlayer(playerId);
    int duration = 500;
    setBankChipsImage();
    ui.bankChips->setVisible(true);    
    //updateBankUI();
    QPropertyAnimation* _bankChipAnimation = new QPropertyAnimation(ui.bankChips, "geometry", this);
    _bankChipAnimation->setDuration(_chipsDuration + duration);
    ui.bankImage_lbl->clear();
    ui.bankValue_lbl->clear();

    QRect startRect(ui.bankFrame->geometry());
    startRect.setX(startRect.x() + ui.bankImage_lbl->geometry().x());
    startRect.setY(startRect.y() + ui.bankImage_lbl->geometry().y());

    QRect endRect(winner->get_generalLayout()->geometry());
    endRect.setX(endRect.x() + winner->get_moneyChipsUI()->geometry().x());
    endRect.setY(endRect.y() + winner->get_moneyChipsUI()->geometry().y());

    if (winner->get_id() == 0)
    {
        endRect.setX(ui.playerGeneralFrame->geometry().x() + winner->get_generalLayout()->geometry().x() + winner->get_moneyChipsUI()->geometry().x());
        endRect.setY(ui.playerGeneralFrame->geometry().y() + winner->get_generalLayout()->geometry().y() + winner->get_moneyChipsUI()->geometry().y());
    }
    endRect.setX(endRect.x() - 10);
    endRect.setY(endRect.y() - 10);
    _bankChipAnimation->setStartValue(startRect);
    _bankChipAnimation->setEndValue(endRect);
    _bankChipAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(_chipsDuration + duration, this, SLOT(endWinAnimationSlot()));
}

void MainInterface::dealerChipAnimation(const int previousDealerId, const int nextDealerId)
{
    Player* previousDealer = findPlayer(previousDealerId);
    Player* nextDealer = findPlayer(nextDealerId);
    previousDealer->get_dealerUI()->clear();
    QPropertyAnimation* _dealerChipAnimation = new QPropertyAnimation(ui.dealerChip, "geometry", this);
    ui.dealerChip->setVisible(true);
    _dealerChipAnimation->setDuration(_dealerChipDuration);

    QRect startRect(previousDealer->get_generalLayout()->geometry());
    QRect endRect(nextDealer->get_generalLayout()->geometry());

    if (previousDealerId == 0)
    {
        startRect.setX(ui.playerGeneralFrame->geometry().x() + previousDealer->get_generalLayout()->geometry().x() + previousDealer->get_dealerUI()->geometry().x());
        startRect.setY(ui.playerGeneralFrame->geometry().y() + previousDealer->get_generalLayout()->geometry().y() + previousDealer->get_dealerUI()->geometry().y());
    }
    else
    {
        startRect.setX(startRect.x() + previousDealer->get_dealerUI()->geometry().x());
        startRect.setY(startRect.y() + previousDealer->get_dealerUI()->geometry().y());
    }

    if (nextDealerId == 0)
    {
        endRect.setX(ui.playerGeneralFrame->geometry().x() + nextDealer->get_generalLayout()->geometry().x() + nextDealer->get_dealerUI()->geometry().x());
        endRect.setY(ui.playerGeneralFrame->geometry().y() + nextDealer->get_generalLayout()->geometry().y() + nextDealer->get_dealerUI()->geometry().y());
    }
    else
    {
        endRect.setX(endRect.x() + nextDealer->get_dealerUI()->geometry().x());
        endRect.setY(endRect.y() + nextDealer->get_dealerUI()->geometry().y());
    }
    
    _dealerChipAnimation->setStartValue(startRect);
    _dealerChipAnimation->setEndValue(endRect);
    //_dealerChipAnimation->setEasingCurve(QEasingCurve::Linear);
    _dealerChipAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(_dealerChipDuration, this, SLOT(setImageDealerSlot()));
}

void MainInterface::setBankChipsImage()
{
    if (_game.get_bank() <= 1000)
        ui.bankChips->setPixmap(QPixmap("imgs/bankChips_1.png"));
    else if (_game.get_bank() > 1000 && _game.get_bank() <= 3000)
        ui.bankChips->setPixmap(QPixmap("imgs/bankChips_2.png"));
    else if (_game.get_bank() > 3000 && _game.get_bank() <= 5000)
        ui.bankChips->setPixmap(QPixmap("imgs/bankChips_3.png"));
    else if (_game.get_bank() > 5000 && _game.get_bank() <= 10000)
        ui.bankChips->setPixmap(QPixmap("imgs/bankChips_4.png"));
    else if (_game.get_bank() > 10000 && _game.get_bank() <= 30000)
        ui.bankChips->setPixmap(QPixmap("imgs/bankChips_5.png"));
    else if (_game.get_bank() > 30000 && _game.get_bank() <= 50000)
        ui.bankChips->setPixmap(QPixmap("imgs/bankChips_6.png"));
    else if (_game.get_bank() > 50000 && _game.get_bank() <= 100000)
        ui.bankChips->setPixmap(QPixmap("imgs/bankChips_7.png"));
    else if (_game.get_bank() > 100000 && _game.get_bank() <= 200000)
        ui.bankChips->setPixmap(QPixmap("imgs/bankChips_8.png"));
    else if (_game.get_bank() > 200000 && _game.get_bank() <= 400000)
        ui.bankChips->setPixmap(QPixmap("imgs/bankChips_9.png"));
    else if (_game.get_bank() > 400000 && _game.get_bank() <= 500000)
        ui.bankChips->setPixmap(QPixmap("imgs/bankChips_10.png"));
    else if (_game.get_bank() > 500000)
        ui.bankChips->setPixmap(QPixmap("imgs/bankChips_11.png"));
}

int MainInterface::getNotFoldCount()
{
    int notFoldCount = 0;
    for (const auto& player : _players)
    {
        if (!player.is_fold())
            ++notFoldCount;
    }
    return notFoldCount;
}

void MainInterface::clearPlayerImageBorder()
{
    for (auto& player : _players)
        player.get_nameUI()->setStyleSheet("");
}

const QString MainInterface::getTurnBorder()
{
    QString border = "border: 1px solid rgb(0, 255, 0, 200); border-radius: 3px;";
    return border;
}

void MainInterface::dealCardsToPlayersSlot()
{
    std::sort(_players.begin(), _players.end(), [](const Player& p1, const Player& p2) {return p1.get_turn() < p2.get_turn(); });
    int highest = _deck.size() - 1;
    for (int i = 0; i < 2; ++i)
    {
        for (Player& player : _players)
        {
            if (!player.inGame() || player.get_hand().size() > i)
                continue;
            int random = QRandomGenerator::global()->bounded(0, highest);
            player.addCardToHand(getCardFromDeck(random));
            //player.setCardImage(i);
            dealToPlayersAnimation(player.get_id(), i);
            QTimer::singleShot(_dealDelay, this, SLOT(setImagePlayerCardSlot()));
            return;
        }
    }

    ui.card_1->setVisible(false);
    ui.deck->setPixmap(QPixmap(_shirtImageStr));
    _dealToPlayersTimer->stop();
    setNameRound("Префлоп");
    std::sort(_players.begin(), _players.end(), [](const Player& p1, const Player& p2) {return p1.get_turn() < p2.get_turn(); });
    _game.set_turn(3);
    _game.set_bet(_bigBlind);
    ui.bankBetValue_lbl->setText(QString::number(_game.get_bet()));
    _game.set_raised(true);
    //playerActionSlot();
    QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
}

void MainInterface::dealCardsToTableSlot()
{
    int highest = _deck.size() - 1;
    for (int i = 0; i < 6; ++i)
    {
        if (_table.size() > i)
            continue;
        int random = QRandomGenerator::global()->bounded(0, highest);
        _table.push_back(std::move(getCardFromDeck(random)));
        //setCardImageToTable(i);
        dealToTableAnimation(i+1);
        QTimer::singleShot(_dealDelay, this, SLOT(setImageTableCardSlot()));

        if ((_game.get_round() == 1 && _table.size() == 3) || 
            (_game.get_round() == 2 && _table.size() == 4) ||
            (_game.get_round() == 3 && _table.size() == 5))
        {
            for (Player& player : _players)
            {
                player.set_bet(0);
                player.get_betUI()->clear();
                if (player.is_fold() || player.is_allIn())
                    continue;                
                player.get_infoUI()->clear(); 
                player.get_infoUI()->setStyleSheet("");
                player.get_chipUI()->clear();
            }
            _dealToTableTimer->stop();
            //playerActionSlot();
            QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
            QTimer::singleShot(_dealDelay, this, SLOT(endFoldAnimationSlot()));
        }
        return;
    }
}

void MainInterface::getBlindsSlot()
{
    int smallBlind = _bigBlind / 2;
    for (Player& player : _players)
    {
        if (player.is_smallBlind() && player.get_bet() == 0)
        //if (player.is_smallBlind())
        {
            player.takeFromMoney(smallBlind);
            if (player.get_money() <= 0)
            {
                smallBlind += player.get_money();
                player.set_money(0);
                player.set_allIn(true);
            }
            _game.addToBank(smallBlind);
            player.set_bet(smallBlind);
            player.updateBetUI();
            player.updateMoneyUI();
            player.get_infoUI()->setText("Малый блайнд");
            player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 90, 90, 255), stop:1 rgba(0, 230, 230, 200));");
            //player.set_check(true);
            //player.set_call(true);
            //player.set_smallBlind(false);
            //updateBankUI();
            _game.incrTurn(_players.size());
            chipSingleAnimation(player.get_id());
            updatePlayersChips(player.get_id());
            return;
        }
        else if (player.is_bigBlind() && player.get_bet() == 0)
        //else if (player.is_bigBlind())
        {
            int bigBlind = _bigBlind;
            player.takeFromMoney(bigBlind);
            if (player.get_money() <= 0)
            {
                bigBlind += player.get_money();
                player.set_money(0);
                player.set_allIn(true);
            }
            _game.addToBank(bigBlind);
            _game.set_raised(true);
            _game.set_bet(bigBlind);
            ui.bankBetValue_lbl->setText(QString::number(_game.get_bet()));
            player.set_bet(bigBlind);
            player.updateBetUI();
            player.updateMoneyUI();
            player.get_infoUI()->setText("Большой блайнд");
            player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 90, 90, 255), stop:1 rgba(0, 230, 230, 200));");
            //player.set_check(true);
            //player.set_call(true);
            //player.set_bigBlind(false);
            player.set_bet(bigBlind);
            //updateBankUI();
            _game.incrTurn(_players.size());
            chipSingleAnimation(player.get_id());
            updatePlayersChips(player.get_id());
            return;
        }
    }

    _blindsTimer->stop();
    nextRound();
}

void MainInterface::playerActionSlot()
{
    setEnabledActionBtns(false);
    clearPlayerImageBorder();
    //ui.card_1->setVisible(false);
    //ui.test_lbl->setText("Очередь хода: " + QString::number(_game.get_turn()));
    /*if (checkGameOnAllIn())
    {
        nextRound();
    }*/

    for (Player& player : _players)
    {
        if (player.is_allIn())
        {
            player.get_infoUI()->setText("Ва-банк");
            player.get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(90, 0, 90, 255), stop:1 rgba(230, 0, 230, 200));");
        }

        if (player.get_turn() != _game.get_turn())
            continue;

        if (player.get_id() == 0)
        {
            if (player.is_fold())
            {
                _game.incrTurn(_players.size());
                playerActionSlot();
                //QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
                return;
            }

            if (checkGameOnAllIn() && !_game.is_raised())
            {
                player.set_check(true);
                _game.incrTurn(_players.size());
                playerActionSlot();
                //QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
                return;
            }

            if (player.is_allIn())
            {
                if (checkGameOnAllIn())
                {
                    nextRound();
                    return;
                }
                else
                {
                    player.set_check(true);
                    player.set_call(true);
                    _game.incrTurn(_players.size());
                    playerActionSlot();
                    //QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
                    return;
                }
            }

            if (_game.is_raised())
            {
                if (player.is_call())
                {
                    nextRound();
                    //ui.nextRound_btn->setEnabled(true);
                    return;
                }
                player.get_nameUI()->setStyleSheet(getTurnBorder());
                if (checkGameOnAllIn())
                {
                    ui.fold_btn->setVisible(true);
                    ui.call_btn->setVisible(true);
                    
                }
                else if (player.is_bigBlind() || player.is_smallBlind())
                {
                    ui.fold_btn->setVisible(true);
                    ui.check_btn->setVisible(true);
                    if (!checkGameOnAllIn())
                        ui.raise_btn->setVisible(true);
                }
                else
                {
                    setEnabledActionBtns(true);
                    ui.check_btn->setVisible(false);
                    if (player.get_money() - _game.get_bet() + player.get_bet() < 0)
                        ui.raise_btn->setVisible(false);
                }
            }   
            else if (player.is_check() || checkGameOnAllIn())
            {
                nextRound();
                return;
            }
            else 
            {
                setEnabledActionBtns(true);
                player.get_nameUI()->setStyleSheet(getTurnBorder());
                ui.call_btn->setVisible(false);
            }
            return;
        }

        if (player.is_fold())
        {
            _game.incrTurn(_players.size());
            playerActionSlot();
            //QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
            return;
        }

        if (player.is_allIn())
        {
            if (checkGameOnAllIn())
            {
                nextRound();
                return;
            }

            /*if (_game.is_raised() && player.is_call())
            {
                qDebug() << "_game.is_raised() && player.is_call()";
                nextRound();
                return;
            }

            if (!_game.is_raised() && player.is_check())
            {
                qDebug() << "_game.is_raised() && player.is_call()";
                nextRound();
                return;
            }*/

            player.set_check(true);
            player.set_call(true);
            _game.incrTurn(_players.size());
            playerActionSlot();
            return;
        }

        if (_game.is_raised() && player.is_call())
        {
            nextRound();
            return;
        }

        if (!_game.is_raised() && player.is_check())
        {
            nextRound();
            return;
        }

        

        player.get_infoUI()->setText("...");
        player.get_infoUI()->setStyleSheet("");
        player.get_nameUI()->setStyleSheet(getTurnBorder());
        QTimer::singleShot(_compActionDelay, this, SLOT(compActionSlot()));
        return;
    }
    nextRound();
}

void MainInterface::compActionSlot()
{
    for (Player& player : _players)
    {
        if (player.get_turn() != _game.get_turn())
            continue;

        compAction(player);
        return;
    }
}

void MainInterface::checkBtnSlot()
{
    Player* player = findPlayer(0);
    player->set_check(true);
    player->get_infoUI()->setText("Чек");
    player->get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 60, 0, 255), stop:1 rgba(0, 143, 0, 200));");
    _game.incrTurn(_players.size());
    setEnabledActionBtns(false);
    //playerActionSlot();
    QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
}

void MainInterface::setBetSliderSlot(int value)
{
    value = value * 10;
    Player* player = findPlayer(0);
    int money = player->get_money();
    int playerBet = player->get_bet();
    money -= value;
    ui.raisingBet_lbl->setText(QString::number(value + playerBet));
    ui.maxBet_lbl->setText(QString::number(money-_game.get_bet() - playerBet));
    if (value > 0)
        ui.betOk_btn->setEnabled(true);
    else
        ui.betOk_btn->setEnabled(false);
}

void MainInterface::raiseBtnSlot(bool checked)
{
    if (checked)
    {
        Player* player = findPlayer(0);
        int money = player->get_money();
        int playerBet = player->get_bet();
        //money -= _game.get_bet() + bet;
        money = money - _game.get_bet() + playerBet;
        ui.check_btn->setVisible(false);
        ui.call_btn->setVisible(false);
        ui.fold_btn->setVisible(false);
        ui.betVSlider->setVisible(true);
        ui.betVSlider->setMaximum(money / 10);
        ui.betVSlider->setTickInterval(ui.betVSlider->maximum()/4);
        ui.maxBet_lbl->setText(QString::number(money));
        ui.raisingBet_lbl->setText("0");
        ui.betVSlider->setValue(0);
        setEnabledBetsBtns(true);
        ui.setBetFrame->setVisible(true);
    }
    else
    {
        if (_game.is_raised())
            ui.call_btn->setVisible(true);
        else
            ui.check_btn->setVisible(true);
        ui.fold_btn->setVisible(true);
        ui.betVSlider->setValue(0);
        ui.betOk_btn->setEnabled(false);
        ui.maxBet_lbl->clear();
        ui.raisingBet_lbl->clear();
        setEnabledBetsBtns(false);
        ui.setBetFrame->setVisible(false);
    }
}

void MainInterface::callBtnSlot()
{
    setEnabledActionBtns(false);
    Player* player = findPlayer(0);
    int bet = _game.get_bet() - player->get_bet();

    player->takeFromMoney(bet);
    if (player->get_money() <= 0)
    {
        bet += player->get_money();
        player->set_money(0);
        player->set_allIn(true);
    }
    player->set_bet(bet + player->get_bet());
    //setChipImage(player->get_chipUI(), player->get_bet());
    chipSingleAnimation(player->get_id());
    updatePlayersChips(player->get_id());
    QTimer::singleShot(_chipsDuration, this, SLOT(setImagePlayerChipSlot()));
    _game.addToBank(bet);
    player->updateBetUI();
    player->updateMoneyUI();
    //updateBankUI();
    player->set_call(true);
    if (player->is_allIn())
    {
        player->get_infoUI()->setText("Ва-банк");
        player->get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(90, 0, 90, 255), stop:1 rgba(230, 0, 230, 200));");
    }
    else
    {
        player->get_infoUI()->setText("Колл");
        player->get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 105, 255), stop:1 rgba(0, 0, 200, 200));");
    }
    //_game.incrTurn(_players.size());
    //playerActionSlot();

}

void MainInterface::foldBtnSlot()
{
    Player* player = findPlayer(0);
    player->set_fold(true);
    player->get_card1UI()->clear();
    player->get_card2UI()->clear();
    player->get_infoUI()->setText("Пас");
    player->get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(90, 90, 90, 255), stop:1 rgba(200, 200, 200, 200));");
    player->get_chipUI()->clear();
    setEnabledActionBtns(false);
    _game.incrTurn(_players.size());
    //playerActionSlot();
    foldAnimation(player->get_id());
    QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
}

void MainInterface::acceptBetBtnSlot()
{
    Player* player = findPlayer(0);
    int bet = ui.raisingBet_lbl->text().toInt() + _game.get_bet() + player->get_bet();
    _game.addToBank(bet);
    _game.set_bet(bet);
    ui.bankBetValue_lbl->setText(QString::number(_game.get_bet()));
    _game.set_raised(true);
    //updateBankUI();
    player->set_bet(bet);
    player->updateBetUI();
    player->takeFromMoney(bet);
    player->updateMoneyUI();
    player->set_raised(true);
    player->set_call(true);
    if (player->get_money() == 0)
    {
        player->set_allIn(true);
        player->get_infoUI()->setText("Ва-банк");
        player->get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(90, 0, 90, 255), stop:1 rgba(230, 0, 230, 200));");
    }
    else
    {
        player->get_infoUI()->setText("Райз");
        player->get_infoUI()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(100, 0, 0, 255), stop:1 rgba(200, 0, 0, 200));");
    }
    //setChipImage(player->get_chipUI(), player->get_bet());
    chipSingleAnimation(player->get_id());
    updatePlayersChips(player->get_id());
    //_game.incrTurn(_players.size());
    ui.raise_btn->setChecked(false);
    setEnabledActionBtns(false);
    setEnabledBetsBtns(false);
    ui.setBetFrame->setVisible(false);
    ui.betVSlider->setValue(0);
    ui.maxBet_lbl->clear();
    ui.raisingBet_lbl->clear();
    setPlayersCallFalse(player->get_id());
    QTimer::singleShot(_chipsDuration, this, SLOT(setImagePlayerChipSlot()));
    //playerActionSlot();
}

void MainInterface::showWinnerSlot()
{
    //updateBankUI();
    int notFoldCount = getNotFoldCount();
    for (auto& player : _players)
    {
        if (_game.get_round() == 4 && notFoldCount > 1)
        {
            player.get_card1UI()->setEnabled(false);
            player.get_card2UI()->setEnabled(false);
        }
        player.get_generalLayout()->setEnabled(false);
        if (player.is_winner())
            showWinner(player);
    }
}

void MainInterface::cancelBetSlot()
{
    ui.betVSlider->setValue(0);
}

void MainInterface::setAllInBetSlot()
{
    int maxValue = ui.betVSlider->maximum();
    ui.betVSlider->setValue(maxValue);
}

void MainInterface::leaveTableSlot()
{
    saveData();
    hide();
    MainMenu mainMenu;
    mainMenu.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    if (mainMenu.exec())
    {
        setData();
        show();
    }
    else
    {
        close();
    }
}

void MainInterface::showCombinationsHelpSlot()
{
    //CombinationsHelpForm combHelp;
    ////combHelp.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    //combHelp.setWindowTitle("Комбинации");
    //combHelp.setWindowModality(Qt::WindowModality::ApplicationModal);
    //combHelp.exec();
    if (ui.combinations_btn->isChecked())
        ui.combsImage_lbl->setPixmap(QPixmap("imgs/combinations.png"));
    else
        ui.combsImage_lbl->clear();
}

void MainInterface::setImagePlayerCardSlot()
{
    //std::sort(_players.begin(), _players.end(), [](const Player& p1, const Player& p2) {return p1.get_turn() < p2.get_turn(); });
    for (int i = 0; i < 2; ++i)
    {
        for (Player& player : _players)
        {
            if (!player.inGame() || player.get_cardUI(i+1)->pixmap())
                continue;
            player.is_ai() ? player.setCardShirtImage(i) : player.setCardImage(i);
            //player.setCardImage(i);
            return;
        }
    }
}

void MainInterface::setImageTableCardSlot()
{
    if (!ui.tableCard_1->pixmap())
    {
        setCardImageToTable(0);
        return;
    }
    else if (!ui.tableCard_2->pixmap())
    {
        setCardImageToTable(1);
        return;
    }
    else if (!ui.tableCard_3->pixmap())
    {
        setCardImageToTable(2);
        return;
    }
    else if (!ui.tableCard_4->pixmap())
    {
        setCardImageToTable(3);
        return;
    }
    else if (!ui.tableCard_5->pixmap())
    {
        setCardImageToTable(4);
        return;
    }
}

void MainInterface::setImagePlayerChipSlot()
{
    for (Player& player : _players)
    {
        if (player.get_turn() != _game.get_turn())
            continue;
        setChipImage(player.get_chipUI(), player.get_bet());
        findChipLbl(player.get_id())->setVisible(false);
        _game.incrTurn(_players.size());
        QTimer::singleShot(_compActionDelay, this, SLOT(playerActionSlot()));
        return;
    }
}

void MainInterface::setImageDealerSlot()
{
    for (Player& player : _players)
    {
        if (player.is_dealer())
            player.get_dealerUI()->setPixmap(QPixmap("imgs/dealer1.png"));
        else
            player.get_dealerUI()->clear();
    }
    ui.dealerChip->setVisible(false);
}

void MainInterface::setBankImageSlot()
{
    if (_game.get_bank() <= 1000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bankChips_1.png"));
    else if (_game.get_bank() > 1000 && _game.get_bank() <= 3000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bankChips_2.png"));
    else if (_game.get_bank() > 3000 && _game.get_bank() <= 5000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bankChips_3.png"));
    else if (_game.get_bank() > 5000 && _game.get_bank() <= 10000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bankChips_4.png"));
    else if (_game.get_bank() > 10000 && _game.get_bank() <= 30000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bankChips_5.png"));
    else if (_game.get_bank() > 30000 && _game.get_bank() <= 50000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bankChips_6.png"));
    else if (_game.get_bank() > 50000 && _game.get_bank() <= 100000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bankChips_7.png"));
    else if (_game.get_bank() > 100000 && _game.get_bank() <= 200000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bankChips_8.png"));
    else if (_game.get_bank() > 200000 && _game.get_bank() <= 400000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bankChips_9.png"));
    else if (_game.get_bank() > 400000 && _game.get_bank() <= 500000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bankChips_10.png"));
    else if (_game.get_bank() > 500000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bankChips_11.png"));

    /*if (_game.get_bank() <= 1000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bank1.png"));
    else if (_game.get_bank() > 100 && _game.get_bank() <= 10000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bank2.png"));
    else if (_game.get_bank() > 10000 && _game.get_bank() <= 50000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bank3.png"));
    else if (_game.get_bank() > 50000 && _game.get_bank() <= 100000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bank4.png"));
    else if (_game.get_bank() > 100000 && _game.get_bank() <= 500000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bank5.png"));
    else if (_game.get_bank() > 500000)
        ui.bankImage_lbl->setPixmap(QPixmap("imgs/bank6.png"));
    else
        ui.bankImage_lbl->clear();*/
    setVisibleChips(false);
    if (_game.get_bank() != 0)
        updateBankUI();
}
void MainInterface::endWinAnimationSlot()
{
    ui.bankChips->setVisible(false);
    for (auto& player : _players)
    {
        if (player.is_fold() || !player.inGame())
            continue;
        player.updateMoneyUI();
        updatePlayersChips(player.get_id());
    }
    ui.startGame_btn->setEnabled(true);
    ui.leaveTable_btn->setEnabled(true);
}

void MainInterface::winAnimationSlot()
{
    for (auto& player : _players)
    {
        if (player.is_winner())
            winAnimation(player.get_id());
    }
}

void MainInterface::showDownCardsSlot()
{
    for (auto& player : _players)
    {
        if (!player.is_fold())
        {
            player.setCardImage(0);
            player.setCardImage(1);
        }
    }
}

void MainInterface::showSettingsFormSlot()
{
    //SettingsForm settingsForm(_soundPlayer, _dealDelay, _chipsDuration, _cardImage, _shirtImage);
    SettingsForm settingsForm(_dealDelay, _chipsDuration, _cardImage, _shirtImage, _playerName);
    settingsForm.setWindowFlags(Qt::FramelessWindowHint);
    if (settingsForm.exec())
        updateSettings();
}

void MainInterface::showRulesFormSlot()
{
    RulesForm rulesForm;
    rulesForm.setWindowFlags(Qt::FramelessWindowHint);
    rulesForm.exec();
}

void MainInterface::cheatBtnSlot()
{
    if (Player* player = findPlayer(0))
    {
        player->addToMoney(10000);
        player->updateMoneyUI();
    }
}

void MainInterface::showCheatBtnSlot()
{
    if (ui.cheat_btn->isVisible())
        ui.cheat_btn->setVisible(false);
    else
        ui.cheat_btn->setVisible(true);
}

void MainInterface::showTestBtnSlot()
{
    if (ui.test_btn->isVisible())
        ui.test_btn->setVisible(false);
    else
        ui.test_btn->setVisible(true);
}


