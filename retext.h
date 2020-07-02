#ifndef RETEXT_H
#define RETEXT_H

#include <QDebug>

#include <config.h>

class reText
{
public:
    reText();
    ~reText();
    void toHexoButterfly(QString *text);
    void addHexoHead(QString *text);
    void fixPound(QString *text);
    void fixEnter(QString *text);
    void transSpeNodes(QString *text);
    void transAlbums(QString *text);
};

#endif // RETEXT_H
