#include "Logic.h"

#include <iostream> // std::cerr
#include <vector>

#include <QDialog>

/// Static Field Initialization
int Logic::currentBet = 0;
int Logic::currentInsuranceBet = 0;
int Logic::currentMoney = 100;
// card fields
std::vector<Card> Logic::deck = Logic::generateDeck();
std::vector<std::vector<Card>> Logic::userHands {};
std::vector<bool> Logic::doubleDownTracker {};
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
            deck.emplace_back( (rank % 2 == 0)?1:10 , suits[suit] ); // rank
        }
    }
    return deck;
}

void Logic::payPlayer(int amount)
{
    currentMoney += amount;
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

bool Logic::makeInsuranceBet()
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
    userHands.push_back(newUserHand);   // adding the hand once full w/ cardz
    doubleDownTracker.push_back(false); // add "false" value to doubleDownTracker
}

unsigned short Logic::doDealerActions()
{
    bool dealerHasAce = hasAce(dealerHand);
    unsigned short currentDealerValue = dealerHand.front() + dealerHand.back();
    if (dealerHasAce)
    {
        // add ace offset
        currentDealerValue += 10;
    }

    while (currentDealerValue < 17) // dealer hits until 17 or more
    {
        dealerHand.push_back(deck.back());
        deck.pop_back();
        currentDealerValue = dealerHand.back() + currentDealerValue;
        // if the dealer's hit results in a "bust" if ace = 11, remove offset
        if (currentDealerValue > 21 && dealerHasAce)
        {
            currentDealerValue -= 10;
            dealerHasAce = false;
        }
    }

    return currentDealerValue;
}

void Logic::calculatePayout()
{
    /* NOTE: payouts are 3:2 */
    const float PAYOUT_RATE = 1.5;

    // if the dealer has blackjack, check if player has blackjack
    if (hasBlackjack(dealerHand)) // this only matters if the dealer has blackjack initially (user only has 1 hand)
    {
        // if user has blackjack
        if (hasBlackjack(userHands.front()))
        {
            payPlayer(currentBet); // this is a tie
        }
        // otherwise do nothing
        return;
    }

    // if the dealer doesn't have blackjack, start checking user's hands
    const unsigned short dealerHandValue = calculateHandValue(dealerHand);
    for (unsigned int i = 0; i < userHands.size(); i++)
    {
        auto& hand = userHands.at(i);

        if (hasBlackjack(hand))
        {
            // do payout immediately (no double down here)
            payPlayer(currentBet * PAYOUT_RATE);
        }
        else
        {
            // factor to apply when paying player
            int doubleDownFactor = (doubleDownTracker.at(i))?2:1;

            // get the value for the current user's hand
            unsigned short currentUserHandValue = calculateHandValue(hand);

            // if bust, go to next hand
            if (currentUserHandValue > 21) continue;

            /// compare hand values here:
            // on win
            if (currentUserHandValue > dealerHandValue || dealerHandValue > 21)
            {
                payPlayer(currentBet * doubleDownFactor * PAYOUT_RATE);
            }
            // on tie
            else if (currentUserHandValue == dealerHandValue)
            {
                payPlayer(currentBet * doubleDownFactor);
            }
            // on loss (do nothing)
        }
    }

}

/// Public Getter function definitions
int Logic::getCurrentBet()
{
    return currentBet;
}

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

/// Public Helper function definitions
unsigned short Logic::calculateHandValue(const std::vector<Card>& hand)
{
    // handValue (set to 10 if ace is there, otherwise set to 0)
    bool handHasAce = hasAce(hand);
    unsigned short handValue = (handHasAce)?10:0;

    // loop to add all of the ranks
    for (auto& card : hand)
    {
        handValue = card + handValue;
    }

    // if ace is in hand, then remove offset if value > 21
    if (handHasAce && handValue > 21)
    {
        handValue -= 10;
    }

    return handValue;
}


