#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QMainWindow>
#include "echonestsong.h"

namespace Ui {
    class ResultWindow;
}

class ResultWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = 0, const EchoNestSong *song = 0);
    ~ResultWindow();

private slots:
    void paintEvent(QPaintEvent*);

private:
    Ui::ResultWindow *ui;
    const EchoNestSong *_song;
};

#endif // RESULTWINDOW_H
