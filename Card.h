#ifndef CARD_H
#define CARD_H

class Card
{
private:
    unsigned short rank;
    char suit;
public:

    // initializer list ctor
    Card(unsigned short r, char s): rank(r), suit(s) {}

    // run time dtor
    ~Card() = default;

    // getter funcs
    unsigned short getRank() const;
    char getSuit() const;
};

#endif