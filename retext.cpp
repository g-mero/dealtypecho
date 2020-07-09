#include "retext.h"

reText::reText()
{

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

//对按钮进行转化
void reText::fixButton(QString *text)
{
    int i = 0;
    while(i > -1) {
        int a = text->indexOf("[button color=", i);
        int b = text->indexOf("[/button]", a);
        if(a == -1 || b == -1) return;
        i = b + 8;
        QString tmpStr = text->mid(a, b - a);
        text->remove(a, b - a + 9);
        int j = tmpStr.indexOf("color=") + 7;
        int t = tmpStr.indexOf('"', j);
        QString color = tmpStr.mid(j, t - j);
        if(color == "info") color = "blue";
        else if(color == "success") color = "green";
        else if(color == "warning") color = "orange";
        else if(color == "danger") color = "red";
        else color = "";
        j = tmpStr.indexOf("url=", j) + 5;
        t = tmpStr.indexOf('"', j);
        qDebug() << t << "   "<< tmpStr;
        QString url = tmpStr.mid(j, t - j);
        j = tmpStr.indexOf("]", j) + 1;
        QString content = tmpStr.mid(j);
        text->insert(a, "{% btn '" + url + "'," + content +
                     ",far fa-hand-point-right," + color + " larger %}");
    }
}

reText::~reText()
{

}
