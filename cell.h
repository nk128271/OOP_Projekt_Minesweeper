#ifndef CELL_H
#define CELL_H

#include <QPushButton>

class BaseCell {
public:
    explicit BaseCell(QPushButton* button)
        : isRevealed(false), isFlagged(false), button(button) {}
    virtual ~BaseCell() = default;

    virtual void reveal() = 0;

    bool isRevealed;
    bool isFlagged;
    QPushButton* button;
};

class MineCell : public BaseCell {
public:
    explicit MineCell(QPushButton* button) : BaseCell(button) {}

    void reveal() override {
        button->setText("💣");
        button->setStyleSheet("background-color: red; color: black;");
    }
};

class NumberCell : public BaseCell {
public:
    explicit NumberCell(QPushButton* button, int neighboringMines)
        : BaseCell(button), neighboringMines(neighboringMines) {}

    void reveal() override {
        isRevealed = true;
        button->setEnabled(false);
        if (neighboringMines > 0) {
            button->setText(QString::number(neighboringMines));
            button->setStyleSheet("background-color: white; color: black;");
        } else {
            button->setText(" ");
            button->setStyleSheet("background-color: white; color: black;");
        }
    }

    int neighboringMines;
};

#endif
