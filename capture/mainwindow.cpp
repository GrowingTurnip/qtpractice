#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "capture.h"


#include <QMessageBox>
#include <QDesktopWidget>
#include <QApplication>
#include <QFileDialog>
#include <QMouseEvent>
#include <QScreen>
#include <QString>
#include <QDebug>
#include <QPoint>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口
    setFixedSize(451, 0);
    setWindowTitle(QString("MyFastStone_Capture"));
    setWindowIcon(QIcon(":/icons/capture.png"));

    beginPoint = this->pos();
    isMousePressed = false;
    this->setProperty("CanMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint |
                         Qt::WindowStaysOnTopHint);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint|
                         Qt::WindowCloseButtonHint);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::mouseEnter(QEvent *event)
{
    if (this->pos().y() <= 0)  // 鼠标进入并贴边
        move(pos().x(), 0);   // 弹出整个窗体

}

void MainWindow::mouseLeave(QEvent *event)
{
    if (this->pos().y() <= 0)   // 窗体在屏幕上边
        move(pos().x(), -1);  // 贴边

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isMousePressed = true;
         beginPoint = event->pos();
    }
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isMousePressed) {
        endPoint = event->pos();
        update();
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    endPoint = event->pos();
    isMousePressed = false;

}

// function1

void MainWindow::on_actioncaptureFullScreen_triggered()
{
    // 截取全屏
    QPixmap p = QPixmap::grabWindow(QApplication::desktop()->winId());
    QString fileName = QFileDialog::getSaveFileName(this, "文件另存为", "", tr("Config Files(*.bmp *.jpg)"));
    if (fileName.length() > 0 && p.save(fileName, "bmp"))
        QMessageBox::information(this, "提示", "保存成功！", QMessageBox::Ok);

}

// function2, 截取任意大小的矩形区域

void MainWindow::on_actioncaptureSelRect_triggered()
{
    // 截取捕获的矩形区域
    // 截图之前隐去窗体
    if (windowState() != Qt::WindowMinimized)
        setWindowState(Qt::WindowMinimized);
    // 延时等待父窗体最小化
    QTime _timer = QTime::currentTime().addMSecs(250);
    while (QTime::currentTime() < _timer)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    // 先获取全屏截图， 再拉上幕布
    Capture *im = new Capture();
    im->fullScreen = QPixmap::grabWindow(QApplication::desktop()->winId());
    im->showFullScreen();

}

// function3 截取任意大小的椭圆


