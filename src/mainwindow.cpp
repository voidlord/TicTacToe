#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->turn = 0;
	this->lockButtons = true;

	this->setWindowTitle("Tic-Tac-Toe");
	this->setFixedSize(400, 400);

	ui->controlLayout->setAlignment(Qt::AlignCenter);

	ui->playerLayout1->setAlignment(Qt::AlignHCenter);
	ui->playerLayout2->setAlignment(Qt::AlignHCenter);

	ui->boardSizeLayout->setAlignment(Qt::AlignLeft);
	ui->strikeSettingLayout->setAlignment(Qt::AlignLeft);

	ui->boardSettingsLayout->setAlignment(Qt::AlignRight);

	ui->boardSizeX->setText(QString::number(this->settings.Height));
	ui->boardSizeY->setText(QString::number(this->settings.Width));
	ui->strikeBox->setText(QString::number(this->settings.Strike));

	connect(ui->saveSettingsButton, &QPushButton::pressed, [=](){
		bool error = false;

		bool ok;

		QPalette okPalette = ui->boardSizeX->palette();
		okPalette.setColor(ui->boardSizeX->foregroundRole(), Qt::black);

		QPalette errorPalette = ui->boardSizeX->palette();
		errorPalette.setColor(ui->boardSizeX->foregroundRole(), Qt::red);

		int sizeX = ui->boardSizeX->text().toInt(&ok);
		if (!ok) {
			ui->boardSizeX->setPalette(errorPalette);
			error = true;
		} else {
			ui->boardSizeX->setPalette(okPalette);
		}

		int sizeY = ui->boardSizeY->text().toInt(&ok);
		if (!ok) {
			ui->boardSizeY->setPalette(errorPalette);
			error = true;
		} else {
			ui->boardSizeY->setPalette(okPalette);
		}

		int strike = ui->strikeBox->text().toInt(&ok);
		if (!ok) {
			ui->strikeBox->setPalette(errorPalette);
			error = true;
		} else {
			ui->strikeBox->setPalette(okPalette);
		}

		if (!error) {
			this->settings.Height = sizeX;
			this->settings.Width = sizeY;
			this->settings.Strike = strike;
		}
	});

	connect(ui->playButton, &QPushButton::pressed, [&](){
		if (!ui->playerNameEdit1->text().isEmpty()) {
			this->playerName1 = ui->playerNameEdit1->text();
		} else {
			this->playerName1 = "Player 1";
		}
		if (!ui->playerNameEdit2->text().isEmpty()) {
			this->playerName2 = ui->playerNameEdit2->text();
		} else {
			this->playerName2 = "Player 2";
		}

		// Generate board
		this->generateBoard();

		ui->mainMenuWidget->hide();
		this->centralWidget()->setLayout(this->gridLayout);

		// Actually if we dont do this, the loser will start next time :O
		//this->turn = 0;

		this->lockButtons = false;
		if (this->turn == 0) {
			this->setWindowTitle(this->playerName1);
		} else {
			this->setWindowTitle(this->playerName2);
		}
	});
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::generateBoard() {
	this->board = vector<vector<QPushButton*>>(this->settings.Height);
	for (int i = 0; i < this->settings.Height; i++) {
		this->board[i] = vector<QPushButton*>(this->settings.Width);
	}

	this->gridLayout = new QGridLayout();
	for (int i = 0; i < this->settings.Height; i++) {
		for (int j = 0; j < this->settings.Width; j++) {
			QPushButton* pushButton = new QPushButton();
			pushButton->setText("");
			pushButton->setFont(QFont("Arial", 24));
			pushButton->setMaximumSize(this->settings.maxButtonSize, this->settings.maxButtonSize);

			connect(pushButton, &QPushButton::pressed, [=](){
				if (this->lockButtons == false) {
					this->lockButtons = true;
					if (pushButton->text().isEmpty()) {
						if (this->turn == 0) {
							pushButton->setText("O");
						} else {
							pushButton->setText("X");
						}

						// Check if win conditions are met
						char c = this->checkBoard();
						if (c != ' ') {
							QMessageBox* resultMessage = new QMessageBox();
							if (c == 'T') {
								resultMessage->setText("Its a Tie!");
							} else if (c == 'O') {
								resultMessage->setText(this->playerName1 + " has won!");
							} else if (c == 'X') {
								resultMessage->setText(this->playerName2 + " has won!");
							} else {
								resultMessage->setText("Error!");
							}

							resultMessage->exec();

							this->clearBoard();
							ui->mainMenuWidget->show();
							this->setWindowTitle("Tic-Tac-Toe");

							// change turns, so next time the loser starts
							this->turn = !this->turn;

							return;
						}

						// Change window title so it matches who's turn is it
						if (this->turn == 0) {
							this->setWindowTitle(this->playerName2);
						} else {
							this->setWindowTitle(this->playerName1);
						}
						// Change turns
						this->turn = !this->turn;
					}

					this->lockButtons = false;
				}
			});

			this->board[i][j] = pushButton;
			this->gridLayout->addWidget(pushButton, i, j);
		}
	}
}

