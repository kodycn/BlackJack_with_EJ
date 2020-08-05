#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>

#include "HandUI.h"
#include "Logic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class Logic;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BetButton_clicked();

    void on_HitButton_clicked();

    void on_StandButton_clicked();

    void on_InsuranceComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    // helper functions
    void showCards();       // displays cards in Logic class
    void revealDealerHand();// reveals dealer hand
    void updateChoices();   // processes cards in Logic class and enables buttons
};
#endif // MAINWINDOW_H
