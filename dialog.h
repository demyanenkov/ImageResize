#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QImage>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QString>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
signals:
    void imageLoad(QImage image);
public slots:
    void imgRub(QPoint TL,double zm);

private:
    Ui::Dialog *ui;

private slots:
    void on_last_clicked();
    void on_first_clicked();
    void on_prop_clicked(bool checked);
    void on_preview_clicked(bool checked);
    void on_next_clicked();
    void on_prev_clicked();
    void on_save_clicked();
    void on_open_clicked();
    void on_setIn_clicked();
    void on_setOut_clicked();
    void on_rotateSize_clicked();
    void on_battonStart_clicked();
protected:
    void open(QString fileName);
    void gotoFile(int direction);
    QImage image;
    QPoint rubTL;
    double zoom;
    int index;
    enum {GO_NEXT, GO_PREV, GO_FIRST, GO_LAST};
};

#endif // DIALOG_H
