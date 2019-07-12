#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QTextBrowser>
#include <QTime>
#include <QTimer>
#include <QTextCodec>
#include <QEventLoop>
#include <QMessageBox>
#include <QRegExp>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTextCodec*codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    QString capation = QTextCodec::codecForName("gb2312")->toUnicode("Tool");
    qDebug()<<capation<<endl;
    setWindowTitle(capation);
    QTimer *timer = new QTimer(this);
    m_timer = timer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    m_timer->start(500);
    QMainWindow::resize(800, 600);
    QWidget *centerWindow = new QWidget;
    this->setCentralWidget(centerWindow);

    QLabel *text_label = new QLabel(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    m_text_label = text_label;
    text_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    text_label->setMinimumSize(10,30);
    text_label->setFixedSize(120, 30);

    QPushButton *utc2localtime = new QPushButton("时间戳转北京时间");
    utc2localtime->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    utc2localtime->setMinimumSize(10,30);
    utc2localtime->setFixedSize(120, 30);

    QPushButton *time2utc = new QPushButton("北京时间转时间戳");
    time2utc->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    time2utc->setMinimumSize(30,30);
    time2utc->setFixedSize(120, 30);

    QTextBrowser *text_r = new QTextBrowser();
    m_text_r = text_r;
    text_r->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    text_r->setMinimumSize(50,30);

    QTextEdit *text_w = new QTextEdit();
    m_text = text_w;
    text_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    text_w->setMinimumSize(160,30);

    QHBoxLayout *H_layout = new QHBoxLayout(this);
    H_layout->setSpacing(10);
    QHBoxLayout *H_layout_text_w = new QHBoxLayout(this);
    H_layout_text_w->setSpacing(10);
    QHBoxLayout *H_layout_text_r = new QHBoxLayout(this);
    H_layout_text_r->setSpacing(10);

    QVBoxLayout *V_layout = new QVBoxLayout(this);

    H_layout->addWidget(utc2localtime);
    H_layout->addWidget(time2utc);
    H_layout->addWidget(text_label);
    H_layout_text_w->addWidget(text_w);
    H_layout_text_r->addWidget(text_r);

    V_layout->addLayout(H_layout);
    V_layout->addLayout(H_layout_text_w);
    V_layout->addLayout(H_layout_text_r);
    centerWindow->setLayout(V_layout);
    connect(utc2localtime, SIGNAL(clicked()), this, SLOT(get_text()));
    connect(time2utc, SIGNAL(clicked()), this, SLOT(get_utc()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
QString MainWindow::get_text()
{
    QString text = m_text->toPlainText();
    if (!text.isEmpty()){
        QDateTime timestamp = QDateTime::fromTime_t(text.toInt());
        QString time_str = timestamp.toString("yyyy/MM/dd hh:mm:ss");
        qDebug()<<time_str<<endl;
        m_text_r->clear();
        m_text_r->append(time_str);
    }else{
        QMessageBox::information(NULL, "提示", "内容不能为空", QMessageBox::Ok, QMessageBox::Ok);
    }
    return text;
}

QString MainWindow::get_utc()
{
    QString text = m_text->toPlainText();
    QString utc;
    if (!text.isEmpty()){
        //        yyyy/MM/dd-hh:mm:ss
        //        2015/04/14
        QRegExp rx1("^((?!0000)[0-9]{4}/((0[1-9]|1[0-2])/(0[1-9]|1[0-9]|2[0-8])|(0[13-9]|1[0-2])/(29|30)|(0[13578]|1[02])/31)|([0-9]{2}(0[48]|[2468][048]|[13579][26])|(0[48]|[2468][048]|[13579][26])00)/02/29)(\\s|-)+([0-1]?[0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])$");
        QRegExp rx2("^((?!0000)[0-9]{4}-((0[1-9]|1[0-2])-(0[1-9]|1[0-9]|2[0-8])|(0[13-9]|1[0-2])-(29|30)|(0[13578]|1[02])-31)|([0-9]{2}(0[48]|[2468][048]|[13579][26])|(0[48]|[2468][048]|[13579][26])00)-02-29)(\\s|-)+([0-1]?[0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])$");
        int res1 = rx1.indexIn(text);
        int res2 = rx2.indexIn(text);
        qDebug()<<text<<"yyyy/mm/dd "<<res1<<"yyyy-mm-dd "<<res2<<endl;
        if (res1 == -1 && res2 == -1){
            QMessageBox::information(NULL, "error", "yyyy/mm/dd OR yyyy-mm-dd", QMessageBox::Ok, QMessageBox::Ok);
            return text;
        }
        if (res1 == 0){
            QDateTime utctime = QDateTime::fromString(text, "yyyy/MM/dd hh:mm:ss");
            utc = QString("%1").arg(utctime.toUTC().toTime_t());
        }
        if (res2 == 0){
            QDateTime utctime = QDateTime::fromString(text, "yyyy-MM-dd hh:mm:ss");
            utc = QString("%1").arg(utctime.toUTC().toTime_t());
        }
        m_text_r->clear();
        m_text_r->append(utc);
    }else {
        QMessageBox::information(NULL, "提示", "内容不能为空", QMessageBox::Ok, QMessageBox::Ok);
    }
    return text;
}

void MainWindow::onTimeout()
{
    QString time_str;
    if (is_half_time){
        is_half_time = false;
        time_str = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }else{
        is_half_time = true;
        time_str = QDateTime::currentDateTime().toString("yyyy-MM-dd:hh:mm:ss");
    }
    if (m_text_label){
        m_text_label->setText(time_str);
    }
}
