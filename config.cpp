#include "config.h"
#include <QtCore/QtCore>
#include <QDebug>

Config::Config(QString qstrfilename)
{
    if (qstrfilename.isEmpty())
    {
        m_qstrFileName = QCoreApplication::applicationDirPath() + "/Config.ini";
    }
    else
    {
        m_qstrFileName = qstrfilename;
    }

    m_psetting = new QSettings(m_qstrFileName, QSettings::IniFormat);
    qDebug() << m_qstrFileName;
}
Config::~Config()
{
    delete m_psetting;
    m_psetting = 0;
}
void Config::Set(QStringList info)
{
    QString dbAdd = info.at(0);
    QString dbPort = info.at(1);
    QString dbName = info.at(2);
    QString dbUser = info.at(3);
    QString dbPass = info.at(4);
    QString outPath = info.at(5);
    m_psetting->setValue("/MySql/ipAddress", dbAdd.isEmpty() ? "localhost" : dbAdd);
    m_psetting->setValue("/MySql/dbPort", dbPort.isEmpty() ? "3306" : dbPort);
    m_psetting->setValue("/MySql/dbName", dbName.isEmpty() ? "dbName" : dbName);
    m_psetting->setValue("/MySql/dbUser", dbUser.isEmpty() ? "dbUser" : dbUser);
    m_psetting->setValue("/MySql/dbPass", dbPass.isEmpty() ? "password" : dbPass);
    if(!outPath.isEmpty()) {
        m_psetting->setValue("/OUT/outPath", outPath);
    }

}

QStringList Config::Get()
{
    QStringList info;
    info << m_psetting->value("/MySql/ipAddress").toString() << m_psetting->value("/MySql/dbPort").toString()
         << m_psetting->value("/MySql/dbName").toString() << m_psetting->value("/MySql/dbUser").toString()
         << m_psetting->value("/MySql/dbPass").toString() << m_psetting->value("/OUT/outPath").toString();
    return info;
}
