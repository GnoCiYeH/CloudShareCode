#include "codethemeitem.h"
#include "ui_codethemeitem.h"

#include <QColorDialog>

CodeThemeItem::CodeThemeItem(QString title,QColor color,HightlightType type,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CodeThemeItem)
{
    ui->setupUi(this);

    ui->label->setText(title);
    ui->label_color->setText(color.name());
    ui->widget->setStyleSheet("background-color:"+color.name()+";");
    this->color = color;
    this->type = type;
}

CodeThemeItem::~CodeThemeItem()
{
    delete ui;
}

void CodeThemeItem::setColor(QColor color)
{
    ui->label_color->setText(color.name());
    ui->widget->setStyleSheet("background-color:"+color.name()+";");
}

void CodeThemeItem::on_pushButton_clicked()
{
    QColor selectColor = QColorDialog::getColor(Qt::white,this);
    if(selectColor!=color)
    {
        ui->widget->setStyleSheet("background-color:"+selectColor.name()+";");
        ui->label_color->setText(selectColor.name());
        color = selectColor;
        emit colorChanged(this->type,selectColor);
    }
}

