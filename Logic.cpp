#include "Logic.h"

#include <iostream> // std::cerr

/// Static Field Initialization
int Logic::currentBet   = 0;
int Logic::currentMoney = 100;
// card fields
std::vector<Card> Logic::deck = Logic::generateDeck();
std::vector<Card> Logic::userHand {};
std::vector<Card> Logic::dealerHand {};
// random number generator fields
std::random_device rd {};
std::mt19937 rng ( rd() );

/// Private Functions
std::vector<Card> Logic::generateDeck()
{
    std::vector<Card> deck {};
    QString suits[4] =
    {
        "clubs",
        "diamonds",
        "hearts",
        "spades"
    };
    for (size_t suit = 0; suit < 4; suit++)
    {
        for (unsigned short rank = 1; rank <= 13; rank++)
        {
            deck.emplace_back( rank, suits[suit] );
        }
    }
    return deck;
}


/// Public Function Definitions
bool Logic::doBet(int bet)
{
    if(bet <= currentMoney)
    {
        currentBet = bet;
        currentMoney -= currentBet;
        return true;
    }
    return false;
}

bool Logic::doBet(const QString& bet)
{
    return doBet(bet.toInt());
}


/// Getter function definitions
int Logic::getCurrentBet()
{
    return currentBet;
}

int Logic::getCurrentMoney()
{
    return currentMoney;
}
