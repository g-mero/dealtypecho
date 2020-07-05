#include "retext.h"

reText::reText()
{

}

void reText::toHexoButterfly(QString * text)
{
    // 删除<!--markdown-->
    *text = text->remove("<!--markdown-->");
    fixPound(text);
    fixEnter(text);
    transSpeNodes(text);
    transAlbums(text);
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
            if(i == -1) {
                *text += "\n{% endnote %}";
            } else {
            text->insert(i, "\n{% endnote %}");
            i += 16;
            }
        }
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
