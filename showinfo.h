#ifndef SHOWINFO_H
#define SHOWINFO_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

namespace Ui {
class showinfo;
}

class showinfo : public QDialog
{
    Q_OBJECT

public:
    explicit showinfo(QSqlDatabase in_db, QWidget *parent = nullptr, QString in_preFix = "typecho_", QString in_outpath = "", bool b[4] = {});
    ~showinfo();
    int sig;
    void getdate();
    void printTitle();

private slots:
    void on_btn_select_clicked();
    void savetofile();

private:
    Ui::showinfo *ui;
    QStringList cids, mids, titleList;
    QSqlDatabase db;
    QString preFix, outPath;
    bool a[5];
};

#endif // SHOWINFO_H
