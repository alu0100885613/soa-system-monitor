#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <QString>
#include <QTableWidgetItem>
#include <QtConcurrent>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tabWidget_tabBarClicked(int index);
    QList<QTableWidgetItem*> get_processInfo(const QString sdir);
    void futurefunction(void);
    QStringList amountOfProc(void);
    QList<QTableWidgetItem*> dataOfProc(QStringList qsl);
    void uiEditTable(void);
    void uiEditData(void);

private:
    Ui::MainWindow *ui;
    QFutureWatcher<QStringList>* fwi;
    QFutureWatcher<QList<QTableWidgetItem*>>* fwl;
};

#endif // MAINWINDOW_H
