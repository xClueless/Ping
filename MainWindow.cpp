#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

void MainWindow::setImage(QImage img)
{
	mPixmap = QPixmap::fromImage(img);
	ui->imageLabel->setPixmap(mPixmap);
}

MainWindow::~MainWindow()
{
	delete ui;
}
