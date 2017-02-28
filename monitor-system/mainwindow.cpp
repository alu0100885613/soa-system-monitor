#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windowworker.h"
#include "qjsonmodel.cpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fwi(new QFutureWatcher<QStringList>),
    fwl(new QFutureWatcher<QList<QTableWidgetItem*>>),
    timer(new QTimer),
    workingThread_(),
    windowWorker_()
{
    ui->setupUi(this);
    qRegisterMetaType<QByteArray>("QByteArray");
    connect(fwi,SIGNAL(finished()),this,SLOT(uiEditTable()));
    connect(fwl,SIGNAL(finished()),this,SLOT(uiEditData()));
    connect(timer,SIGNAL(timeout()),this,SLOT(futurefunction()));
    connect(this,SIGNAL(workRequest()),&windowWorker_,SLOT(doWork()));
    connect(&windowWorker_,SIGNAL(workFinished(QByteArray)),this,SLOT(uiHardware(QByteArray)));

    windowWorker_.moveToThread(&workingThread_);
    workingThread_.start();
    emit workRequest();
    futurefunction();

    timer->start(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fwi;
    delete fwl;
    delete timer;

    workingThread_.quit();
    workingThread_.wait();
}

void MainWindow::uiEditTable(void)
{
    QStringList QSLResult = fwi->future();

    const QStringList headers  = (QStringList() << "PID" << "STATUS" << "OWNER" << "THREADS" << "CMDLINE" );
    ui->processTable->setColumnCount(headers.size());
    ui->processTable->setRowCount(QSLResult.size());
    ui->processTable->setHorizontalHeaderLabels(headers);
    ui->processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void MainWindow::uiEditData(void)
{
    QList<QTableWidgetItem*> ILResult = fwl->future();
    int i = 0;

    int headersSize = ui->processTable->columnCount();
    for(auto item: ILResult){
        ui->processTable->setItem(i/headersSize,i%headersSize,item);
        i++;
    }
}

void MainWindow::uiHardware(QByteArray data){

    QJsonModel *model = new QJsonModel;
    ui->treeHw->setModel(model);
    model->loadJson(data);
}

void MainWindow::futurefunction()
{

    QFuture<QStringList> futureAOP = QtConcurrent::run(this,&MainWindow::amountOfProc);
    fwi->setFuture(futureAOP);

    QStringList QSLResult = fwi->future();
    QFuture<QList<QTableWidgetItem*>> futureDOP = QtConcurrent::run(this,&MainWindow::dataOfProc,QSLResult);
    fwl->setFuture(futureDOP);

}

QStringList MainWindow::amountOfProc()
{
    QDir dir("/proc");
    dir.setFilter(QDir::Dirs);
    dir.setSorting(QDir::LocaleAware);
    const QStringList regexp("[0-9]*");
    dir.setNameFilters( regexp );
    return dir.entryList();
}

QList<QTableWidgetItem*> MainWindow::dataOfProc(QStringList qsl)
{

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
        //algo
    } else {
        QTextStream in (&file_status);
        QString line;
        QStringList splited;

        do{
            line = in.readLine();
            splited = line.split('\t');

            if( line.contains(sstate) ){

               QTableWidgetItem *item = new QTableWidgetItem(splited[1]);
               toReturn.append(item);

            }

            if( line.contains(sthread) ){

               QTableWidgetItem *item = new QTableWidgetItem(splited[1]);
               toReturn.append(item);

            }

            if( line.contains(suid) ){

               QString uid = splited[1];
               QFile file_passwd("/etc/passwd");
               if(!file_passwd.open(QIODevice::ReadOnly)){

                    QTableWidgetItem *item = new QTableWidgetItem(uid);
                    toReturn.append(item);

               } else {

                   QTextStream _in (&file_passwd);
                   QString _line;
                   QStringList _splited;
                   bool exit = false;

                   do {

                       _line = _in.readLine();
                       _splited = _line.split(':');

                       if(_splited[2] == uid){
                           QTableWidgetItem *item = new QTableWidgetItem(_splited[0]);
                           toReturn.append(item);
                           exit = true;
                       }

                   } while(!_line.isNull() && !exit);

                   file_passwd.close();
               }
            }

        } while (!line.isNull());

        file_status.close();
    }

    QFile file_cmdline("/proc/"+sdir+"/cmdline");
    if ( !file_cmdline.open(QIODevice::ReadOnly) ){
        //algo
    } else {
        QTextStream in (&file_cmdline);
        const QString content = in.readAll();

        QTableWidgetItem *item = new QTableWidgetItem(content);
        toReturn.append(item);

        file_cmdline.close();
    }

    return toReturn;
}
