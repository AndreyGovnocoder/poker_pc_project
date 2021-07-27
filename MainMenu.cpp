#include "MainMenu.h"

MainMenu::MainMenu(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
    set_myBackground();
    setCity();
}

void MainMenu::set_myBackground()
{
    QPixmap bkgnd("imgs/main_scr.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

void MainMenu::changeCitySlot()
{
    ChangeCityForm changeCityForm;
    changeCityForm.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    if (changeCityForm.exec())
    {
        updateCity(changeCityForm.get_city());
    }
}

void MainMenu::updateCity(int city)
{
    //QSettings settings(QDir::currentPath() + "/settings.ini", QSettings::IniFormat);
    QSettings settings("Goose", "Poker");
    settings.beginGroup("game");
    settings.setValue("city", city);
    setCity();
}

void MainMenu::setCity()
{
    QSettings settings("Goose", "Poker");
    settings.beginGroup("game");
    int city = settings.value("city", 1).toInt();

    switch (city)
    {
    case 1:
        currCity_lbl->setText("Сочи");
        break;
    case 2:
        currCity_lbl->setText("Сингапур");
        break;
    case 3:
        currCity_lbl->setText("Макао");
        break;
    case 4:
        currCity_lbl->setText("Лас-Вегас");
        break;
    }
}

void MainMenu::showSettingsFormSlot()
{
    SettingsForm settingsForm;
    settingsForm.setWindowFlags(Qt::FramelessWindowHint);
    settingsForm.exec();
}
