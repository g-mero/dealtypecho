#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QtSql/QSqlError>
#include <QStandardPaths>
#include <QIntValidator>
#include <QTranslator>

#include "config.h"
#include "showinfo.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QTranslator translator;
    ui->setupUi(this);
    translator.load(":/languages/dealtypecho_zh_CN.qm");
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
    ui->lineEdit_dbport->setValidator( new QIntValidator(0, 9999999, this) );
    //获取用户桌面路径
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    ui->lineEdit_outpath->setText(desktopPath);
    //读取config
    QStringList info = Config().Get();
    if(!info.at(0).isEmpty()) {
        ui->lineEdit_dbadd->setText(info.at(0));
        ui->lineEdit_dbport->setText(info.at(1));
        ui->lineEdit_dbname->setText(info.at(2));
        ui->lineEdit_dbuser->setText(info.at(3));
        ui->lineEdit_dbpass->setText(info.at(4));
        ui->lineEdit_outpath->setText(info.at(5));
    }
    connect(ui->pushButton_quit, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->pushButton_getpath, &QPushButton::clicked, this, [=](){
        QString outPath = QFileDialog::getExistingDirectory(this,"选择输出文件夹", ui->lineEdit_outpath->text());
        qDebug() << outPath;
        if(outPath.isEmpty()) return;
        else {
            ui->lineEdit_outpath->setText(outPath);
        }
    });
    connect(ui->actionabout, &QAction::triggered, this, [=](){
        QMessageBox::information(this, "关于", "作者: gmero\n博客：https://gemro.com\n版本：develop 0.0.1\n");
        qDebug() << "cc";
    });
    connect(ui->pushButton_gen, &QPushButton::clicked, this, [=](){

        ui->statusbar->showMessage(tr("Database Connecting..."));
        QString dbAdd = ui->lineEdit_dbadd->text();
        QString dbName = ui->lineEdit_dbname->text();
        QString dbUser = ui->lineEdit_dbuser->text();
        QString dbPass = ui->lineEdit_dbpass->text();
        QString outPath = ui->lineEdit_outpath->text();
        int dbPort = ui->lineEdit_dbport->text().toInt();
        QStringList info;
        info << dbAdd << ui->lineEdit_dbport->text() << dbName <<dbUser << dbPass << outPath;
        Config().Set(info);
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        qDebug()<<db.drivers();
        db.setHostName(dbAdd);//数据库地址
        db.setDatabaseName(dbName);//现有的数据库名称
        db.setPort(dbPort);//数据库端口
        db.setUserName(dbUser);//用户名
        db.setPassword(dbPass);//密码
        if (!db.isValid())
        {
            ui->statusbar->showMessage(tr("Failed to connect."), 2000);
            QMessageBox::critical(0, QObject::tr("Database"), db.lastError().text());
        }

        if(!db.open())
        {
            ui->statusbar->showMessage(tr("Failed to connect."), 2000);
            QMessageBox::critical(0, QObject::tr("Database"), db.lastError().text());
            return ;
        }
        else
        {
            ui->statusbar->showMessage(tr("Database connect sucessed!!!"));
            QStringList titleList;
            QSqlQuery * query = new QSqlQuery;
            QString sql = "SELECT title FROM typecho_contents WHERE type = 'post'";
            query->exec(sql);
            while(query->next())
            {
                titleList.append(query->value(0).toString());
            }
            showinfo * s = new showinfo();
            s->printTitle(titleList);
            s->setWindowModality(Qt::ApplicationModal);
            s->setAttribute(Qt::WA_DeleteOnClose);
            connect(s,&showinfo::sendsignal, this, [=](int a){
                qDebug() << a;
                QString sql2 = "SELECT text FROM typecho_contents WHERE type = 'post'";
                QString sql3 = "SELECT text FROM typecho_contents WHERE created = 1592305500";
                query->exec(sql3);
                while(query->next())
                {
                    sql3 = query->value(0).toString();
                }
                reText().toHexoButterfly(&sql3);
                QFile file(outPath + "/bf5tobf2.md");
                file.open(QIODevice::WriteOnly | QIODevice::Text);
                file.write(sql3.toUtf8());
                file.close();
            });
            s->show();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

