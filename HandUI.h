#ifndef HANDUI_H
#define HANDUI_H

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>

#include "Card.h"

/// HandUI Class
/// - displays the hand using a QHBoxLayout
class HandUI : public QFrame
{
    Q_OBJECT
private:
    QHBoxLayout *handLayout;
    QLabel *valueLabel;
public:
    HandUI();

    // getter funcs
    QHBoxLayout* getLayout() const;

    // setter funcs
    void setValueLabel(const QString&);
};

#endif // HANDUI_H
