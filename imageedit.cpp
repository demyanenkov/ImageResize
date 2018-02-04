#include "imageedit.h"
//#include "dialog.h"

ImageEdit::ImageEdit(QWidget *parent) :
    QWidget(parent)
{
    rub.setHeight(10);
    rub.setWidth(10);
    zoom=1;
}

void ImageEdit::paintEvent(QPaintEvent *event)
{
    //using namespace Ui;
    if(image.isNull())return;
    QPainter painter(this);
    center.setX(width()/2);
    center.setY(height()/2);

    //painter.setRenderHint(QPainter::Antialiasing, true);
    if(sizeWidget.height()==0 || sizeWidget.width()==0){
        //инициализаци€
        rubCenter=center;
        sizeWidget.setHeight(height());
        sizeWidget.setWidth(width());
    }

    if(height()!=sizeWidget.height() || width()!=sizeWidget.width() || zoomChanged){
        // при изменении размера
        if(!image.isNull()){
            if(image.height()*width() > image.width()*height()){
                sizeImage.setHeight(height());
                sizeImage.setWidth(height()*image.width()/image.height());
            }
            else{
                sizeImage.setWidth(width());
                sizeImage.setHeight(width()*image.height()/image.width());
            }
            imagePaint=image.scaled(sizeImage,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        }
        // настройка ленты
        if(sizeImage.height()*rub.width() >sizeImage.width()*rub.height())
            zoom = (double)sizeImage.width()/(double)rub.width();
        else zoom = (double)sizeImage.height()/(double)rub.height();
        //rubCenter+=QPoint((width()-sizeWidget.width())/2,(height()-sizeWidget.height())/2);
        rubCenter = QPoint(width()/2, height()/2);
        sizeWidget.setHeight(height());
        sizeWidget.setWidth(width());
        zoomChanged=false;
    }
    if(!image.isNull()){
        painter.drawImage(center.x()-imagePaint.width()/2,center.y()-imagePaint.height()/2, imagePaint);
    }

    if(fMove == MOVE_PAINT){
        rubMove=moveCurrent-moveStart;
        int tmp;
        tmp = zoom*rub.width()/2-sizeImage.width()/2-rubCenter.x()+width()/2;
        if(rubMove.x()<tmp) rubMove.setX(tmp);
        tmp = -zoom*rub.width()/2+sizeImage.width()/2-rubCenter.x()+width()/2;
        if(rubMove.x()>=tmp) rubMove.setX(tmp);
        tmp = zoom*rub.height()/2-sizeImage.height()/2-rubCenter.y()+height()/2;
        if(rubMove.y()<tmp) rubMove.setY(tmp);
        tmp = -zoom*rub.height()/2+sizeImage.height()/2-rubCenter.y()+height()/2;
        if(rubMove.y()>=tmp) rubMove.setY(tmp);
    }
    else if(fMove==MOVE_L){
        newZoom = zoomBase*(moveStart.x()-moveCurrent.x()+zoomBase*rub.width())/(zoomBase*rub.width());
        if(newZoom>0){
            //&& (base.x()-newZoom*rub.width()>0)){
            zoom=newZoom;
            rubCenter.setX((base.x()-(zoom*rub.width())/2));
        }
    }
    else if(fMove==MOVE_R){
        newZoom = zoomBase*(moveCurrent.x()-moveStart.x()+zoomBase*rub.width())/(zoomBase*rub.width());
        if(newZoom>0){
            //&& (base.x()+newZoom*rub.width()>0)){
            zoom=newZoom;
            rubCenter.setX(base.x()+(zoom*rub.width())/2);
        }
    }
    else if(fMove==MOVE_T){
        newZoom = zoomBase*(moveStart.y()-moveCurrent.y()+zoomBase*rub.height())/(zoomBase*rub.height());
        if(newZoom>0){
            zoom=newZoom;
            rubCenter.setY(base.y()-(zoom*rub.height())/2);
        }
    }
    else if(fMove==MOVE_D){
        newZoom = zoomBase*(moveCurrent.y()-moveStart.y()+zoomBase*rub.height())/(zoomBase*rub.height());
        if(newZoom>0){
            zoom=newZoom;
            rubCenter.setY(base.y()+(zoom*rub.height())/2);
        }
    }

    rubTL=rubCenter-QPoint(zoom*rub.width()/2,zoom*rub.height()/2);
    rubDR=rubCenter+QPoint(zoom*rub.width()/2,zoom*rub.height()/2);

    if(!image.isNull()){
        //painter.setPen(QPen(QColor(0,0,0),1,Qt::SolidLine));
        //painter.drawRect(0,0,width()-1,height()-1);
        painter.setPen(QPen(QColor(0,0,0),1,Qt::DashLine));
        painter.drawRect(rubTL.x()+rubMove.x(),rubTL.y()+rubMove.y(),rub.width()*zoom-1,rub.height()*zoom-1);
        painter.setPen(QPen(QColor(255,255,255),1,Qt::DashLine));
        painter.drawRect(rubTL.x()+rubMove.x()+1,rubTL.y()+rubMove.y()+1,rub.width()*zoom-3,rub.height()*zoom-3);
    }
    // ƒанные дл€ сохранини€ на диск
    zoomAll=((double)image.height()/(double)sizeImage.height())*zoom;
    emit imgRub(QPoint(
            ((double)(rubTL.x()-width()/2+sizeImage.width()/2))*zoomAll/zoom,
            ((double)(rubTL.y()-height()/2+sizeImage.height()/2))*zoomAll/zoom),
            zoomAll);
}

void ImageEdit::setImage(QImage img)
{
    image=img;
    zoomChanged=true;
    update();
}

void ImageEdit::mousePressEvent(QMouseEvent *event)
{
    QPoint pos=event->pos();
    const int delta=5;
    if(event->button()==Qt::RightButton) emit rightButton();
    if(event->button()==Qt::MidButton) emit midButton();
    if(event->button()==Qt::LeftButton){
        if((pos.x()-rubTL.x())>-delta && (pos.x()-rubTL.x())<delta){
            fMove = MOVE_L;
            base=QPoint(rubDR.x(), rubCenter.y());
            this->setCursor(Qt::SizeHorCursor);
        }
        else if((pos.x()-rubDR.x())>-delta && (pos.x()-rubDR.x())<delta){
            fMove = MOVE_R;
            base=QPoint(rubTL.x(), rubCenter.y());
            this->setCursor(Qt::SizeHorCursor);
        }
        else if((pos.y()-rubTL.y())>-delta && (pos.y()-rubTL.y())<delta){
            fMove = MOVE_T;
            base=QPoint(rubCenter.x(),rubDR.y());
            this->setCursor(Qt::SizeVerCursor);
        }
        else if((pos.y()-rubDR.y())>-delta && (pos.y()-rubDR.y())<delta){
            fMove = MOVE_D;
            base=QPoint(rubCenter.x(),rubTL.y());
            this->setCursor(Qt::SizeVerCursor);
        }
        else if(pos.x()>rubTL.x() && pos.x()<rubDR.x() && pos.y()>rubTL.y() && pos.y()<rubDR.y()){
            fMove = MOVE_PAINT;
            this->setCursor(Qt::SizeAllCursor);
        }
        moveCurrent = pos;
        moveStart = pos;
        zoomBase = zoom;
        //if()
    }
    update();
}
void ImageEdit::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos=event->pos();
    if(pos!=moveCurrent) update();
    moveCurrent=pos;
}
void ImageEdit::mouseReleaseEvent(QMouseEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    rubCenter=rubCenter+rubMove; rubMove=QPoint(0,0);
    fMove=NO_MOVE;
    update();
}
void ImageEdit::wheelEvent(QWheelEvent *event)
{
    if(event->orientation()==Qt::Vertical)
    {
        if(event->delta()>0) emit upWheel();
        else if(event->delta()<0) emit downWheel();
    }
}
void ImageEdit::setHeight(int h)
{
    rub.setHeight(h);
    zoomChanged=true;
    update();
}
void ImageEdit::setWidth(int w)
{
    rub.setWidth(w);
    zoomChanged=true;
    update();
}
