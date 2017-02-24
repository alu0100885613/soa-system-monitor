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
    const QStringList headers  = (QStringList() << "PID" << "STATUS" << "TRHEADS" << "OWNER" << "CMDLINE" );
    dir.setNameFilters( regexp );
    ui->processTable->setRowCount(dir.count());
    ui->processTable->setColumnCount(5);
    ui->processTable->setHorizontalHeaderLabels( headers );


    int i = 0;
    for(auto directory: dir.entryList()){
        if( set_processInfo( directory,i ) == -1 )
            return;
        i++;
    }

    ui->processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

int MainWindow::set_processInfo( const QString sdir,int i )
{

    const QString sstate = "State:";
    const QString sthread = "Threads:";
    const QString suid = "Uid:";

    QTableWidgetItem *item = new QTableWidgetItem(sdir);
    ui->processTable->setItem(i,0,item);

    QFile file_status("/proc/"+sdir+"/status");
    if ( !file_status.open(QIODevice::ReadOnly) ){
        QMessageBox::warning( this,tr("Warning"),tr("No se ha podido abrir el archivo de status en /proc/") );
        return -1;
    } else {
        QTextStream in (&file_status);
        QString line;

        do{
            line = in.readLine();

            if( line.contains(sstate) ){
               QTableWidgetItem *item = new QTableWidgetItem(line);
               ui->processTable->setItem(i,1,item);
            }

            if( line.contains(sthread) ){
               QTableWidgetItem *item = new QTableWidgetItem(line);
               ui->processTable->setItem(i,2,item);
            }

            if( line.contains(suid) ){
               QTableWidgetItem *item = new QTableWidgetItem(line);
               ui->processTable->setItem(i,3,item);
            }

        } while ( !line.isNull() );
    }
    file_status.close();

    QFile file_cmdline("/proc/"+sdir+"/cmdline");
    if ( !file_cmdline.open(QIODevice::ReadOnly) ){
        QMessageBox::warning( this,tr("Warning"),tr("No se ha podido abrir el archivo de cmdline en /proc/") );
        return -1;
    } else {
        QTextStream in (&file_cmdline);
        const QString content = in.readAll();

        QTableWidgetItem *item = new QTableWidgetItem(content);
        ui->processTable->setItem(i,4,item);

    }
    file_cmdline.close();

    return 0;
}
