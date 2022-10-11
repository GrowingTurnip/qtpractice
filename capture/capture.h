#ifndef CAPTURE_H
#define CAPTURE_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QFileDialog>
#include <QShowEvent>
#include <QClipboard>
#include <QKeyEvent>
#include <QRect>
#include <QFile>

// 截屏的状态
enum captureState {
    initCapture,
    beginSlideCapture,
    endSlideCapture,
    beginMoveImage,
    endMoveImage,
    beginAdjustImage,
    endAdjustImage
};

// 调整截取矩形区域的状态
enum adjustCaptureState {
    initAdjustCapture,
    topLeftPoint,
    topRightPoint,
    bottomLeftPoint,
    bottomRightPoint,
    topCenterPoint,
    bottomCenterPoint,
    leftCenterPoint,
    rightCenterPoint
};

class Capture : public QWidget
{
    Q_OBJECT
public:
    explicit Capture(QWidget *parent = 0);

public slots:
    void saveCaptureOther();  // 另存截图
    void grapFullCature();  // 截取全屏
    void copyCapture();  // 复制到粘贴板

protected:
    void contextMenuEvent(QContextMenuEvent *event);  // 右键菜单事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *event);

public:
    QPixmap fullScreen;

private:

    int fullScreenWidth;
    int fullScreenHeight;
    captureState state;  // 截图状态
    adjustCaptureState adjustState;  // 调整截取矩形区域的状态

    QPoint beginPoint, endPoint;  // 最初截屏的起点终点
    QPoint beginMovePoint, endMovePoint;  // 拖动所截取图片的起点终点
    QPoint beginAdjustPoint, endAdjustPoint;  // 调整所截取的图片的起点终点
    QMenu *menu;
    QRect *rect;  // 矩形区域截图
    QRect curRect;
    QPainter painter;

    bool ismousePressed; // 鼠标是否按下
    bool isPointInRect(QPoint movedPoint);  // 拖动所截取图片时按下的点是否在截取的矩形内部
    bool isAdjustRect;  // 调整矩形区域是否开始
    QRect getRect(QPoint beginPoint, QPoint endPoint);  // 返回最初截取的矩形区域
    QRect getMoveRect();  // 返回拖动截取图片后的矩形区域
    QRect getAdjustRect();  // 返回调整过后的矩形区域
    QRect getSelectRect();  // 返回最终选取的矩形区域
    void drawCapture();  // 绘制选取的矩形区域
    void drawAdjustCapture();  // 绘制各调整处的矩形

    QPoint getBeginPoint();
    QPoint getEndPoint();
    QPoint getMvePoint();
    QPoint getAdjustPoint();
    QPoint getOffset(QPoint beginPos, QPoint endPos);  // 获取起始点的偏移
    adjustCaptureState getAdjustStateOfPoint(QPoint curPoint);  // 获取调整时鼠标的位置状态
    void setAdjustCursor(adjustCaptureState adjustState);  // 设置调整时鼠标的形状
    void setBeginPoint(QPoint p);
    void setEndPoiint(QPoint p);

    // 以下8个QRect为调整处的小矩形
    QRect topLeftRect;
    QRect topRightRect;
    QRect bottomLeftRect;
    QRect bottomRightRect;
    QRect topCenterRect;
    QRect bottomCenterRect;
    QRect leftCenterRect;
    QRect rightCenterRect;
};

#endif // CAPTURE_H
