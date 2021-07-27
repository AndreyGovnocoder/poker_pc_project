#include "WinLoseForm.h"

WinLoseForm::WinLoseForm(bool win, int city, QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	if (win)
	{
		setBackground("imgs/img_win.png");
		switch (city)
		{
		case 1:
			headLabel->setText(headLabel->text() + " Сочи");
			break;
		case 2:
			headLabel->setText(headLabel->text() + " Сингапур");
			break;
		case 3:
			headLabel->setText(headLabel->text() + " Макао");
			break;
		case 4:
			headLabel->setText(headLabel->text() + " Лас-Вегас");
			break;
		}
	}
	else
	{
		setBackground("imgs/img_lose.png");
		headLabel->setText("ВЫ ПРОИГРАЛИ!");
	}
}

void WinLoseForm::setBackground(const QString& fon)
{
	QPixmap bkgnd(fon);
	bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);
}