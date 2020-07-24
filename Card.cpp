#include "Card.h"

unsigned short Card::getRank() const
{
    return rank;
}

QString Card::getSuit() const
{
    return suit;
}

unsigned short Card::operator+ (const Card& that)
{
    unsigned short thisRank = this->getRank();
    unsigned short thatRank = that.getRank();
    if (thisRank > 10) thisRank = 10;
    if (thatRank > 10) thatRank = 10;
    return thisRank + thatRank;
}

unsigned short Card::operator+ (const unsigned short& that)
{
    unsigned short thisRank = this->getRank();
    if (thisRank > 10) thisRank = 10;
    return thisRank + that;
}

// non-member card output
std::ostream& operator<<(std::ostream& out, Card c)
{
    switch (c.getRank())
    {
    case 1:
        out << "ace";
        break;
    case 11:
        out << "jack";
        break;
    case 12:
        out << "queen";
        break;
    case 13:
        out << "king";
        break;
    default:
        out << c.getRank();
        break;
    }
    out << " of " << c.getSuit().toStdString();
    return out;
}
