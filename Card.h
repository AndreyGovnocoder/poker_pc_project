#pragma once
#include <qstring.h>

class Card
{
public:
	Card(int id, const QString& suit, int rank, const QString& image, const QString& shirtImage);
	Card(const Card* card);
	Card() = default;
	~Card() = default;

	void set_id(int id) { _id = id; };
	void set_suit(const QString& suit) { _suit = suit; };
	void set_rank(int rank) { _rank = rank; };
	void set_image(const QString& image) { _image = image; };
	void set_shirtImage(const QString& shirtImage) { _shirtImage = shirtImage; };

	int get_id() const { return _id; };
	const QString& get_suit() const { return _suit; };
	int get_rank() const { return _rank; };
	const QString& get_image() const { return _image; };
	const QString& get_shirtImage() const { return _shirtImage; };

private:
	int _id;
	QString _suit;
	int _rank;
	QString _image;
	QString _shirtImage;
};

