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
    static int currentBet; // this should not be adjusted when splitting
    static int currentInsuranceBet;
    static int currentMoney;

    // card fields
    static std::vector<Card> deck;
    static std::vector<std::vector<Card>> userHands; // split functi0nality
    static std::vector<bool> doubleDownTracker; // tracking which hand in userHands have doubled down
                                                // ensure that its size = userHands.size()
    static size_t activeUserHandIndex;
    static std::vector<Card> dealerHand;

    // random number generator fields
    static std::random_device rd;
    static std::mt19937 rng;

    /// Functions
    // helper functions
    static std::vector<Card> generateDeck();
    static void payPlayer(int);



public:
    /// Functions
    static bool doBet(int);
    static bool doBet(const QString&);
    static bool makeInsuranceBet();
    static void shuffleDeck();
    static bool hasAce(const std::vector<Card>&);
    static bool hasBlackjack(const std::vector<Card>&);

    // Game funcs
    static void dealCards();
    static unsigned short doDealerActions();
    static void calculatePayout(); // calculate player winnings (depending on the player's hands)

    // getter funcs
    static int getCurrentBet();
    static int getInsuranceBet();
    static int getCurrentMoney();
    static const std::vector<Card> getDeck();
    static const std::vector<Card> getDealerHand();
    static const std::vector<std::vector<Card>> getUserHand();

    // helper funcs
    static unsigned short calculateHandValue(const std::vector<Card>&);
};

#endif // LOGIC_H
