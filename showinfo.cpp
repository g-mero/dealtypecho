#include "showinfo.h"
#include "ui_showinfo.h"

showinfo::showinfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showinfo)
{
    ui->setupUi(this);
    connect(ui->btn_cancel, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->btn_confirm, &QPushButton::clicked, this, [=](){
        emit this->sendsignal(243177);
    });
}

showinfo::~showinfo()
{
    delete ui;
}

void showinfo::printTitle(QStringList titles)
{
    ui->listWidget->addItems(titles);
}
