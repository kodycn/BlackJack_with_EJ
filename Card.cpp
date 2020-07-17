#include "Card.h"

unsigned short Card::getRank() const
{
    return rank;
}

QString Card::getSuit() const
{
    return suit;
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
