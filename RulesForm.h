#pragma once

#include <QDialog>
#include "ui_RulesForm.h"

class RulesForm : public QDialog, public Ui::RulesForm
{
	Q_OBJECT

public:
	RulesForm(QWidget *parent = Q_NULLPTR);
	~RulesForm() = default;

private:
	void setBackground();

private slots:

};
