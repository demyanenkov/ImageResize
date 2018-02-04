#ifndef IMAGEEDIT_H
#define IMAGEEDIT_H

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QImage>
#include <QPainter>

class ImageEdit : public QWidget
{
    Q_OBJECT
public:
    explicit ImageEdit(QWidget *parent = 0);

signals:
    void imgRub(QPoint TL,double zm);
    void midButton();
    void rightButton();
    void upWheel();
    void downWheel();
public slots:
    void setHeight(int h);
    void setWidth(int w);
    void setImage(QImage img);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    QImage image;
    QImage imagePaint;
    QPoint rubTL,rubDR,rubCenter;
    QPoint center, base;
    QSize rub,sizeWidget,sizeImage;
    double zoom, newZoom,zoomBase,zoomAll;
    bool zoomChanged;
    int fMove; // признак перемещения мышью
    enum {
        NO_MOVE,
        MOVE_PAINT,
        MOVE_TL, MOVE_T, MOVE_TR, MOVE_R,
        MOVE_DR, MOVE_D, MOVE_DL, MOVE_L
    };
    QPoint moveStart, moveCurrent, moveEnd, rubMove,rubZoom;

};

#endif // IMAGEEDIT_H
