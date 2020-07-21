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
    for(auto& userHand: Logic::userHands)
    {
        Hand* newUserHand = new Hand;
        ui->UserHandLayout->addWidget(newUserHand);
        for(Card& card: userHand)
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

void MainWindow::on_BetButton_clicked()
{
    // put input bet through Logic
    if ( Logic::doBet(ui->BetInputter->text()) )
    {
        ui->TotalMoneyLabel->setText( QString::number(Logic::getCurrentMoney()) );
        ui->BetInputter->setText("");
        // disable bet button
        ui->BetButton->setEnabled(false);

        /// deal cards to user and dealer
        Logic::dealCards();
        /// update ui to have user cards and dealer cards
        showCards();
        // show ui
        ui->PlayWidget->setHidden(false);
    }
}

