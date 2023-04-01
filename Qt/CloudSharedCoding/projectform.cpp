#include "projectform.h"
#include "ui_projectform.h"

ProjectForm::ProjectForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectForm)
{
    ui->setupUi(this);

    QListWidgetItem* item = new QListWidgetItem("xiangmu");
    ui->listWidget->addItem(item);
}

ProjectForm::~ProjectForm()
{
    delete ui;
}
