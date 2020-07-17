#ifndef LOGIC_H
#define LOGIC_H

#include <vector>
#include <random> // std::random_device

#include "mainwindow.h"
#include "Card.h"

class Logic
{
private:
    /// Fields
    static int currentBet;
    static int currentMoney;

    // card fields
    static std::vector<Card> deck;
    static std::vector<Card> userHand;
    static std::vector<Card> dealerHand;

    // random number generator fields
    static std::random_device rd;
    static std::mt19937 rng;

    /// Functions
    // helper functions
    static std::vector<Card> generateDeck();
public:
    /// Functions
    static bool doBet(int);
    static bool doBet(const QString&);

    // getter funcs
    static int getCurrentBet();
    static int getCurrentMoney();
};

#endif // LOGIC_H
