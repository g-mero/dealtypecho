#include "article.h"

#include<QFile>

article::article()
{

}

QString article::save(QString outPath)
{
    QFile file(outPath + "/" + cid + ".md");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(text.toUtf8());
    QString filename = file.fileName();
    file.close();
    return filename;
}
