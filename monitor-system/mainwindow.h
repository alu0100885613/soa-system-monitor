#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "windowworker.h"

#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <QString>
#include <QTableWidgetItem>
#include <QtConcurrent>
#include <QMessageBox>
#include <QTimer>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void workRequest(void);

private slots:
    QList<QTableWidgetItem*> get_processInfo(const QString sdir);
    void futurefunction(void);
    QStringList amountOfProc(void);
    QList<QTableWidgetItem*> dataOfProc(QStringList qsl);
    void uiEditTable(void);
    void uiEditData(void);
    void uiHardware(QByteArray data);

private:
    Ui::MainWindow *ui;
    QFutureWatcher<QStringList>* fwi;
    QFutureWatcher<QList<QTableWidgetItem*>>* fwl;
    QTimer* timer;
    QThread workingThread_;
    WindowWorker windowWorker_;
};

#endif // MAINWINDOW_H