char MainWindow::checkBoard() {
	bool tie = true;

	QString prev;
	int k;
	// check rows
	for (int i = 0; i < this->settings.Height; i++) {
		prev = "";
		k = 0;
		for (int j = 0; j < this->settings.Width; j++) {
			if (QString::compare(this->board[i][j]->text(), prev) != 0) {
				k = 1;
				prev = this->board[i][j]->text();
			} else if (!this->board[i][j]->text().isEmpty()) {
				k++;
			}

			if (k == this->settings.Strike) {
				if (this->turn == 0) {
					return 'O';
				} else if (this->turn == 1) {
					return 'X';
				}
			}

			if (this->board[i][j]->text().isEmpty()) {
				tie = false;
			}
		}
	}

	// if there are no more empty tiles
	if (tie) {
		return 'T';
	}

	// check rows
	for (int i = 0; i < this->settings.Width; i++) {
		prev = "";
		k = 0;
		for (int j = 0; j < this->settings.Height; j++) {
			if (QString::compare(this->board[j][i]->text(), prev) != 0) {
				k = 1;
				prev = this->board[j][i]->text();
			} else if (!this->board[j][i]->text().isEmpty()) {
				k++;
			}

			if (k == this->settings.Strike) {
				if (this->turn == 0) {
					return 'O';
				} else {
					return 'X';
				}
			}
		}
	}

	// check diagonally from top left, to bottom right (/)
	int t = this->settings.Strike-1;
	for (int l = t; l <= this->settings.Width + this->settings.Height - 2 - t; l++) {
		prev = "";
		k = 0;
		for (int j = 0; j <= l; j++) {
			int i = l - j;
			if (i < this->settings.Height && j < this->settings.Width) {
				if (QString::compare(this->board[i][j]->text(), prev) != 0) {
					k = 1;
					prev = this->board[i][j]->text();
				} else if (!this->board[i][j]->text().isEmpty()) {
					k++;
				}

				if (k == this->settings.Strike) {
					if (this->turn == 0) {
						return 'O';
					} else {
						return 'X';
					}
				}
			}
		}
	}

	// check diagonally from top right, to bottom left (\)
	for (int l = t; l <= this->settings.Width + this->settings.Height - 2 - t; l++) {
		prev = "";
		k = 0;
		for (int j = 0; j <= l; j++) {
			int i = l - j;
			int jj = this->settings.Width-1 - j;
			if (i < this->settings.Height && jj >= 0) {
				if (QString::compare(this->board[i][jj]->text(), prev) != 0) {
					k = 1;
					prev = this->board[i][jj]->text();
				} else if (!this->board[i][jj]->text().isEmpty()) {
					k++;
				}

				if (k == this->settings.Strike) {
					if (this->turn == 0) {
						return 'O';
					} else {
						return 'X';
					}
				}
			}
		}
	}

	return ' ';
}

void MainWindow::clearBoard() {
	for (int i = 0; i < this->settings.Height; i++) {
		for (int j = 0; j < this->settings.Width; j++) {
			this->board[i][j]->deleteLater();
			this->board[i][j] = nullptr;
		}
	}
	this->gridLayout->deleteLater();
	this->gridLayout = nullptr;
}
