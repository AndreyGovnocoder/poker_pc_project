#include "Card.h"

Card::Card(int id, const QString& suit, int rank, const QString& image, const QString& shirtImage)
	: _id(id)
	, _suit(suit)
	, _rank(rank)
	, _image(image)
	, _shirtImage(shirtImage)
{
}

Card::Card(const Card* card)
	: _id(card->get_id())
	, _suit(card->get_suit())
	, _rank(card->get_rank())
	, _image(card->get_image())
	, _shirtImage(card->get_shirtImage())
{
}