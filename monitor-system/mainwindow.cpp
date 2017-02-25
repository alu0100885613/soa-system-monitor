#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    futurefunction();
    qRegisterMetaType< QVector<int> >("QVector<int>");
    qRegisterMetaType< Qt::Orientation >("Qt::Orientation");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::futurefunction(){

    QFuture<QStringList> futureAOP = QtConcurrent::run(this,&MainWindow::amountOfProc);
    futureAOP.waitForFinished();
    QStringList QSLResult = futureAOP.result();

    const QStringList headers  = (QStringList() << "PID" << "STATUS" << "TRHEADS" << "OWNER" << "CMDLINE" );
    ui->processTable->setColumnCount(headers.size());
    ui->processTable->setRowCount(QSLResult.size());
    ui->processTable->setHorizontalHeaderLabels(headers);

    QFuture<QList<QTableWidgetItem*>> futureDOP = QtConcurrent::run(this,&MainWindow::dataOfProc,QSLResult);
    futureDOP.waitForFinished();
    QList<QTableWidgetItem*> ILResult = futureDOP.result();

    int i = 0;
    for(auto item: ILResult){
        ui->processTable->setItem(i/headers.size(),i%headers.size(),item);
        i++;
    }

    ui->processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

QStringList MainWindow::amountOfProc(){

    QDir dir("/proc");
    dir.setFilter(QDir::Dirs);
    const QStringList regexp("[0-9]*");
    dir.setNameFilters( regexp );
    return dir.entryList();
}

QList<QTableWidgetItem*> MainWindow::dataOfProc(QStringList qsl){

    QList<QTableWidgetItem*> toReturn;
    QList<QTableWidgetItem*> toWork;

    for(auto directory: qsl){
        toWork = get_processInfo(directory);
        for(auto item: toWork){

            toReturn.push_back(item);
        }
    }
    return toReturn;
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    switch (index) {
    case 0:
        break;
    default:
        break;
    }
}


QList<QTableWidgetItem*> MainWindow::get_processInfo(const QString sdir)
{
    QList<QTableWidgetItem*> toReturn;
    const QString sstate = "State:";
    const QString sthread = "Threads:";
    const QString suid = "Uid:";

    QTableWidgetItem *item = new QTableWidgetItem(sdir);
    toReturn.push_back(item);

    QFile file_status("/proc/"+sdir+"/status");
    if ( !file_status.open(QIODevice::ReadOnly) ){

    } else {
        QTextStream in (&file_status);
        QString line;

        do{
            line = in.readLine();

            if( line.contains(sstate) ){
               QTableWidgetItem *item = new QTableWidgetItem(line);
               toReturn.append(item);
            }

            if( line.contains(sthread) ){
               QTableWidgetItem *item = new QTableWidgetItem(line);
               toReturn.append(item);
            }

            if( line.contains(suid) ){
               QTableWidgetItem *item = new QTableWidgetItem(line);
               toReturn.append(item);
            }

        } while ( !line.isNull() );
    }
    file_status.close();

    QFile file_cmdline("/proc/"+sdir+"/cmdline");
    if ( !file_cmdline.open(QIODevice::ReadOnly) ){
        QMessageBox::warning(this,tr("Warning"),tr("No se ha podido abrir el archivo de cmdline en /proc/"));
    } else {
        QTextStream in (&file_cmdline);
        const QString content = in.readAll();

        QTableWidgetItem *item = new QTableWidgetItem(content);
        toReturn.append(item);

    }
    file_cmdline.close();

    return toReturn;
}
