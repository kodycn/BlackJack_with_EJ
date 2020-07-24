#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Logic.h"


#include <QIntValidator>
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
        // DEBUG!
        std::cout << Logic::dealerHand.front() << ", " << Logic::dealerHand.back() << std::endl;
        /// update ui to have user cards and dealer cards
        showCards();
        ui->PlayWidget->setHidden(false);
        /// start enabling the buttons that apply to the player
        updateChoices();
    }
}

/// HELPER FUNCTIONS
void MainWindow::showCards()
{

    // emptying the layout beforehand

    // Note: Card isn't a widget (1)
    while ( ui->DealerHandLayout->count() > 0) {
        QLayoutItem* dealerChild = ui->DealerHandLayout->takeAt ( 0 );
        // not sure if line below needed
        if ( dealerChild->widget() != 0 ) {
            delete dealerChild->widget(); // (1)
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

        // put a box around active hand
        if (idx == Logic::activeUserHandIndex) newUserHand->setFrameStyle(QFrame::Box | QFrame::Plain);

        // add the newuserhand to the layout
        ui->UserHandLayout->addWidget(newUserHand);

        // add QLabels for the cards
        for( Card& card : Logic::userHands.at(idx) )
        {
            buffer << card;
            newUserHand->getLayout()->addWidget(
                        new QLabel(QString::fromStdString(buffer.str()))
            );
            buffer.clear();
            buffer.str(std::string());
        }
    }
}

void MainWindow::updateChoices() // this should only be called at the start of the game
{
    // first reset insurance/surrender/split buttons
    ui->InsuranceButton->setDisabled(true);
    ui->SurrenderButton->setDisabled(true);
    ui->SplitButton->setDisabled(true);

    // boolean for dealer's shown cards
    bool hasAceShown = (Logic::dealerHand.front().getRank() == 1);
    bool hasTenOrFaceShown = (Logic::dealerHand.front().getRank() >= 10);

    // hit and stand and double down are always available
    ui->HitButton->setEnabled(true);
    ui->StandButton->setEnabled(true);
    ui->DoubleDownButton->setEnabled(true);

    // insurance is available when dealer has shown an ace
    if (hasAceShown)
    {
        ui->InsuranceButton->setEnabled(true);
    }

    // surrender is available when dealer has shown ace or 10 is shown AND dealer doesn't have blackjack
    if ( (hasAceShown       && Logic::dealerHand.back().getRank() < 10) ||
         (hasTenOrFaceShown && Logic::dealerHand.back().getRank() != 1)
       )
    {
        ui->SurrenderButton->setEnabled(true);
    }

    // split is available when player has two cards of the same rank
    if ( Logic::userHands.front().front().getRank() == Logic::userHands.front().back().getRank() )
    {
        ui->SplitButton->setEnabled(true);
    }
}
