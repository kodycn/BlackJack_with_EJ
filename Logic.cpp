#include "Logic.h"

#include <iostream> // std::cerr
#include <vector>


/// Static Field Initialization
int Logic::currentBet = 0;
int Logic::currentInsuranceBet = 0;
int Logic::currentMoney = 100;
// card fields
std::vector<Card> Logic::deck = Logic::generateDeck();
std::vector<std::vector<Card>> Logic::userHands {};
size_t Logic::activeUserHandIndex = 0;
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

bool Logic::doInsurance()
{
    if (currentMoney - (currentBet / 2) > 0)
    {
        currentInsuranceBet = currentBet / 2;
        currentMoney -= currentInsuranceBet;
        return true;
    }
    return false;
}

void Logic::shuffleDeck()
{
    std::shuffle(deck.begin(), deck.end(), rng);
}

bool Logic::hasAce(const std::vector<Card>& hand)
{
    for (const Card& card : hand)
    {
        if (card.getRank() == 1) return true;
    }
    return false;
}

bool Logic::hasBlackjack(const std::vector<Card>& hand)
{
    if (hand.size() != 2)
    {
        return false;
    }

    if ( (hand.at(0).getRank() >= 10 && hand.at(1).getRank() == 1) ||
         (hand.at(1).getRank() >= 10 && hand.at(0).getRank() == 1)
       )
    {
        return true;
    }

    return false;
}

/// Public Game function definitions
void Logic::dealCards()
{
    auto& dealerHandCards = dealerHand;

    // if cards are in dealer's hand, return to deck
    while (dealerHandCards.size() > 0)
    {
        deck.push_back( Card(dealerHandCards.back()) );
        dealerHandCards.pop_back();
    }
    // if cards are in user's hand, return to deck

    while (userHands.size() >0) // all the possible hands in user's hand
    {
        auto& hand = userHands.back(); // one hand at a time
        while(hand.size() > 0) // while there're still cards in that split
        {
            deck.push_back(Card(hand.back())); // put the card back
            hand.pop_back(); // take the card outta the hand
        }
        userHands.pop_back(); //take that hand out after we're done with it
    }

    // shuffle deck prior to dealing
    shuffleDeck();
    std::vector<Card> newUserHand {};
    // deal cards to user and dealer
    for (int i = 0; i < 2; i++)
    {
        dealerHand.push_back( Card(deck.back()) );
        deck.pop_back();

        newUserHand.push_back( Card(deck.back()) );
        deck.pop_back();


    }
    userHands.push_back(newUserHand); // adding the hand once full w/ cardz

}

unsigned short Logic::doDealerActions()
{
    bool dealerHasAce = hasAce(dealerHand);
    unsigned short currentDealerValue = dealerHand.front() + dealerHand.back();
    if (dealerHasAce)
    {
        currentDealerValue += 10;
    }

    while (currentDealerValue < 17) // dealer hits until 17 or more
    {
        dealerHand.push_back(deck.back());
        deck.pop_back();
        currentDealerValue = dealerHand.back() + currentDealerValue;
        if (currentDealerValue > 21 && dealerHasAce)
        {
            currentDealerValue -= 10;
            dealerHasAce = false;
        }
    }

    return currentDealerValue;
}


/// Public Getter function definitions
int Logic::getCurrentBet()
{
    return currentBet;
}

/// Public Getter function definitions
int Logic::getInsuranceBet()
{
    return currentInsuranceBet;
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
const std::vector<std::vector<Card>> Logic::getUserHand()
{
    return userHands;
}
