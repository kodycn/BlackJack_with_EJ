#ifndef CARD_H
#define CARD_H

#include "qchar.h"
#include "qstring.h"
#include <iostream>

class Card
{
    friend std::ostream& operator<<(std::ostream&, Card);
private:
    unsigned short rank; // 1-13
    QString suit; // C D H S
public:

    // initializer list ctor
    Card(unsigned short r, QString s): rank(r), suit(s) {}

    // run time dtor
    ~Card() = default;

    // getter funcs
    unsigned short getRank() const;
    QString getSuit() const;
};

#endif // CARD_H
