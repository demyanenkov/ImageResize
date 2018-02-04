#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    QDir dir;
    ui->setupUi(this);
    ui->pathIn->addItem(QDir::currentPath());
    ui->pathOut->addItem(QDir::currentPath());
    ui->pathOut->addItem(QDir::currentPath()+"/out");
    ui->height->setValue(360);
    ui->width->setValue(640);
    index = -1;
    this->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    ui->par->setVisible(ui->prop->isChecked());
    ui->view->setVisible(ui->preview->isChecked());
    ui->run->setVisible(!ui->preview->isChecked());
    ui->prev->setVisible(ui->preview->isChecked());
    ui->next->setVisible(ui->preview->isChecked());
    ui->open->setVisible(ui->preview->isChecked());
    ui->save->setVisible(ui->preview->isChecked());
    ui->first->setVisible(ui->preview->isChecked());
    ui->last->setVisible(ui->preview->isChecked());
    ui->size->setVisible(!ui->preview->isChecked());
    ui->autoSave->setVisible(ui->preview->isChecked());

    //this->resize(this->size().width(),this->minimumHeight());
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_battonStart_clicked()
{
    QImage img;
    QSize size(ui->height->value(),ui->width->value());
    QSize autoSize(size);
    QString path = ui->pathIn->currentText();
    QStringList filters;
    filters << "*.jpg"<<"*.jpeg";
    QDir dir;
    QStringList dirList;
    QString name,nameBase;
     ui->progressBar->setValue(0);
    dir.setPath(ui->pathIn->currentText());
    QString current=dir.currentPath();
    if(dir.exists(ui->pathIn->currentText())) dir.setCurrent(ui->pathIn->currentText());
    else dir.setCurrent(current);
    dirList = dir.entryList(filters);
    nameBase=ui->pattern->currentText();
    nameBase.remove("#");
    for(int i=0;i<dirList.size();i++){
        if(dir.exists(ui->pathIn->currentText())) dir.setCurrent(ui->pathIn->currentText());
        else dir.setCurrent(current);
        if(img.load(dirList.at(i))){
            if(ui->autoRotate->isChecked()){
                if(img.width()>img.height()){
                    if(size.width()>size.height())
                        autoSize=size;
                    else{
                        autoSize.setHeight(size.width());
                        autoSize.setWidth(size.height());
                    }
                }
                else{
                    if(size.width()>size.height()){
                        autoSize.setHeight(size.width());
                        autoSize.setWidth(size.height());
                    }
                    else autoSize=size;
                }
            }
            ui->height->setValue(autoSize.height());
            ui->width->setValue(autoSize.width());
            img=img.scaled(autoSize,
                ui->size->isChecked()? Qt::KeepAspectRatioByExpanding:
                Qt::KeepAspectRatio,
                //Qt::IgnoreAspectRatio,
                ui->smooth->isChecked()?
                Qt::SmoothTransformation:Qt::FastTransformation);
            if(dir.exists(ui->pathOut->currentText())) dir.setCurrent(ui->pathOut->currentText());
            else(dir.setCurrent(current));
            switch(ui->pattern->currentText().count('#')){
            case 2: name.sprintf("%02i.jpg",i+ui->index->value()); break;
            case 3: name.sprintf("%03i.jpg",i+ui->index->value()); break;
            case 4: name.sprintf("%04i.jpg",i+ui->index->value()); break;
            case 5: name.sprintf("%05i.jpg",i+ui->index->value()); break;
            default: name.sprintf("%i.jpg",i+ui->index->value());
            }
            name="/"+nameBase+name;
            ui->name->setText(nameBase+name);
            img.save(ui->pathOut->currentText()+name,"JPG",ui->compression->value());
            ui->progressBar->setValue((100.0*(i+1))/dirList.size());
        }
    }
    ui->battonStart->setChecked(false);
}

void Dialog::on_rotateSize_clicked()
{
    int tmp;
    tmp=ui->width->value();
    ui->width->setValue(ui->height->value());
    ui->height->setValue(tmp);
}

void Dialog::on_setIn_clicked()
{
    //QString fileName = QFileDialog::getOpenFileName(this);
    //ui->pathIn->addItem(fileName);
    QString dirName = QFileDialog::getExistingDirectory(this);
    if(QDir(dirName).exists() && dirName!=""){
        ui->pathIn->addItem(dirName);
        ui->pathIn->setCurrentIndex(ui->pathIn->count()-1);
    }
    ui->name->setText("");
}

void Dialog::on_setOut_clicked()
{
    //QString fileName = QFileDialog::getOpenFileName(this);
    QString dirName = QFileDialog::getExistingDirectory(this);
    if(QDir(dirName).exists() && dirName!=""){
        ui->pathOut->addItem(dirName);
        ui->pathOut->setCurrentIndex(ui->pathOut->count()-1);
    }
    ui->name->setText("");
}
void Dialog::open(QString fileName)
{
    QSize size(ui->height->value(),ui->width->value());
    QSize autoSize(size);

    if(QFile(fileName).exists()){
        image.load(fileName);
        if(ui->autoRotate->isChecked()){
            if(image.width()>image.height()){
                if(size.width()>size.height())
                    autoSize=size;
                else{
                    autoSize.setHeight(size.width());
                    autoSize.setWidth(size.height());
                }
            }
            else{
                if(size.width()>size.height()){
                    autoSize.setHeight(size.width());
                    autoSize.setWidth(size.height());
                }
                else autoSize=size;
            }
            ui->height->setValue(autoSize.height());
            ui->width->setValue(autoSize.width());
        }
        emit imageLoad(image);
        //dir.setCurrent(QFile(fileName).symLinkTarget());
    }
}
void Dialog::on_open_clicked()
{
    QDir dir;
    QString dirName;
    QStringList dirList;
    QStringList filters;
    QFileInfo file;
    filters << "*.jpg"<<"*.jpeg";

    if(dir.exists(ui->pathIn->currentText())) dir.setCurrent(ui->pathIn->currentText());

    QString fileName = QFileDialog::getOpenFileName(this);
    if(!QFileInfo(fileName).exists()) return;

    dirName=QFileInfo(fileName).absoluteDir().path();
    if(dirName!=ui->pathIn->currentText()){
        ui->pathIn->addItem(dirName);
        ui->pathIn->setCurrentIndex(ui->pathIn->count()-1);
    }

    if(dir.exists(ui->pathIn->currentText())) dir.setCurrent(ui->pathIn->currentText());

    dirList = dir.entryList(filters);

    index=0;
    for(int i=0;i<dirList.size();i++)
        if(QFileInfo(dirList.at(i)).absoluteFilePath() == QFileInfo(fileName).absoluteFilePath()) index=i;
    ui->name->setText(fileName+QString().sprintf(" <font color=blue>[%i/%i]</font> %6.1fk",index+1,dirList.size(), (double)QFileInfo(dirList.at(index)).size()/(double)1000));
    open(fileName);
}

