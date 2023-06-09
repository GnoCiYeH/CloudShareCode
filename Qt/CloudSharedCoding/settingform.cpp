#include "settingform.h"
#include "ui_settingform.h"
#include"myhelper.h"
#include <QToolButton>
#include<QSettings>
#include"codethemeitem.h"
#include"mainwindow.h"

SettingForm::SettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingForm)
{
    ui->setupUi(this);
    ui->listWidget->setStyleSheet("QListWidget#listWidget{background-color:rgb(255,255,255);border:0px;}"
                                  "QListWidget#listWidget::item{height:40px;border:0px;}");

    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(itemChanged(int)));
    settings = new QSettings("./configs/configs.ini",QSettings::IniFormat,this);
    //environment
    QListWidgetItem* environment = new QListWidgetItem(ui->listWidget);
    environment->setText("环境配置");
    environment->setIcon(QIcon("://qss/black/calendar_prevmonth.png"));
    ui->listWidget->addItem(environment);

    QVariant var;
    var.setValue(QString(":/qdarkstyle/dark/darkstyle.qss"));
    ui->comboBox->addItem("dark",var);
    var.setValue(QString(":/qdarkstyle/light/lightstyle.qss"));
    ui->comboBox->addItem("light",var);
    var.setValue(QString("://vsDark/vsDark.css"));
    ui->comboBox->addItem("vsDark",var);
    var.setValue(QString("://DarkStyle/darkStyle.css"));
    ui->comboBox->addItem("dark2",var);
    var.setValue(QString("://materialStyle/materialStyle.css"));
    ui->comboBox->addItem("material",var);
    var.setValue(QString("://qss/one-dark.qss"));
    ui->comboBox->addItem("one-dark",var);
    var.setValue(QString("://qss/black.qss"));
    ui->comboBox->addItem("black",var);
    var.setValue(QString("://qss/carbon.qss"));
    ui->comboBox->addItem("carbon",var);
    var.setValue(QString("://qss/coffee.qss"));
    ui->comboBox->addItem("coffee",var);
    var.setValue(QString("://qss/default.qss"));
    ui->comboBox->addItem("default",var);
    var.setValue(QString("://qss/default_macos.qss"));
    ui->comboBox->addItem("default_macos",var);
    var.setValue(QString("://qss/detroit-future.qss"));
    ui->comboBox->addItem("detroit-future",var);
    var.setValue(QString("://qss/evilworks.qss"));
    ui->comboBox->addItem("evilworks",var);
    var.setValue(QString("://qss/gray.qss"));
    ui->comboBox->addItem("gray",var);
    var.setValue(QString("://qss/sublime.qss"));
    ui->comboBox->addItem("sublime",var);
    var.setValue(QString("://qss/vs-dark-hdpi.qss"));
    ui->comboBox->addItem("vs-dark-hdpi",var);
    var.setValue(QString("://qss/vs-dark.qss"));
    ui->comboBox->addItem("vs-dark",var);
    connect(ui->comboBox,SIGNAL(currentIndexChanged (int)),this,SLOT(themeChanged(int)));

    //text editer
    QListWidgetItem* editer = new QListWidgetItem(ui->listWidget);
    editer->setText("文本编辑器");
    editer->setIcon(QIcon("://qss/black/calendar_prevmonth.png"));
    ui->listWidget->addItem(editer);

    QFormLayout* layout = new QFormLayout(ui->page_2);
    layout->addWidget(new QLabel("代码风格",ui->page_2));
    QFrame* hframe = new QFrame(this);
    hframe->setFrameShape(QFrame::HLine);
    hframe->setStyleSheet("QFrame{background:black;min-height:2px}");
    layout->addWidget(hframe);

    settings->beginGroup("CODETHEME");
    auto keywordTheme = new CodeThemeItem("Keywords",QColor(settings->value("KEYWORD","#37bb2d").toString()),HightlightType::KEYWORD,ui->page_2);
    layout->addWidget(keywordTheme);
    auto classTheme = new CodeThemeItem("Class",QColor(settings->value("CLASS","#bcc069").toString()),HightlightType::CLASS,ui->page_2);
    layout->addWidget(classTheme);
    auto signleLineTheme = new CodeThemeItem("Signle line comment",QColor(settings->value("SIGNLE_LINE_COMMENT","#6e888f").toString()),HightlightType::SIGNLE_LINE_COMMENT,ui->page_2);
    layout->addWidget(signleLineTheme);
    auto muliilineTheme = new CodeThemeItem("Multiline comment",QColor(settings->value("MULITLINE_COMMENT","#82887f").toString()),HightlightType::MULITLINE_COMMENT,ui->page_2);
    layout->addWidget(muliilineTheme);
    auto quotationTheme = new CodeThemeItem("Quotation",QColor(settings->value("QUOTATION","#dbcf2c").toString()),HightlightType::QUOTATION,ui->page_2);
    layout->addWidget(quotationTheme);
    auto functionTheme = new CodeThemeItem("Function",QColor(settings->value("FUNCTION","#ce9191").toString()),HightlightType::FUNCTION,ui->page_2);
    layout->addWidget(functionTheme);
    auto headerTheme = new CodeThemeItem("Header",QColor(settings->value("HEADER","#a65579").toString()),HightlightType::HEADER,ui->page_2);
    layout->addWidget(headerTheme);
    auto stdioTheme = new CodeThemeItem("Stdio",QColor(settings->value("STDIO","#8962c1").toString()),HightlightType::STDIO,ui->page_2);
    layout->addWidget(stdioTheme);
    auto branchTheme = new CodeThemeItem("Branch",QColor(settings->value("BRANCH","#ff7637").toString()),HightlightType::BRANCH,ui->page_2);
    layout->addWidget(branchTheme);
    settings->endGroup();

    connect(keywordTheme,SIGNAL(colorChanged(HightlightType,QColor)),this,SLOT(codeThemeChanged(HightlightType,QColor)));
    connect(classTheme,SIGNAL(colorChanged(HightlightType,QColor)),this,SLOT(codeThemeChanged(HightlightType,QColor)));
    connect(signleLineTheme,SIGNAL(colorChanged(HightlightType,QColor)),this,SLOT(codeThemeChanged(HightlightType,QColor)));
    connect(muliilineTheme,SIGNAL(colorChanged(HightlightType,QColor)),this,SLOT(codeThemeChanged(HightlightType,QColor)));
    connect(quotationTheme,SIGNAL(colorChanged(HightlightType,QColor)),this,SLOT(codeThemeChanged(HightlightType,QColor)));
    connect(functionTheme,SIGNAL(colorChanged(HightlightType,QColor)),this,SLOT(codeThemeChanged(HightlightType,QColor)));
    connect(headerTheme,SIGNAL(colorChanged(HightlightType,QColor)),this,SLOT(codeThemeChanged(HightlightType,QColor)));
    connect(stdioTheme,SIGNAL(colorChanged(HightlightType,QColor)),this,SLOT(codeThemeChanged(HightlightType,QColor)));
    connect(branchTheme,SIGNAL(colorChanged(HightlightType,QColor)),this,SLOT(codeThemeChanged(HightlightType,QColor)));

    //local project
    QListWidgetItem* makeProject = new QListWidgetItem(ui->listWidget);
    makeProject->setText("本地项目与构建");
    makeProject->setIcon(QIcon("://qss/black/calendar_prevmonth.png"));
    ui->listWidget->addItem(makeProject);


    //init
    settings->beginGroup("ENVIRONMENT");
    QString theme = settings->value("THEME",":/qdarkstyle/light/lightstyle.qss").toString();
    myHelper::setStyle(theme);
    int num = ui->comboBox->count();
    for (int var = 0; var < num; ++var) {
        if(ui->comboBox->itemData(var,Qt::UserRole).toString()==theme)
        {
            ui->comboBox->setCurrentIndex(var);
            break;
        }
    }
    settings->endGroup();
}

