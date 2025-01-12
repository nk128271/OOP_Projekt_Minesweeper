#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cell.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSoundEffect>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: lightblue;");

    explosionPlayer = new QSoundEffect(this);
    explosionPlayer->setSource(QUrl::fromLocalFile(":/sounds/vineboom.wav"));

    winPlayer = new QSoundEffect(this);
    winPlayer->setSource(QUrl::fromLocalFile(":/sounds/hihihiha.wav"));

    showDifficultyDialog();
}

MainWindow::~MainWindow() {
    cleanupGrid();
    delete ui;
    delete explosionPlayer;
    delete winPlayer;
}

void MainWindow::setDifficulty(int difficulty) {
    switch (difficulty) {
    case 0:
        rows = 10;
        cols = 10;
        totalMines = 10;
        break;
    case 1:
        rows = 10;
        cols = 10;
        totalMines = 25;
        break;
    case 2:
        rows = 10;
        cols = 10;
        totalMines = 35;
        break;
    }

    revealedCells = 0;
    cleanupGrid();
    initGame();
}

void MainWindow::showDifficultyDialog() {
    QMessageBox difficultyBox;
    difficultyBox.setWindowTitle("Select Difficulty");
    difficultyBox.setText("Choose a difficulty level:");
    QPushButton *easyButton = difficultyBox.addButton("Easy", QMessageBox::ActionRole);
    QPushButton *hardButton = difficultyBox.addButton("Hard", QMessageBox::ActionRole);
    QPushButton *impossibleButton = difficultyBox.addButton("Impossible", QMessageBox::ActionRole);

    difficultyBox.exec();

    if (difficultyBox.clickedButton() == easyButton) {
        setDifficulty(0);
    } else if (difficultyBox.clickedButton() == hardButton) {
        setDifficulty(1);
    } else if (difficultyBox.clickedButton() == impossibleButton) {
        setDifficulty(2);
    }
}

void MainWindow::initGame() {
    auto* layout = new QGridLayout(ui->centralwidget);
    grid.resize(rows, std::vector<BaseCell*>(cols, nullptr));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QPushButton* button = new FlaggableButton(this);
            button->setFixedSize(60, 60);
            button->setStyleSheet("background-color: lightgray; border: 1px solid black;");
            layout->addWidget(button, r, c);

            grid[r][c] = new NumberCell(button, 0);

            connect(button, &FlaggableButton::clicked, [this, r, c]() { handleClick(r, c); });
            connect(static_cast<FlaggableButton*>(button), &FlaggableButton::rightClicked, [this, r, c]() { toggleFlag(r, c); });
        }
    }

    placeMines();
    calculateNeighborMines();
}

void MainWindow::placeMines() {
    int placedMines = 0;
    while (placedMines < totalMines) {
        int r = QRandomGenerator::global()->bounded(rows);
        int c = QRandomGenerator::global()->bounded(cols);

        if (dynamic_cast<MineCell*>(grid[r][c]) == nullptr) {
            delete grid[r][c];
            grid[r][c] = new MineCell(grid[r][c]->button);
            ++placedMines;
        }
    }
}

void MainWindow::calculateNeighborMines() {
    const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (dynamic_cast<MineCell*>(grid[r][c])) continue;

            int count = 0;
            for (int d = 0; d < 8; ++d) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    if (dynamic_cast<MineCell*>(grid[nr][nc])) {
                        ++count;
                    }
                }
            }

            auto* numberCell = dynamic_cast<NumberCell*>(grid[r][c]);
            if (numberCell) {
                numberCell->neighboringMines = count;
            }
        }
    }
}

void MainWindow::revealCell(int r, int c) {
    if (r < 0 || r >= rows || c < 0 || c >= cols || grid[r][c]->isRevealed || grid[r][c]->isFlagged) {
        return;
    }

    grid[r][c]->reveal();

    if (dynamic_cast<MineCell*>(grid[r][c])) {
        explosionPlayer->play();
        gameOver(true);
        return;
    }

    auto* numberCell = dynamic_cast<NumberCell*>(grid[r][c]);
    if (numberCell && numberCell->neighboringMines == 0) {
        const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int d = 0; d < 8; ++d) {
            revealCell(r + dr[d], c + dc[d]);
        }
    }

    if (++revealedCells == rows * cols - totalMines) {
        winPlayer->play();
        gameOver(false);
    }
}

void MainWindow::handleClick(int r, int c) {
    revealCell(r, c);
}

void MainWindow::toggleFlag(int r, int c) {
    if (grid[r][c]->isRevealed) return;

    grid[r][c]->isFlagged = !grid[r][c]->isFlagged;
    grid[r][c]->button->setText(grid[r][c]->isFlagged ? "🚩" : "");
}

void MainWindow::gameOver(bool lost) {
    if (lost) {
        explosionPlayer->play();
        QMessageBox::about(this, "Game Over", "You Lose!");
    } else {
        winPlayer->play();
        QMessageBox::about(this, "Game Over", "You Win!");
    }

    qApp->quit();
}

void MainWindow::cleanupGrid() {
    for (auto& row : grid) {
        for (auto* cell : row) {
            delete cell;
        }
    }
    grid.clear();
}

