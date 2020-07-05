#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QFile>


#include "softupdate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_confirm_clicked();

    void on_pushButton_quit_2_clicked();

    void on_cb_dofixenter_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    softUpdate *update;

};
#endif // MAINWINDOW_H