SettingForm::~SettingForm()
{
    delete ui;
}

void SettingForm::itemChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void SettingForm::themeChanged(int)
{
    settingQueue.enqueue(QPair<SettingType,QVariant>(SettingType::THEME,NULL));
}

void SettingForm::codeThemeChanged(HightlightType type,QColor color)
{
    QPair<HightlightType,QColor> pair(type,color);
    QVariant var;
    var.setValue(pair);
    settingQueue.enqueue(QPair<SettingType,QVariant>(SettingType::CODETHEME,var));
}

void SettingForm::on_pushButton_clicked()
{
    this->close();
}

void SettingForm::on_pushButton_2_clicked()
{
    int count = settingQueue.length();
    while(count>0)
    {
        auto it = settingQueue.dequeue();
        switch (it.first) {
        case SettingType::THEME:
        {
            QVariant var = ui->comboBox->currentData(Qt::UserRole);
            QString themePath = var.toString();
            myHelper::setStyle(themePath);
            settings->beginGroup("ENVIRONMENT");
            settings->setValue("THEME",themePath);
            settings->endGroup();
            break;
        }
        case SettingType::CODETHEME:
        {
            QPair<HightlightType,QColor> pair = it.second.value<QPair<HightlightType,QColor>>();
            switch (pair.first) {
            case HightlightType::KEYWORD:
            {
                settings->beginGroup("CODETHEME");
                settings->setValue("KEYWORD",pair.second.name());
                settings->endGroup();
                break;
            }
            case HightlightType::CLASS:
            {
                settings->beginGroup("CODETHEME");
                settings->setValue("CLASS",pair.second.name());
                settings->endGroup();
                break;
            }
            case HightlightType::BRANCH:
            {
                settings->beginGroup("CODETHEME");
                settings->setValue("BRANCH",pair.second.name());
                settings->endGroup();
                break;
            }
            case HightlightType::SIGNLE_LINE_COMMENT:
            {
                settings->beginGroup("CODETHEME");
                settings->setValue("SIGNLE_LINE_COMMENT",pair.second.name());
                settings->endGroup();
                break;
            }
            case HightlightType::MULITLINE_COMMENT:
            {
                settings->beginGroup("CODETHEME");
                settings->setValue("MULITLINE_COMMENT",pair.second.name());
                settings->endGroup();
                break;
            }
            case HightlightType::QUOTATION:
            {
                settings->beginGroup("CODETHEME");
                settings->setValue("QUOTATION",pair.second.name());
                settings->endGroup();
                break;
            }
            case HightlightType::FUNCTION:
            {
                settings->beginGroup("CODETHEME");
                settings->setValue("FUNCTION",pair.second.name());
                settings->endGroup();
                break;
            }
            case HightlightType::HEADER:
            {
                settings->beginGroup("CODETHEME");
                settings->setValue("HEADER",pair.second.name());
                settings->endGroup();
                break;
            }
            case HightlightType::STDIO:
            {
                settings->beginGroup("CODETHEME");
                settings->setValue("STDIO",pair.second.name());
                settings->endGroup();
                break;
            }
            default:
                break;
            }
            break;
        }
        case SettingType::PROJECT_INCLUDE:
        {
            QString path = ui->includePath_edit->text();
            QStringList list = path.split(";");
            settings->beginGroup("CXX_INCLUDE");
            settings->beginWriteArray("INCLUDES");
            for(int i = 0;i< list.length();i++)
            {
                settings->setArrayIndex(i);
                settings->setValue("include_path",list[i]);
            }
            settings->endArray();
            settings->endGroup();
            break;
        }
        default:
            break;
        }
        count--;
    }
    this->close();
}


void SettingForm::on_includePath_edit_textEdited(const QString &arg1)
{
    settingQueue.enqueue(QPair<SettingType,QVariant>(SettingType::PROJECT_INCLUDE,QVariant()));
}

