#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType< QVector<int> >("QVector<int>");
    qRegisterMetaType< Qt::Orientation >("Qt::Orientation");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    switch (index) {
    case 0: QtConcurrent::run(this,&MainWindow::tab_process);
        break;
    default:
        break;
    }
}

void MainWindow::tab_process()
{
    QDir dir("/proc");

    dir.setFilter(QDir::Dirs);
    const QStringList regexp("[0-9]*");
    const QStringList headers  = (QStringList() << "PID" << "STATUS" << "CMDLINE" << "TRHEADS" << "OWNER");
    dir.setNameFilters(regexp);
    ui->processTable->setRowCount(dir.count());
    ui->processTable->setColumnCount(5);
    ui->processTable->setHorizontalHeaderLabels(headers);

    int i = 0;
    int j = 0;
    for(auto directory: dir.entryList()){
        set_processPID(directory,i);
        i++;
    }

    ui->processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::set_processPID(const QString sdir,int i)
{
    QDir dir2("/proc/"+sdir);
    QTableWidgetItem *item = new QTableWidgetItem(sdir);
    ui->processTable->setItem(i,0,item);

}
