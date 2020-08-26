#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Logic.h"

#include <QIntValidator>
#include <QDialog>


#include <iostream> // std::cerr
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setFixedSize(800, 600);
    ui->setupUi(this);
    // Initial UI setup:
    // set BetInputter QLineEdit object to only accept numeric inputs
    ui->BetInputter->setValidator(new QIntValidator(0, 999999999, this) );
    // initial labeling of money
    ui->TotalMoneyLabel->setText( QString::number(Logic::getCurrentMoney()) );
    // disable card board
    ui->PlayWidget->setHidden(true);
}
MainWindow::~MainWindow()
{
    delete ui;
}


/// "PRIVATE SLOTS:" FUNCTIONS
void MainWindow::on_BetButton_clicked()
{
    // put input bet through Logic
    if ( Logic::doBet(ui->BetInputter->text()) )
    {
        ui->TotalMoneyLabel->setText( QString::number(Logic::getCurrentMoney()) );
        ui->BetInputter->setText("");
        // disable bet button
        // DEBUG! ui->BetButton->setEnabled(false);

        /// deal cards to user and dealer
        Logic::dealCards();
        /// update ui to have user cards and dealer cards
        showCards();
        ui->PlayWidget->setHidden(false);
        /// handle insurance UI elements
        ui->InsuranceComboBox->setCurrentIndex(0);
        ui->InsurancePaidLabel->setText("");
        ui->InsurancePaidLabel->setDisabled(true);
        /// start enabling the buttons that apply to the player
        updateChoices();

    }
}

void MainWindow::on_HitButton_clicked()
{
    // add card to active hand and then show cards
    auto& activeHand = Logic::userHands.at(Logic::activeUserHandIndex);
    activeHand.push_back( Logic::deck.back() );
    Logic::deck.pop_back();
    unsigned short newHandValue = 0;
    for (auto& card : activeHand)
    {
        newHandValue = card + newHandValue;
    }
    showCards();
    if (newHandValue >= 21)
    {
        emit on_StandButton_clicked();
    }
}

void MainWindow::on_StandButton_clicked()
{
    // move to next hand (for splits)
    Logic::activeUserHandIndex++;

    // disable all buttons if Logic::activeUserHandIndex is now invalid
    if (Logic::activeUserHandIndex >= Logic::userHands.size())
    {
        ui->HitButton->setDisabled(true);
        ui->StandButton->setDisabled(true);
        ui->SurrenderButton->setDisabled(true);
        ui->SplitButton->setDisabled(true);
        ui->DoubleDownButton->setDisabled(true);

        // Dealer acts here
        Logic::doDealerActions();
        revealDealerHand();
    }
}

void MainWindow::on_InsuranceComboBox_currentIndexChanged(int index)
{
    if (index == 1) // insurance is taken
    {
        Logic::makeInsuranceBet();
        ui->TotalMoneyLabel->setText( QString::number(Logic::getCurrentMoney()) );
        ui->TotalMoneyLabel->setEnabled(true);
        ui->InsurancePaidLabel->setText( QString("Ins: ") + QString::number(Logic::getInsuranceBet()) );
    }
    updateChoices(); // to update the rest of the choices
}


/// HELPER FUNCTIONS
void MainWindow::showCards()
{

    // emptying the dealer and user handlayouts beforehand
    while ( ui->DealerHandLayout->count() > 0) {
        QLayoutItem* dealerChild = ui->DealerHandLayout->takeAt ( 0 );
        // not sure if line below needed
        if ( dealerChild->widget() != 0 ) {
            delete dealerChild->widget();
        }
        delete dealerChild;
    }
    while ( ui->UserHandLayout->count() > 0) {
        QLayoutItem* userChild = ui->UserHandLayout->takeAt ( 0 );
        if ( userChild->widget() != 0 ) {
            delete userChild->widget();
        }
        delete userChild;
    }



    // buffer for card output stream
    std::stringstream buffer;

    // display dealer hand (only one card)
    buffer << Logic::dealerHand.front();
    ui->DealerHandLayout->addWidget(
                new QLabel(QString::fromStdString(buffer.str()))
    );
    ui->DealerHandLayout->addWidget( new QLabel("??") );
    buffer.clear();
    buffer.str(std::string());

    // display all user hands
    for(size_t idx = 0; idx < Logic::userHands.size(); idx++)
    {
        HandUI* newUserHand = new HandUI();
        unsigned short handValue = 0;

        // put a box around active hand
        if (idx == Logic::activeUserHandIndex) newUserHand->setFrameStyle(QFrame::Box | QFrame::Plain);

        // add the newuserhand to the layout
        ui->UserHandLayout->addWidget(newUserHand);

        // add QLabels for the cards (also calculate value of card)
        for( Card& card : Logic::userHands.at(idx) )
        {
            buffer << card;
            newUserHand->getLayout()->addWidget(
                        new QLabel(QString::fromStdString(buffer.str()))
            );
            buffer.clear();
            buffer.str(std::string());
            handValue = card + handValue;
        }

        // update label for the value of the user's hand
        std::string hvStr = std::to_string(handValue);
        if ( Logic::hasAce(Logic::userHands.at(idx)) )
        {
            if (handValue + 10 < 21)
            {
                std::string hvStrPlusTen = std::to_string(handValue + 10);
                newUserHand->setValueLabel( QString::fromStdString(hvStr + "/" + hvStrPlusTen) );
            }
            else if ( handValue + 10 == 21 )
            {
                newUserHand->setValueLabel("21");
            }
            else
            {
                newUserHand->setValueLabel(QString::fromStdString(hvStr));
            }
        }
        else
        {
            newUserHand->setValueLabel(QString::fromStdString(hvStr));
        }
    }
}

