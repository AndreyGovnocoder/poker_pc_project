#pragma once

#include <QDialog>
#include "ui_MainMenu.h"
#include "ChangeCityForm.h"
#include "MainInterface.h"

class MainMenu : public QDialog, public Ui::MainMenu
{
	Q_OBJECT

public:
	MainMenu(QWidget *parent = Q_NULLPTR);
	~MainMenu() = default;
	void set_myBackground();

private:
	void updateCity(int city);
	void setCity();

private slots:
	void changeCitySlot();
	void startGameSlot() { accept(); };
	void showSettingsFormSlot();
};
