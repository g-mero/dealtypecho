#include "softupdate.h"


#include <QDesktopServices>

const QString current_version = "0.5.1_alpha";

softUpdate::softUpdate(QWidget *parent) :QWidget(parent)
{

}


void softUpdate::replyFinished(QNetworkReply *reply)
{
    QString str = reply->readAll();//读取接收到的数据
    //    qDebug() << str;
    parse_UpdateJSON(str);
}

void softUpdate::parse_UpdateJSON(QString str)
{
    QJsonParseError err_rpt;
    QJsonDocument  root_Doc = QJsonDocument::fromJson(str.toUtf8(),&err_rpt);//字符串格式化为JSON
    if(err_rpt.error != QJsonParseError::NoError)
    {
        return ;
    }
    if(root_Doc.isObject())
    {
        QJsonObject  root_Obj = root_Doc.object();
        QJsonObject PulseValue = root_Obj.value("dealtypecho").toObject();
        QString latestVerison = PulseValue.value("LatestVerison").toString();
        QString Url = PulseValue.value("Url").toString();
        QString UpdateTime = PulseValue.value("UpdateTime").toString();
        QString ReleaseNote = PulseValue.value("ReleaseNote").toString();
        if(latestVerison != current_version)
        {
            QString warningStr =  "检测到新版本!\t\t\t\t\t\t\n"
                                  "当前版本号：" + current_version +
                    "\n最新版本号：" + latestVerison +
                    "\n更新时间：" + UpdateTime +
                    "\n更新说明：" + ReleaseNote;
            int ret = QMessageBox::warning(0, "检查更新",  warningStr, "去下载", "不更新");
            if(ret == 0)    //点击更新
            {
                QDesktopServices::openUrl(QUrl(Url));
            }
        }
    }
}

void softUpdate::run()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//关联信号和槽
    QNetworkRequest quest;
    quest.setUrl(QUrl("https://blog.gmero.com/softupdate.json")); //包含最新版本软件的下载地址
    quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    manager->get(quest);
}

softUpdate::~softUpdate()
{

}
