#include "Hand.h"
#include <iostream>

Hand::Hand()
{
    this->setFrameStyle(QFrame::Box | QFrame::Plain);
    this->handLayout = new QHBoxLayout(this);
}

std::vector<Card>& Hand::getCards()
{
    return cards; // dangerous
}

QHBoxLayout* Hand::getLayout() const
{
    return handLayout;
}
