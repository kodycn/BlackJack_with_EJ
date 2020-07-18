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
        std::stringstream buffer;
        // show one dealer card
        buffer << Logic::getDealerHand().back();
        ui->DealerCardFirst->setText( QString::fromStdString(buffer.str()) );
        buffer.clear();
        buffer.str(std::string());
        // show both cards for user
        buffer << Logic::getUserHand().back();
        ui->UserCardFirst->setText( QString::fromStdString(buffer.str()) );
        buffer.clear();
        buffer.str(std::string());
        buffer << Logic::getUserHand().front();
        ui->UserCardSecond->setText( QString::fromStdString(buffer.str()) );
        buffer.clear();
        buffer.str(std::string());
        // show ui
        ui->PlayWidget->setHidden(false);
    }
}
