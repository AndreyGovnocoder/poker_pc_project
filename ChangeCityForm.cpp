#include "ChangeCityForm.h"

ChangeCityForm::ChangeCityForm(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
    setBackground();
    setCityImages();
    //QSettings settings(QDir::currentPath() + "/settings.ini", QSettings::IniFormat);
    QSettings settings("Goose", "Poker");
    settings.beginGroup("game");
    set_deposit(settings.value("deposit").toInt());
    set_city(settings.value("city").toInt());
    setEnableButtons();
    set_page();
}

void ChangeCityForm::setBackground()
{
    QPixmap bkgnd("imgs/fon.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatioByExpanding);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    palette.setColor(QPalette::WindowText, Qt::white);
    this->setPalette(palette);
}

void ChangeCityForm::set_deposit(int deposit)
{
    _deposit = deposit;
    depositValue_lbl->setText(QString::number(_deposit) + " $");
}

void ChangeCityForm::set_page()
{
    stackedWidget->setCurrentIndex(_city - 1);
}

void ChangeCityForm::setEnableButtons()
{
    if (_deposit >= 5000)
        startSingapore_btn->setEnabled(true);
    if (_deposit >= 30000)
        startMakao_btn->setEnabled(true);
    if (_deposit >= 100000)
        startLasVegas_btn->setEnabled(true);
}

void ChangeCityForm::setCityImages()
{
    sochiImage->setPixmap(QPixmap("imgs/sochi.png"));
    singaporeImage->setPixmap(QPixmap("imgs/singapore.png"));
    makaoImage->setPixmap(QPixmap("imgs/makao.png"));
    lasVegasImage->setPixmap(QPixmap("imgs/lasVegas.png"));
}

void ChangeCityForm::nextPageSlot()
{
    int currentIndex = stackedWidget->currentIndex();
    ++currentIndex;
    if (currentIndex > 3)
        currentIndex = 0;
    stackedWidget->setCurrentIndex(currentIndex);
}

void ChangeCityForm::previousPageSlot()
{
    int currentIndex = stackedWidget->currentIndex();
    --currentIndex;
    if (currentIndex < 0)
        currentIndex = 3;
    stackedWidget->setCurrentIndex(currentIndex);
}

void ChangeCityForm::startSochiSlot()
{
    _city = 1;
    accept();
}

void ChangeCityForm::startstartSingaporeSlot()
{
    _city = 2;
    accept();
}

void ChangeCityForm::startMakaoSlot()
{
    _city = 3;
    accept();
}

void ChangeCityForm::startLasVegasSlot()
{
    _city = 4;
    accept();
}
