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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BetButton_clicked();

private:
    Ui::MainWindow *ui;

    // helper functions
    void showCards();       // displays cards in Logic class
    void updateChoices();   // processes cards in Logic class and enables buttons
};
#endif // MAINWINDOW_H
