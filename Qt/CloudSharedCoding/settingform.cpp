#include "settingform.h"
#include "ui_settingform.h"

#include <QToolButton>

SettingForm::SettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingForm)
{
    ui->setupUi(this);
    ui->listWidget->setStyleSheet("QListWidget#listWidget{background-color:rgb(255,255,255);border:0px;}"
                                  "QListWidget#listWidget::item{height:40px;border:0px;}");

    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(itemChanged(int)));

    //environment
    QListWidgetItem* environment = new QListWidgetItem(ui->listWidget);
    environment->setText("环境配置");
    environment->setIcon(QIcon("://qss/black/calendar_prevmonth.png"));
    ui->listWidget->addItem(environment);

    QVariant var;
    var.setValue("://qss/black.css");
    ui->comboBox->addItem("black",var);
    var.setValue("://qss/blue.css");
    ui->comboBox->addItem("blue",var);
    var.setValue("://qss/darkblue.css");
    ui->comboBox->addItem("darkblue",var);
    var.setValue("://qss/darkgray.css");
    ui->comboBox->addItem("darkgray",var);
    var.setValue("://qss/flatwhite.css");
    ui->comboBox->addItem("flatwhite",var);
    var.setValue("://qss/gray.css");
    ui->comboBox->addItem("gray",var);
    var.setValue("://qss/lightblack.css");
    ui->comboBox->addItem("lightblack",var);
    var.setValue("://qss/lightblue.css");
    ui->comboBox->addItem("lightblue",var);
    var.setValue("://qss/lightgray.css");
    ui->comboBox->addItem("lightgray",var);
    var.setValue("://qss/blue.css");
    ui->comboBox->addItem("psblack",var);
    var.setValue("://qss/psblack.css");
    ui->comboBox->addItem("silvery",var);
    var.setValue("://qss/silvery.css");
    ui->comboBox->addItem("blue",var);

    //text editer
    QListWidgetItem* editer = new QListWidgetItem(ui->listWidget);
    editer->setText("文本编辑器");
    editer->setIcon(QIcon("://qss/black/calendar_prevmonth.png"));
    ui->listWidget->addItem(editer);

    //local project
    QListWidgetItem* makeProject = new QListWidgetItem(ui->listWidget);
    makeProject->setText("本地项目与构建");
    makeProject->setIcon(QIcon("://qss/black/calendar_prevmonth.png"));
    ui->listWidget->addItem(makeProject);
}

SettingForm::~SettingForm()
{
    delete ui;
}

void SettingForm::itemChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}
