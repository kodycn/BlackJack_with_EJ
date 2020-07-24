#include "HandUI.h"
#include <iostream>

HandUI::HandUI()
{
    this->handLayout = new QHBoxLayout(this);
}

QHBoxLayout* HandUI::getLayout() const
{
    return handLayout;
}
