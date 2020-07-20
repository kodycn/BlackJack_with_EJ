#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>

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

    // user hands vector (as QHBoxLayouts) (handles split games)
    std::vector<QFrame> userHands {};
    // helper functions
    void showCards();
};
#endif // MAINWINDOW_H
