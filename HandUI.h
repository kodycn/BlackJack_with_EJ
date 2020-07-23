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
public:
    HandUI();

    // getter funcs
    QHBoxLayout* getLayout() const;
};

#endif // HANDUI_H
