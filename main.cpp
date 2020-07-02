#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFile file(":/qss/global.qss"); //工程控件样式表需要在qss/abc.qss里面
    file.open(QFile::ReadOnly);
    QLatin1String styleSheet = QLatin1String(file.readAll());
    qDebug() << styleSheet;
    a.setStyleSheet(styleSheet);
    file.close();
    w.show();
    return a.exec();
}
