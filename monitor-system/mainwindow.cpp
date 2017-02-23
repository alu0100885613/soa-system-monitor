#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    switch (index) {
    case 0: tab_process();
        break;
    default:
        break;
    }
}

void MainWindow::tab_process()
{
    QDir dir(".");

    dir.setFilter(QDir::Dirs);
    const QStringList regexp("[0-9]+");
    dir.setNameFilters(regexp);

    QStringList directories = dir.entryList();
    qDebug() << directories;
    ui->processTable->setRowCount(dir.count());


}
