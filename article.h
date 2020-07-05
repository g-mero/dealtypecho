#ifndef ARTICLE_H
#define ARTICLE_H

#include <retext.h>
#include <QDate>

class article
{
public:
    article();
    QString cid;
    QString title = "默认标题";
    QString text = "";
    QString created = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString modified = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString tags = "";
    QString categories = "\n - 杂项"; //hexo的分类只能有一个，但支持子分类
    QString save(QString outpath);
};

#endif // ARTICLE_H
