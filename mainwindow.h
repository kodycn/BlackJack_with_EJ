#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>

#include "Hand.h"

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

    // user hands vector (as Hand objects) (handles split games)
    std::vector<Hand> userHands {};
    // helper functions
    void showCards();
};
#endif // MAINWINDOW_H
