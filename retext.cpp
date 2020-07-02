#include "retext.h"

reText::reText()
{

}

void reText::toHexoButterfly(QString * text)
{
    //添加头部
    *text = "---\nlayout: posts\ntitle: 测试\ndate: 2020-06-29 00:53:34\nupdated: 2020-06-29 10:20:50\ntags: 网站建设\ncategories: 网站建设\nkeywords: 测试,hexo\ndescription: 这只是一个测试文章\ntop_img: https://www.gmero.com/usr/themes/handsome/usr/img/sj/1.jpg\ncover: https://www.gmero.com/usr/themes/handsome/usr/img/sj/1.jpg\n---\n"
            + *text;
    // 删除<!--markdown-->
    *text = text->remove("<!--markdown-->");
    fixPound(text);
    fixEnter(text);
    transSpeNodes(text);
    transAlbums(text);
}

void reText::addHexoHead(QString *text)
{
    ;
}

// 修复#
void reText::fixPound(QString *text)
{
    *text = text->replace(QRegExp("#+\r"), "\r");
    *text = text->replace(QRegExp("#+\n"), "\n");
    // ##后无空格的加空格
    int i = 0;
    while(i != -1)
    {
        i = text->indexOf(QRegExp("#+\\S"), i);
        if(i > -1) {
            i = text->indexOf(QRegExp("[^#]"), i);
            text->insert(i, ' ');
        }
    }
}

// 删除回车符
void reText::fixEnter(QString *text)
{
    *text = text->remove(QRegExp("\r"));
}

// 对引用进行转化
void reText::transSpeNodes(QString *text)
{
    int i = 0;
    text->replace("\n!> ", "\n{% note warning %}\n");
    text->replace("\n@> ", "\n{% note default %}\n");
    text->replace("\ni> ", "\n{% note info %}\n");
    text->replace("\nx> ", "\n{% note danger %}\n");
    text->replace("\n√> ", "\n{% note success %}\n");
    while(i != -1)
    {
        i = text->indexOf("%}\n", i);
        if(i > -1) {
            i = text->indexOf("\n", i+3);
            text->insert(i, "\n{% endnote %}");
            i += 16;
        }
        //qDebug() << i;
    }
}


// 对相册进行转化
void reText::transAlbums(QString *text)
{
    int i = 0;
    QRegExp reg("\\]: https*://.+\\.jpg");
    reg.setMinimal(true);
    QStringList imgUrls, repTexts;

    while(i != -2)
    {
    i = reg.indexIn(*text, i);
    i += reg.matchedLength();
    imgUrls << reg.cap(0).remove("]: ");
    }
    reg.setPattern("\\[[1-9][0-9]{0,1}\\]: https*://.+\\.jpg");
    text->remove(reg);
    reg.setPattern("\\[[1-9][0-9]{0,1}\\]\n");
    i = 0;
    while(i != -2)
    {
    i = reg.indexIn(*text, i);
    i += reg.matchedLength();
    repTexts << reg.cap(0);
    }
    i = 0;
    while(! imgUrls.at(i).isNull()) {
        text->replace(repTexts.at(i), "(" + imgUrls.at(i) + ")\n");
        i++;
    }
    text->replace("[album]", "{% gallery %}");
    text->replace("[/album]", "{% endgallery %}");
}

reText::~reText()
{

}
