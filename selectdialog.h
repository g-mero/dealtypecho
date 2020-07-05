#ifndef SELECTDIALOG_H
#define SELECTDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QPushButton>

namespace Ui {
class selectdialog;
}

class selectdialog : public QDialog
{
    Q_OBJECT

public:
    explicit selectdialog(QWidget *parent = nullptr);
    ~selectdialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);


 signals:
    void sendInfo(QStringList cids, QStringList mids, int);

private:
    Ui::selectdialog *ui;
};

#endif // SELECTDIALOG_H
