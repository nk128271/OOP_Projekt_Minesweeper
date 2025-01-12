#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSoundEffect>
#include <QMouseEvent>
#include <vector>
#include "cell.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setDifficulty(int difficulty);
    void showDifficultyDialog();

private:
    Ui::MainWindow* ui;

    int rows;
    int cols;
    int totalMines;
    int revealedCells;

    std::vector<std::vector<BaseCell*>> grid;

    QSoundEffect* explosionPlayer;
    QSoundEffect* winPlayer;

    void initGame();
    void placeMines();
    void calculateNeighborMines();
    void revealCell(int r, int c);
    void handleClick(int r, int c);
    void toggleFlag(int r, int c);
    void gameOver(bool won);


    void cleanupGrid();
};
class FlaggableButton : public QPushButton {
    Q_OBJECT

public:
    explicit FlaggableButton(QWidget *parent = nullptr) : QPushButton(parent) {}

signals:
    void rightClicked();

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::RightButton) {
            emit rightClicked();
        } else {
            QPushButton::mousePressEvent(event);
        }
    }
};
#endif
