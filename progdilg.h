#ifndef PROGDILG_H
#define PROGDILG_H

#include <QDialog>
#include <QProgressDialog>

namespace Ui {
class progdilg;
}

class progdilg : public QDialog
{
    Q_OBJECT

public:
    explicit progdilg(QWidget *parent = nullptr);
    ~progdilg();
    void addText(QString text);
    bool wasPaused() const;
    bool wasCanceled() const;
    int showtime = 5;
    bool shownonce;
    int maxValue = 100;

public Q_SLOTS:
    void setRange(int minimum, int maximum);
    void setValue(int progress);

protected Q_SLOTS:
    void forceShow();

private slots:
    void on_btn_cancel_clicked();

    void on_btn_pause_clicked();

    void on_prgbar_glo_valueChanged(int value);

private:
    Ui::progdilg *ui;
    QTimer *forceTimer;
    bool wasCanceled_flag = false;
    bool wasPaused_flag = false;
};

#endif // PROGDILG_H
