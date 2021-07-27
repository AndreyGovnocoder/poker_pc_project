#pragma once

#include <QDialog>
#include "ui_WinLoseForm.h"

class WinLoseForm : public QDialog, public Ui::WinLoseForm
{
	Q_OBJECT

public:
	WinLoseForm(bool win, int city, QWidget *parent = Q_NULLPTR);
	~WinLoseForm() = default;

private:

	void setBackground(const QString& fon);

};
