#include "HandUI.h"
#include <iostream>

HandUI::HandUI()
{
    this->setFrameStyle(QFrame::Box | QFrame::Plain);
    this->handLayout = new QHBoxLayout(this);
}

QHBoxLayout* HandUI::getLayout() const
{
    return handLayout;
}
