#include "showinfo.h"
#include "ui_showinfo.h"

#include <QDebug>
#include <QMessageBox>
#include <QThread>

#include "selectdialog.h"
#include "progdilg.h"
#include "retext.h"
#include "article.h"
showinfo::showinfo( QSqlDatabase in_db, QWidget *parent, QString in_preFix, QString in_outpath, bool b[4]) :
    QWidget(parent),
    ui(new Ui::showinfo)
{
    ui->setupUi(this);
    connect(ui->btn_cancel, &QPushButton::clicked, this, &QWidget::close);
    preFix = in_preFix;
    outPath = in_outpath;
    db = in_db;
    a[0] = b[0];
    a[1] = b[1];
    a[2] = b[2];
    a[3] = b[3];
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);
//    connect(ui->btn_confirm, &QPushButton::clicked, this, [=](){
//        emit this->sendsignal(cids, mids, se);
//    });
}

showinfo::~showinfo()
{
    delete ui;
}

void showinfo::printTitle()
{
     ui->listWidget_find->clear();
    ui->listWidget_find->addItems(titleList);
}

void showinfo::getdate()
{
    QString mid;
    QSqlQuery * query = new QSqlQuery(db);
    QSqlQuery * query2 = new QSqlQuery(db);
    QString sql = "SELECT title,cid FROM typecho_contents WHERE type = 'post'";
    query->exec(sql);
    while(query->next())
    {

        QString tmp_cid = query->value("cid").toString();
        QString sql2 = "select tm.mid from " + preFix +
                "relationships ts, typecho_metas tm "
                "where tm.mid = ts.mid and tm.type = 'category' and ts.cid = " + tmp_cid;
        query2->exec(sql2);
        query2->next();
        titleList.append("cid: " + query->value("cid").toString() + " mid: " +
                         query2->value(0).toString()+ "  title: " +
                         query->value("title").toString());
        cids.append(tmp_cid);
    }
    printTitle();
    connect(ui->btn_confirm, &QPushButton::clicked, this, &showinfo::savetofile);
}

void showinfo::on_btn_select_clicked()
{
    selectdialog *s = new selectdialog(this);
    s->show();
    connect(s, &selectdialog::sendInfo, this, [&](QStringList req_cid, QStringList req_mid, int cs){
        qDebug() << "reqcid: " <<req_cid << " req_mid: "<< req_mid << " cs: " << cs;
        if(req_cid.isEmpty() && req_mid.isEmpty()) return ;
        if(cs == 1) {
            if(!req_cid.at(0).isEmpty()) {
                for(int i = 0; i < titleList.length(); i++) {
                    for(int j = 0; j<req_cid.length(); j++) {
                        if(titleList.at(i).contains("cid: " + req_cid.at(j))) {
                            cids.removeAt(i);
                            titleList.removeAt(i--);
                        }
                    }
                }
            }
            if(!req_mid.at(0).isEmpty()) {
                for(int i = 0; i < titleList.length(); i++) {
                    for(int j = 0; j<req_mid.length(); j++) {
                        if(titleList.at(i).contains("mid: " + req_mid.at(j))) {
                            titleList.removeAt(i);
                            cids.removeAt(i--);
                        }
                    }
                }
            }
        } else {
            if(!req_cid.at(0).isEmpty()) {
                cids = req_cid;
                for(int i = 0; i < titleList.length(); i++) {
                    for(int j = 0; j<req_cid.length(); j++) {
                        if(!titleList.at(i).contains("cid: " + req_cid.at(j))) titleList.removeAt(i--);
                    }
                }
            }
            else if(!req_mid.at(0).isEmpty()) {
                for(int i = 0; i < titleList.length(); i++) {
                    for(int j = 0; j<req_mid.length(); j++) {
                        if(!titleList.at(i).contains("mid: " + req_mid.at(j))) {
                            titleList.removeAt(i);
                            cids.removeAt(i--);
                        }
                    }
                }
            }
        }
        qDebug() << "cids: " << cids;
        printTitle();
    });
}

void showinfo::savetofile()
{
    progdilg * progressdlg = new progdilg(this);


    progressdlg->setRange(0, cids.length());
    for(int i = 0; i < cids.length(); i++)
    {
        bool if_continue = false;
        QString cid = cids.at(i);
        QString sql2 = "select type, name, slug, tm.mid from `" + preFix +
                "relationships` ts, typecho_metas tm where tm.mid = ts.mid and ts.cid = " + cid;
        QString sql3 = "SELECT cid FROM " + preFix +
                "contents WHERE type = 'post'";
        QString sql4 = "SELECT title,created,modified,text FROM " + preFix +
                "contents WHERE cid = " + cid;
        QSqlQuery query2(db), query(db);

        article essay;
        essay.cid = cid;
        if(query2.exec(sql4)){
            query2.next();
            essay.title = query2.value("title").toString();
            essay.created = QDateTime::fromTime_t(query2.value("created").toUInt())
                    .toString("yyyy-MM-dd hh:mm:ss");
            essay.modified = QDateTime::fromTime_t(query2.value("modified").toUInt())
                    .toString("yyyy-MM-dd hh:mm:ss");
            essay.text = query2.value("text").toString();
        } else {
            QMessageBox::critical(this, QObject::tr("Database"),
                                  tr("Nothing catched, check your prefix setting\n"
                                     "or contact author(gmero@foxmail.com)."));
        }
        if(query2.exec(sql2)){
            while(query2.next())
            {
                if(query2.value("type").toString() == "category") {
                    essay.categories = "";
                    if(query.exec("SELECT name FROM typecho_metas WHERE "
                                   "mid = ( SELECT parent FROM typecho_metas WHERE mid = " +
                                   query2.value("mid").toString() + ")")) {
                        query.next();
                        if(!query.value("name").toString().isEmpty())
                        essay.categories += "\n - " + query.value("name").toString();
                    }
                    essay.categories += "\n - " + query2.value("name").toString();
                } else {
                    essay.tags += "\n - " + query2.value("name").toString();
                }
            }
        } else {
            qDebug() << "2222";
        }
        if(if_continue) {
            continue;
        }
        QString text = "";
        text += "---\n"
                "layout: posts"
                "\ntitle: " + essay.title +
                "\ndate: " + essay.created +
                "\nupdated: " + essay.modified +
                "\ntags: " + essay.tags +
                "\ncategories: " + essay.categories +
                "\n---\n";
        text += essay.text;
        text.remove("<!--markdown-->");
        if(a[0]) reText().fixPound(&text);
        if(a[1]) reText().fixEnter(&text);
        if(a[2]) reText().transSpeNodes(&text);
        if(a[3]) reText().transAlbums(&text);
        essay.text = text;
        progressdlg->addText(essay.save(outPath) + tr(" create successful."));
        progressdlg->setValue(i+1);
        if(progressdlg->wasCanceled()) return ;
        while(progressdlg->wasPaused()) {QThread::msleep(10);}
    }
    qDebug() << "complete!!!";
}
