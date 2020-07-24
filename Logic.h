#ifndef LOGIC_H
#define LOGIC_H

#include <vector>
#include <random> // std::random_device

#include "mainwindow.h"
#include "Card.h"
#include "HandUI.h"

class Logic
{
    // friends
    friend class MainWindow;
private:
    /// Fields
    static int currentBet;
    static int currentMoney;

    // card fields
    static std::vector<Card> deck;
    static std::vector<std::vector<Card>> userHands; // split functi0nality
    static size_t activeUserHandIndex;
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
    static void shuffleDeck();

    // Game funcs
    static void dealCards();

    // getter funcs
    static int getCurrentBet();
    static int getCurrentMoney();
    static const std::vector<Card> getDeck();
    static const std::vector<Card> getDealerHand();
    static const std::vector<std::vector<Card>> getUserHand();
};

#endif // LOGIC_H
