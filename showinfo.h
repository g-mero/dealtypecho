#ifndef SHOWINFO_H
#define SHOWINFO_H

#include <QWidget>

namespace Ui {
class showinfo;
}

class showinfo : public QWidget
{
    Q_OBJECT

public:
    explicit showinfo(QWidget *parent = nullptr);
    ~showinfo();
    int sig;
    void printTitle(QStringList titles);

signals:
    void sendsignal(int);

private:
    Ui::showinfo *ui;
};

#endif // SHOWINFO_H
