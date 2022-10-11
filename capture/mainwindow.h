#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actioncaptureFullScreen_triggered();   // 截取全屏

    void on_actioncaptureSelRect_triggered();   // 截取捕获矩形区域



protected:
    void mouseEnter(QEvent *event);
    void mouseLeave(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool isMousePressed;
    QPoint beginPoint, endPoint;  // 起点和终点

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
