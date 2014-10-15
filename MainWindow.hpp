#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPixmap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
private:
	Q_OBJECT
	QPixmap mPixmap;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void setImage(QImage img);
private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
