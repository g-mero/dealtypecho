#include "progdilg.h"
#include "ui_progdilg.h"

#include <QThread>
#include <QtDebug>
#include <QTimer>

progdilg::progdilg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::progdilg)
{
    ui->setupUi(this);
//    this->setWindowModality(Qt::ApplicationModal);
    this->setModal(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    forceTimer = new QTimer(this);
    shownonce = false;
    QObject::connect(forceTimer, SIGNAL(timeout()), this, SLOT(forceShow()));
    forceTimer->start(showtime);


}

void progdilg::addText(QString text)
{
    if(isModal()) {
        QCoreApplication::processEvents();
        ui->text_out->appendPlainText(text);
    }
}

void progdilg::forceShow()
{
    this->forceTimer->stop();
    if(shownonce) {
        return ;
    }
    this->show();
    shownonce = true;
}

void progdilg::setValue(int value)
{
    ui->prgbar_glo->setValue(value);
}

void progdilg::setRange(int a, int b)
{
    ui->prgbar_glo->setRange(a, b);
    maxValue = b;
}

progdilg::~progdilg()
{
    delete ui;
}

void progdilg::on_btn_cancel_clicked()
{
    wasCanceled_flag = true;
    close();
}

bool progdilg::wasCanceled() const
{
    return wasCanceled_flag;
}

bool progdilg::wasPaused() const
{
    // QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    QCoreApplication::processEvents();
    return wasPaused_flag;
}

void progdilg::on_btn_pause_clicked()
{
    if(ui->btn_pause->text() == "OK") {
        close();
    } else {
        if(wasPaused()) {
            ui->btn_pause->setText(tr("pause"));
            wasPaused_flag = false;
            addText(tr("continue..."));
        } else {
            ui->btn_pause->setText(tr("continue"));
            wasPaused_flag = true;
            addText(tr("paused!!!"));
        }
    }
}

void progdilg::on_prgbar_glo_valueChanged(int value)
{
    if (value == maxValue) {
        ui->btn_pause->setText(tr("OK"));
    }
}
