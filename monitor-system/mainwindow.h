#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <QString>
#include <QTableWidgetItem>
#include <QtConcurrent>

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
    void tab_process(void);
    void set_processPID(const QString sdir, int i);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