void Dialog::on_save_clicked()
{
    QDir dir;
    QString name,nameBase;
    QString fileName;
    QString dirName;

    ui->save->setChecked(true);
    if(dir.exists(ui->pathOut->currentText())) dir.setCurrent(ui->pathOut->currentText());

    QImage img=image.copy(rubTL.x(),rubTL.y(),ui->width->value()*zoom,ui->height->value()*zoom);
    img=img.scaled(QSize(ui->width->value(),ui->height->value()),
                              //ui->size->isChecked()?
//                   Qt::KeepAspectRatioByExpanding,
                              //Qt::KeepAspectRatio,
                              Qt::IgnoreAspectRatio,
                              ui->smooth->isChecked()?
                              Qt::SmoothTransformation:Qt::FastTransformation);

    nameBase=ui->pattern->currentText();
    nameBase.remove("#");

    int i= 0;
    do{
        switch(ui->pattern->currentText().count('#')){
        case 2: name.sprintf("%02i.jpg",i+ui->index->value()); break;
        case 3: name.sprintf("%03i.jpg",i+ui->index->value()); break;
        case 4: name.sprintf("%04i.jpg",i+ui->index->value()); break;
        case 5: name.sprintf("%05i.jpg",i+ui->index->value()); break;
        default: name.sprintf("%i.jpg",i+ui->index->value());
        }
        name=nameBase+name;
        i++;
    } while(i<9999 && QFile::exists(name));
    if(dir.exists(ui->pathOut->currentText())) dir.setCurrent(ui->pathOut->currentText());
    if(ui->autoSave->isChecked())
        fileName =ui->pathOut->currentText()+"/"+name;
    else
        fileName = QFileDialog::getSaveFileName(this,"Save",name,"*.jpg");

    img.save(fileName,"JPG",ui->compression->value());
    ui->name->setText(fileName+QString().sprintf(" %6.1fk", (double)QFileInfo(fileName).size()/(double)1000));
    dirName=QFileInfo(fileName).absoluteDir().path();
    if(dirName!=ui->pathOut->currentText() && QFileInfo(fileName).exists()){
        ui->pathOut->addItem(dirName);
        ui->pathOut->setCurrentIndex(ui->pathOut->count()-1);
    }
    ui->save->setChecked(false);
}

void Dialog::on_preview_clicked(bool checked)
{
    ui->view->setVisible(checked);
    ui->run->setVisible(!checked);

    ui->prev->setVisible(checked);
    ui->next->setVisible(checked);
    ui->open->setVisible(checked);
    ui->save->setVisible(checked);
    ui->first->setVisible(checked);
    ui->last->setVisible(checked);
    ui->size->setVisible(!checked);
    ui->autoSave->setVisible(checked);
    //this->resize(this->size().width(),this->minimumHeight());
}

void Dialog::on_prop_clicked(bool checked)
{
    ui->par->setVisible(checked);
    //if(!ui->preview->isChecked())
    //    this->resize(this->size().width(),this->minimumHeight());
}
void Dialog::imgRub(QPoint TL,double zm)
{
    rubTL=TL;
    zoom=zm;
}

void Dialog::gotoFile(int direction)
{
    QDir dir;
    QStringList dirList;
    QStringList filters;
    filters << "*.jpg"<<"*.jpeg";

    if(dir.exists(ui->pathIn->currentText())) dir.setCurrent(ui->pathIn->currentText());

    dirList = dir.entryList(filters);
    if(dirList.size()<1){
        emit imageLoad(QImage());
        return;
    }
    switch(direction){
    case GO_FIRST: index = 0; break;
    case GO_LAST: index=dirList.size()-1; break;
    case GO_NEXT:
        if(index<(dirList.size()-1)) index++; else index = dirList.size()-1;
        if(index<0 || index>=dirList.size()) index = 0;break;
    case GO_PREV:
        if(index>0) index--; else index=0;
        if(index<0 || index>=dirList.size()) index = dirList.size()-1;break;
    }
    open(dirList.at(index));
    ui->name->setText(dirList.at(index)+QString().sprintf(" <font color=blue>[%i/%i]</font> %6.1fk",index+1,dirList.size(), (double)QFileInfo(dirList.at(index)).size()/(double)1000));
}

void Dialog::on_prev_clicked()
{
    gotoFile(GO_PREV);
}

void Dialog::on_next_clicked()
{
    gotoFile(GO_NEXT);
}

void Dialog::on_first_clicked()
{
    gotoFile(GO_FIRST);
}

void Dialog::on_last_clicked()
{
    gotoFile(GO_LAST);
}
