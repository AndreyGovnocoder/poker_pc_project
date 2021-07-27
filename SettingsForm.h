#pragma once

#include <QDialog>
#include "ui_SettingsForm.h"
#include <QDebug>
#include <QSettings>

class SettingsForm : public QDialog, public Ui::SettingsForm
{
	Q_OBJECT

public:
	//SettingsForm(QMediaPlayer* soundPlayer, int dealDelay, int chipsDuration, int cardImage, int shirtImage, QWidget* parent = Q_NULLPTR);
	SettingsForm(int dealDelay, int chipsDuration, int cardImage, int shirtImage, const QString& playerName, QWidget* parent = Q_NULLPTR);
	SettingsForm(QWidget* parent = Q_NULLPTR);
	~SettingsForm() = default;

private:
	int _dealDelay;
	int _chipsDuration;
	int _cardImage;
	int _shirtImage;
	int _soundVolume;
	int _musicVolume;
	QString _playerName;
	//QMediaPlayer* _soundPlayer;
	void setBackground();
	void setCardImageValue();
	void setShirtImageValue();
	void setDealDelayValue();
	void setChipsDurationValue();
	void setSoundValue();
	void setMusicValue();
	void setData();

private slots:
	void setCardsAnimSliderSlot(int value);
	void setChipsAnimSliderSlot(int value);
	void setSoundVolumeSlot(int value);
	void setMusicVolumeSlot(int value);
	void acceptSlot();
};
