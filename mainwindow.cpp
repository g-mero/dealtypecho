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
    ui->tabWidget->setCurrentIndex(0);
    ui->lineEdit_dbport->setValidator( new QIntValidator(0, 9999999, this) );
    //获取用户桌面路径
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    ui->lineEdit_outpath->setText(desktopPath);
    //读取config
    QStringList info = Config().Get();
    if(!info.at(0).isEmpty() && !info.at(info.length()-1).isEmpty()) {
        ui->lineEdit_dbadd->setText(info.at(0));
        ui->lineEdit_dbport->setText(info.at(1));
        ui->lineEdit_dbname->setText(info.at(2));
        ui->lineEdit_prefix->setText(info.at(3));
        ui->lineEdit_dbuser->setText(info.at(4));
        ui->lineEdit_dbpass->setText(info.at(5));
        ui->lineEdit_outpath->setText(info.at(6));
    }



    //检查更新
    update = new softUpdate();
    update->run();

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
        QMessageBox::information(this, "关于", "作者: gmero\n博客：https://gmero.com\n版本：develop 0.0.1\n");
        qDebug() << "cc";
    });

    connect(ui->pushButton_gen, &QPushButton::clicked, this, [=](){
        ui->statusbar->showMessage(tr("Database Connecting..."));
        QString dbAdd = ui->lineEdit_dbadd->text();
        QString dbName = ui->lineEdit_dbname->text();
        QString dbUser = ui->lineEdit_dbuser->text();
        QString dbPass = ui->lineEdit_dbpass->text();
        QString outPath = ui->lineEdit_outpath->text();
        QString preFix = ui->lineEdit_prefix->text();
        int dbPort = ui->lineEdit_dbport->text().toInt();
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        qDebug()<<db.drivers();
        db.setHostName(dbAdd);//数据库地址
        db.setDatabaseName(dbName);//现有的数据库名称
        db.setPort(dbPort);//数据库端口
        db.setUserName(dbUser);//用户名
        db.setPassword(dbPass);//密码
        if (!db.isValid())
        {
            ui->statusbar->showMessage(tr("Failed to connect."), 4000);
            QMessageBox::critical(0, QObject::tr("Database"), db.lastError().text());
        }

        if(!db.open())
        {
            ui->statusbar->showMessage(tr("Failed to connect."), 4000);
            QMessageBox::critical(0, QObject::tr("Database"), db.lastError().text());
            return ;
        }
        else
        {
            bool a[5];
            a[0] = ui->cb_dofixpound->isChecked();
            a[1] = ui->cb_dofixenter->isChecked();
            a[2] = ui->cb_dofixnodes->isChecked();
            a[3] = ui->cb_dofixalbum->isChecked();
            a[4] = ui->cb_dofixbutton->isChecked();
            showinfo * s = new showinfo(db, this, preFix, outPath, a);
            s->show();
            s->getdate();
            ui->statusbar->showMessage(tr("Database connect sucessed!!!"));
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_confirm_clicked()
{
    if(ui->cbox_saveset->isChecked()) {
        QString dbAdd = ui->lineEdit_dbadd->text();
        QString dbName = ui->lineEdit_dbname->text();
        QString dbUser = ui->lineEdit_dbuser->text();
        QString dbPass = ui->lineEdit_dbpass->text();
        QString outPath = ui->lineEdit_outpath->text();
        QString preFix = ui->lineEdit_prefix->text();
        QStringList info;
        info << dbAdd << ui->lineEdit_dbport->text() << dbName << preFix << dbUser << dbPass << outPath;
        Config().Set(info);
    }
    QMessageBox::information(this, "消息", tr("Database set up successfully !!!"));

}

void MainWindow::on_pushButton_quit_2_clicked()
{
    close();
}

void MainWindow::on_cb_dofixenter_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->cb_dofixalbum->setChecked(false);
        ui->cb_dofixalbum->setEnabled(false);
        ui->cb_dofixbutton->setChecked(false);
        ui->cb_dofixbutton->setEnabled(false);
        ui->cb_dofixnodes->setChecked(false);
        ui->cb_dofixnodes->setEnabled(false);
    }
    else if(arg1 == 2) {
        ui->cb_dofixalbum->setEnabled(1);
        ui->cb_dofixbutton->setEnabled(1);
        ui->cb_dofixnodes->setEnabled(1);
    }
}
