#include "Hand.h"
#include <iostream>

Hand::Hand()
{
    this->setFrameStyle(QFrame::Box | QFrame::Plain);
    this->handLayout = new QHBoxLayout(this);
}
