#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTextBrowser>
#include <QLabel>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTextEdit *m_text = nullptr;
    QTextBrowser *m_text_r = nullptr;
    QLabel *m_text_label = nullptr;
    QTimer *m_timer = nullptr;
    bool is_half_time = true;
public slots:
    QString get_text();
    QString get_utc();
    void onTimeout();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
