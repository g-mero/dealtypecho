#ifndef CONFIG_H
#define CONFIG_H

#include <QVariant>
#include <QSettings>

class Config
{
public:
    Config(QString qstrfilename = "");
    virtual ~Config(void);
    void Set(QStringList info);
    QStringList Get();
private:
    QString m_qstrFileName;
    QSettings *m_psetting;
};

#endif // CONFIG_H
