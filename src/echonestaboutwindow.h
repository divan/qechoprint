#ifndef ECHONESTABOUTWINDOW_H
#define ECHONESTABOUTWINDOW_H

#include <QMainWindow>

namespace Ui {
    class EchoNestAboutWindow;
}

class EchoNestAboutWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EchoNestAboutWindow(QWidget *parent = 0);
    ~EchoNestAboutWindow();

private:
    Ui::EchoNestAboutWindow *ui;

private slots:
    void paintEvent(QPaintEvent*);
    void orientationChanged();

    void on_urlButton_clicked();
};

#endif // ECHONESTABOUTWINDOW_H
