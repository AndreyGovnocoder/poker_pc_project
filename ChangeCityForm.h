#pragma once

#include <QDialog>
#include "ui_ChangeCityForm.h"
#include "MainInterface.h"

class ChangeCityForm : public QDialog, public Ui::ChangeCityForm
{
	Q_OBJECT

public:
	ChangeCityForm(QWidget *parent = Q_NULLPTR);
	~ChangeCityForm() = default;
	void setBackground();
	int get_city() { return _city; };

private:
	int _deposit;
	int _city;

	void set_deposit(int deposit);
	void set_city(int city) { _city = city; };
	void set_page();
	void setEnableButtons();
	void setCityImages();

private slots:
	void nextPageSlot();
	void previousPageSlot();
	void startSochiSlot();
	void startstartSingaporeSlot();
	void startMakaoSlot();
	void startLasVegasSlot();
};
