#ifndef SOFTUPDATE_H
#define SOFTUPDATE_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QMessageBox>

//网络相关头文件
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
//JSON相关头文件
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>



class softUpdate : public QWidget
{
    Q_OBJECT
public:
    explicit softUpdate(QWidget *parent = nullptr);
    ~softUpdate();
    void parse_UpdateJSON(QString str);
    void run();

private slots:
    void replyFinished(QNetworkReply *reply);

signals:

};

#endif // SOFTUPDATE_H