void MainWindow::revealDealerHand()
{
    // empty dealer hand layout beforehand
    while ( ui->DealerHandLayout->count() > 0) {
        QLayoutItem* dealerChild = ui->DealerHandLayout->takeAt ( 0 );
        // not sure if line below needed
        if ( dealerChild->widget() != 0 ) {
            delete dealerChild->widget(); // (1)
        }
        delete dealerChild;
    }

    // buffer for card output stream
    std::stringstream buffer;
    bool dealerHasAce = Logic::hasAce(Logic::dealerHand);
    unsigned short handValue = (dealerHasAce)?10:0; // offset by 10 if ace

    // add QLabels for the cards (also calculate value of card)
    for(size_t idx = 0; idx < Logic::dealerHand.size(); ++idx)
    {
        auto& card = Logic::dealerHand.at(idx);
        buffer << card;
        ui->DealerHandLayout->addWidget(
                    new QLabel(QString::fromStdString(buffer.str()))
        );
        buffer.clear();
        buffer.str(std::string());
        handValue = card + handValue;
        if (handValue > 21 && dealerHasAce)
        {
            handValue -= 10;
            dealerHasAce = false;
        }
        ui->DealerHandValueLabel->setText( QString::number(handValue) );
    }
}

void MainWindow::updateChoices() // this should only be called at the start of the game
{
    // first reset insurance/surrender/split buttons
    ui->InsuranceComboBox->setDisabled(true);
    ui->SurrenderButton->setDisabled(true);
    ui->SplitButton->setDisabled(true);

    // boolean for dealer's shown cards
    bool hasAceShown = (Logic::dealerHand.front().getRank() == 1);
    bool hasTenOrFaceShown = (Logic::dealerHand.front().getRank() >= 10);

    // check for user BJ (blackjack)? don't enable anything: continue
    if (Logic::hasBlackjack(Logic::userHands.at(0)))
    {
        // END GAME PHASE
        // --> calculate payout
        //Logic::calculatePayout();
        endPhase();
        return;
    }

    // if ace is shown and user can afford insurance, then prompt user to choose insurance option
    if (hasAceShown && ui->InsuranceComboBox->currentIndex() == 0
            && (Logic::currentMoney - (Logic::currentBet / 2)) > 0
        )
    {
        ui->InsuranceComboBox->setEnabled(true);
        return;
    }

    // hit and stand and double down are always available
    ui->HitButton->setEnabled(true);
    ui->StandButton->setEnabled(true);
    ui->DoubleDownButton->setEnabled(true);

    // surrender is available when dealer has shown ace or 10 is shown AND dealer doesn't have blackjack
    if ( (hasAceShown || hasTenOrFaceShown) && !Logic::hasBlackjack(Logic::dealerHand) )
    {
        ui->SurrenderButton->setEnabled(true);
    }

    // split is available when player has two cards of the same rank
    if ( Logic::userHands.front().front().getRank() == Logic::userHands.front().back().getRank() )
    {
        ui->SplitButton->setEnabled(true);
    }
}
void MainWindow::disableButtons()
{
    ui->BetButton->setDisabled(true);
    ui->HitButton->setDisabled(true);
    ui->StandButton->setDisabled(true);
    ui->SurrenderButton->setDisabled(true);
    ui->SplitButton->setDisabled(true);
    ui->DoubleDownButton->setDisabled(true);
    ui->InsuranceComboBox->setDisabled(true);
}

// after user has BJ or Dealer has BJ
void MainWindow::endPhase()
{
    // maybe do showcards here?

    // ORDER:
    // disable all the buttons
    // check who wins
    // calc payouts
    // open dialogue box to acknowledge win/loss (including amount won) "game over"

    disableButtons();
    Logic::calculatePayout(); // determines winner here too

    // open dialogue
    QDialog* popup = new QDialog(this);


}

