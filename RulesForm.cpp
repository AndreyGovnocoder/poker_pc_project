#include "RulesForm.h"

RulesForm::RulesForm(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	setBackground();
}

void RulesForm::setBackground()
{
	QPixmap bkgnd("imgs/rules_textFon.png");
	bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);
}
