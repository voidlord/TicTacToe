#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>

#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

struct Settings {
	int Width = 3;
	int Height = 3;
	int Strike = 3;
	int maxButtonSize = 120;
};

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void generateBoard();
	char checkBoard();
	void clearBoard();

private:
	Ui::MainWindow *ui;

	struct Settings settings;

	QString playerName1;
	QString playerName2;

	bool turn;
	bool lockButtons;

	vector<vector<QPushButton*>> board;
	QGridLayout* gridLayout;
};
#endif // MAINWINDOW_H
