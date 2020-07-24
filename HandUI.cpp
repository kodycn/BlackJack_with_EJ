#include "HandUI.h"
#include <iostream>

HandUI::HandUI()
{
    this->handLayout = new QHBoxLayout(this);
    this->valueLabel = new QLabel(this);
    this->valueLabel->setGeometry(geometry().x(), geometry().y(), geometry().width(), geometry().height());
    this->valueLabel->setHidden(false);
    this->valueLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
}

QHBoxLayout* HandUI::getLayout() const
{
    return handLayout;
}

void HandUI::setValueLabel(const QString& str)
{
    valueLabel->setText(str);
}
