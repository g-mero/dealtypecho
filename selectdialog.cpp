#include "selectdialog.h"
#include "ui_selectdialog.h"


selectdialog::selectdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectdialog)
{
    ui->setupUi(this);
}

selectdialog::~selectdialog()
{
    delete ui;
}


void selectdialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button == ui->buttonBox->button(QDialogButtonBox::Ok))
    {
        QString tmp_cids = ui->lineEdit_cids->text();
        QStringList cids = tmp_cids.split(",");
        QString tmp_mids = ui->lineEdit_mids->text();
        QStringList mids = tmp_mids.split(",");
        if(ui->rb_remart->isChecked())
        emit sendInfo(cids, mids, 1);
        else emit sendInfo(cids, mids, 0);
        this->close();
    }
}

