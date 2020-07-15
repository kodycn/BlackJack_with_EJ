#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QIntValidator>
#include <iostream> // std::cerr

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
    ui->TotalMoneyLabel->setText( QString::number(currentMoney) );
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BetButton_clicked()
{
    // Convert bet from ui->BetInputter to int from QString
    int receivedBet = ui->BetInputter->text().toInt();
    // Good bet received, reduce from currentMoney
    if (receivedBet <= currentMoney)
    {
        currentMoney -= receivedBet;
        ui->TotalMoneyLabel->setText( QString::number(currentMoney) );
        ui->BetInputter->setText("");
    }
}
