#ifndef HAND_H
#define HAND_H

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>

#include <vector>

#include "Card.h"

class Hand : public QFrame
{
    Q_OBJECT
private:
    QHBoxLayout *handLayout;
    std::vector<Card> cards;
public:
    Hand();
    Hand(const Hand &tmpHand): QFrame() { // copy ctr
        cards = tmpHand.cards;
        handLayout = new QHBoxLayout(this);
    };
    // getter funcs
    std::vector<Card>& getCards(); // let this be modifiable
    QHBoxLayout* getLayout() const;
};

#endif // HAND_H
