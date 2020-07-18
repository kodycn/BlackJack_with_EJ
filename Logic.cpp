#include "Logic.h"

#include <iostream> // std::cerr
#include <vector>

/// Static Field Initialization
int Logic::currentBet   = 0;
int Logic::currentMoney = 100;
// card fields
std::vector<Card> Logic::deck = Logic::generateDeck();
std::vector<Card> Logic::userHand {};
std::vector<Card> Logic::dealerHand {};
// random number generator fields
std::random_device Logic::rd {};
std::mt19937 Logic::rng ( rd() );

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
    if(bet <= currentMoney && bet != 0)
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

void Logic::shuffleDeck()
{
    std::shuffle(deck.begin(), deck.end(), rng);
}


/// Public Game function definitions
void Logic::dealCards()
{
    // if cards are in dealer's hand, return to deck
    while (dealerHand.size() > 0)
    {
        deck.push_back( Card(dealerHand.back()) );
        dealerHand.pop_back();
    }
    // if cards are in user's hand, return to deck
    while (userHand.size() > 0)
    {
        deck.push_back( Card(userHand.back()) );
        userHand.pop_back();
    }

    // shuffle deck prior to dealing
    shuffleDeck();

    // deal cards to user and dealer
    for (int i = 0; i < 2; i++)
    {
        dealerHand.push_back( Card(deck.back()) );
        deck.pop_back();
        userHand.push_back( Card(deck.back()) );
        deck.pop_back();
    }
}


/// Public Getter function definitions
int Logic::getCurrentBet()
{
    return currentBet;
}

int Logic::getCurrentMoney()
{
    return currentMoney;
}
const std::vector<Card> Logic::getDeck()
{
    return deck;
}
const std::vector<Card> Logic::getDealerHand()
{
    return dealerHand;
}
const std::vector<Card> Logic::getUserHand()
{
    return userHand;
}
