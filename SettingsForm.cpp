#include "SettingsForm.h"

//SettingsForm::SettingsForm(QMediaPlayer* soundPlayer, int dealDelay, int chipsDuration, int cardImage, int shirtImage, QWidget *parent)
//	: QDialog(parent)
//	, _dealDelay(dealDelay)
//	, _chipsDuration(chipsDuration)
//	, _cardImage(cardImage)
//	, _shirtImage(shirtImage)
//{
//	_soundPlayer = soundPlayer;
//	setupUi(this);
//	setBackground();
//	setData();
//}

SettingsForm::SettingsForm(int dealDelay, int chipsDuration, int cardImage, int shirtImage, const QString& playerName, QWidget* parent)
	: QDialog(parent)
	, _dealDelay(dealDelay)
	, _chipsDuration(chipsDuration)
	, _cardImage(cardImage)
	, _shirtImage(shirtImage)
	, _playerName(playerName)
{
	setupUi(this);
	setBackground();
	setData();
}

SettingsForm::SettingsForm(QWidget* parent)
	: QDialog(parent)
{
	QSettings settings("Goose", "Poker");
	settings.beginGroup("settings");
	_cardImage = settings.value("cardImage", 1).toInt();
	_shirtImage = settings.value("shirtImage", 1).toInt();
	_dealDelay = settings.value("dealDelay", 500).toInt();
	_chipsDuration = settings.value("chipsDuration", 400).toInt();
	_soundVolume = settings.value("soundVolume", 50).toInt();
	_musicVolume = settings.value("musicVolume", 50).toInt();
	_playerName = settings.value("playerName", "Игрок").toString();
	setupUi(this);
	setBackground();
	setData();
}

void SettingsForm::setBackground()
{
	QPixmap bkgnd("imgs/settings_bkgd.png");
	bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);
}

void SettingsForm::setCardImageValue()
{
	qDebug() << "in setCardImageValue()";
	switch (_cardImage)
	{
	case 1:
		qDebug() << "case 1";
		card1_rbtn->setChecked(true);
		break;
	case 2:
		qDebug() << "case 2";
		card2_rbtn->setChecked(true);
		break;
	default:
		qDebug() << "default";
		card1_rbtn->setChecked(true);
		break;
	}
}

void SettingsForm::setShirtImageValue()
{
	qDebug() << "in setShirtImageValue()";
	switch (_shirtImage)
	{
	case 1:
		shirt1_rbtn->setChecked(true);
		break;
	case 2:
		shirt2_rbtn->setChecked(true);
		break;
	default:
		shirt1_rbtn->setChecked(true);
		break;
	}
}

void SettingsForm::setDealDelayValue()
{
	qDebug() << "in setDealDelayValue()";
	int value = 1000 - _dealDelay;
	cardsAnimValue_lbl->setText(QString::number(value));
	cardsAnimSlider->setValue(value);
}

void SettingsForm::setChipsDurationValue()
{
	qDebug() << "in setChipsDurationValue()";
	int value = 1000 - _chipsDuration;
	qDebug() << "value = " << value;
	chipsAnimValue_lbl->setText(QString::number(value));
	chipsAnimSlider->setValue(value);
}

void SettingsForm::setSoundValue()
{
	QSettings settings("Goose", "Poker");
	settings.beginGroup("settings");
	_soundVolume = settings.value("soundVolume", 50).toInt();
	soundDial->setValue(_soundVolume);
}

void SettingsForm::setMusicValue()
{
	QSettings settings("Goose", "Poker");
	settings.beginGroup("settings");
	_musicVolume = settings.value("musicVolume", 50).toInt();
	musicDial->setValue(_musicVolume);
}

void SettingsForm::setData()
{
	qDebug() << "in setData()";
	setCardImageValue();
	setShirtImageValue();
	setDealDelayValue();
	setChipsDurationValue();
	setMusicValue();
	setSoundValue();
	playerName_textEdit->setText(_playerName);
}

void SettingsForm::setCardsAnimSliderSlot(int value)
{
	cardsAnimValue_lbl->setText(QString::number(value));
	_dealDelay = 1000 - value;
}

void SettingsForm::setChipsAnimSliderSlot(int value)
{
	chipsAnimValue_lbl->setText(QString::number(value));
	_chipsDuration = 1000 - value;
}

void SettingsForm::setSoundVolumeSlot(int value)
{
	qDebug() << "in setSoundVolumeSlot";
	soundValue_lbl->setText(QString::number(value));
	_soundVolume = value;
}

void SettingsForm::setMusicVolumeSlot(int value)
{
	qDebug() << "in setMusicVolumeSlot";
	musicValue_lbl->setText(QString::number(value));
	_musicVolume = value;
}

void SettingsForm::acceptSlot()
{
	shirt1_rbtn->isChecked() ? _shirtImage = 1 : _shirtImage = 2;
	card1_rbtn->isChecked() ? _cardImage = 1 : _cardImage = 2;
	QSettings settings("Goose", "Poker");
	settings.beginGroup("settings");
	settings.setValue("cardImage", _cardImage);
	settings.setValue("shirtImage", _shirtImage);
	settings.setValue("dealDelay", _dealDelay);
	settings.setValue("chipsDuration", _chipsDuration);
	settings.setValue("soundVolume", _soundVolume);
	settings.setValue("musicVolume", _musicVolume);
	if (!playerName_textEdit->text().isEmpty())
		settings.setValue("playerName", playerName_textEdit->text());
	accept();
}


