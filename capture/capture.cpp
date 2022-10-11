#include "capture.h"

Capture::Capture(QWidget *parent) :
    QWidget(parent)
{
    // 初始化操作
    beginPoint = QPoint(-1, -1);
    endPoint = beginPoint;
    ismousePressed = false;
    isAdjustRect = false;
    state = initCapture;
    adjustState = initAdjustCapture;
    setMouseTracking(true);
    int wid = QApplication::desktop()->width();
    int hgt = QApplication::desktop()->height();
    rect = new QRect(0, 0, wid, hgt);

    // 创建右键菜单
    menu = new QMenu(this);
    menu->addAction("复制(CTRL + C)", this, SLOT(copyCapture()));
    menu->addAction("截图另存为(ALT + C)", this, SLOT(saveCaptureOther()));
    menu->addAction("全屏截图(ALT + A)", this, SLOT(grapFullCature()));
    menu->addAction("退出截图(ESC)", this, SLOT(hide()));

}

void Capture::saveCaptureOther()
{
    // 截图另存为
    QString fileName = QFileDialog::getSaveFileName(this, "截图另存为", "test.bmp", "Image(*.jpg *.png *.bmp)");
    if (fileName.length() > 0) {
        fullScreen.copy(curRect).save(fileName, "bmp");
        this->close();
    }
}

void Capture::grapFullCature()
{
    endPoint.setX(-1);  // 避免矩形截图
    QString fileName = QFileDialog::getSaveFileName(this, "保存全屏截图", "test.bmp", "Image (*.jpg *.png *.bmp)");
    if (fileName.length() > 0) {
        fullScreen.save(fileName, "bmp");
        this->close();
    }
    this->hide();

}

void Capture::copyCapture()
{
     QGuiApplication::clipboard()->setPixmap(fullScreen.copy(curRect));
}

void Capture::contextMenuEvent(QContextMenuEvent *event)
{
    // 右击菜单
    this->setCursor(Qt::ArrowCursor);
    menu->exec(cursor().pos());   // 菜单显示的位置跟随鼠标

}

void Capture::mousePressEvent(QMouseEvent *event)
{
    adjustState = getAdjustStateOfPoint(event->pos());
    if (event->button() == Qt::LeftButton) {
        if (state == initCapture) {
            ismousePressed = true;
            state = beginSlideCapture;  // 开始截图
            beginPoint = event->pos();
        }
        else if (isPointInRect(event->pos()) && adjustState == initAdjustCapture) {  // 开始拖动所截取的图片
            state = beginMoveImage;
            beginMovePoint = event->pos();
        }
        else if (adjustState != initAdjustCapture) {  // 开始调整截取的矩形区域
            isAdjustRect = true;
            state = beginAdjustImage;
            setAdjustCursor(adjustState);
            beginMovePoint = event->pos();
            beginAdjustPoint = event->pos();
        }
    }
}


void Capture::mouseMoveEvent(QMouseEvent *event)
{
    // 根据截图状态设置当前状态的终点
    if (state == beginSlideCapture) {
        endPoint = event->pos();
        update();  // 重绘，触发画图事件
    }
    if (state == beginMoveImage) {
        endMovePoint = event->pos();
        update();
    }
    if (state == beginAdjustImage) {
        beginMovePoint = event->pos();
        update();
    }

}

void Capture::mouseReleaseEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {
        // 鼠标左键释放
        if (state == beginSlideCapture) {
            state = endSlideCapture;
            endPoint = event->pos();
            update();
        }
        if (state == beginMoveImage) {
            state = endMoveImage;
            endMovePoint = event->pos();
            update();
        }
        if (state == beginAdjustImage && isAdjustRect) {
            state = endAdjustImage;
            isAdjustRect = false;
            endMovePoint = event->pos();
            update();
        }
    }

    // 使得起始点在左上角，结束点在右下角
    if (beginPoint.x() > endPoint.x()) {
        beginPoint.setX(beginPoint.x() + endPoint.x());
        endPoint.setX(beginPoint.x() - endPoint.x());
        beginPoint.setX(beginPoint.x() - endPoint.x());
    }
    if (beginPoint.y() > endPoint.y()) {
        beginPoint.setY(beginPoint.y() + endPoint.y());
        endPoint.setY(beginPoint.y() - endPoint.y());
        beginPoint.setY(beginPoint.y() - endPoint.y());
    }
    rect->setRect(beginPoint.x(), beginPoint.y(), endPoint.x(), endPoint.y());


}

void Capture::paintEvent(QPaintEvent *event)
{
    painter.begin(this);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(1);
    painter.setPen(pen);

    switch (state) {
    case initCapture:
        break;
    case beginSlideCapture:
    case endSlideCapture:
    case beginMoveImage:
    case endMoveImage:
    case beginAdjustImage:
    case endAdjustImage:{  // 这里一定要加{} 避免其他case情况下访问定义的变量
        curRect = getSelectRect();
        QPixmap captureImage = fullScreen.copy(curRect);
        painter.drawPixmap(curRect.topLeft(), captureImage);
        painter.drawRect(curRect);
        drawAdjustCapture();
        break;
    }
    default:
        break;
    }

    painter.end();

}

void Capture::showEvent(QShowEvent *event)
{
    // 设置透明度实现背景模糊
    setWindowOpacity(0.7);

}

void Capture::keyPressEvent(QKeyEvent *event)
{
    // esc退出截图
    if (event->key() == Qt::Key_Escape)
        hide();
    else if (event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier)
        QGuiApplication::clipboard()->setPixmap(fullScreen.copy(*rect));
    else if (event->key() == Qt::Key_C && event->modifiers() == Qt::AltModifier)
        saveCaptureOther();
    else if (event->key() == Qt::Key_A && event->modifiers() == Qt::AltModifier)
        grapFullCature();
    else
        event->ignore();

}

bool Capture::isPointInRect(QPoint movedPoint)
{
    // 判断鼠标拖动处是否位于截取的矩形区域内
    QRect selRect = getRect(beginPoint, endPoint);
    if (selRect.contains(movedPoint))
        return true;
    return false;

}

QRect Capture::getRect(QPoint beginPoint, QPoint endPoint)
{
    // 根据beginPoint和endPoint确定矩形区域
    int x0 = beginPoint.x() < endPoint.x() ? beginPoint.x() : endPoint.x();
    int y0 = beginPoint.y() < endPoint.y() ? beginPoint.y() : endPoint.y();
    int w = beginPoint.x() < endPoint.x() ? endPoint.x() - beginPoint.x() : beginPoint.x() - endPoint.x();
    int h = beginPoint.y() < endPoint.y() ? endPoint.y() - beginPoint.y() : beginPoint.y() - endPoint.y();

    QRect selRect = QRect(x0, y0, w, h);
    if (selRect.width() == 0)
        selRect.setWidth(1);
    if (selRect.height() == 0)
        selRect.setHeight(1);

    return selRect;
}

QRect Capture::getMoveRect()
{
    // 拖动最初截取的区域后得到的矩形区域
    QPoint movePoint = getMvePoint();
    QPoint moveBeginPoint = beginPoint + movePoint;
    QPoint moveEndPoint = endPoint + movePoint;
    // 移动后更新beginPoint和endPoint
    if (state == endMoveImage) {
        beginPoint = moveBeginPoint;
        endPoint = moveEndPoint;
        beginMovePoint = QPoint(0, 0);
        endMovePoint = QPoint(0, 0);
    }

    return getRect(moveBeginPoint, moveEndPoint);

}

QRect Capture::getAdjustRect()
{
    // 获取调整后的矩形区域
    QRect curRect = getRect(beginPoint, endPoint);

    if (adjustState == initAdjustCapture) {
        beginAdjustPoint = beginPoint;
        endAdjustPoint = endPoint;
    }
    else if (adjustState == topLeftPoint) {
        beginAdjustPoint = endMovePoint;
        endAdjustPoint = curRect.bottomRight();
    }
    else if (adjustState == topRightPoint) {
        beginAdjustPoint = QPoint(curRect.topLeft().x(), endMovePoint.y());
        endAdjustPoint = QPoint(endMovePoint.x(), curRect.bottomRight().y());
    }
    else if (adjustState == bottomLeftPoint) {
        beginAdjustPoint = QPoint(endMovePoint.x(), curRect.topLeft().y());
        endAdjustPoint = QPoint(curRect.bottomRight().x(), endMovePoint.y());
    }
    else if (adjustState == bottomRightPoint) {
        beginAdjustPoint = curRect.topLeft();
        endAdjustPoint = endMovePoint;
    }
    else if (adjustState == topCenterPoint) {
        beginAdjustPoint = QPoint(curRect.topLeft().x(), endMovePoint.y());
        endAdjustPoint = curRect.bottomRight();
    }
    else if (adjustState == bottomCenterPoint) {
        beginAdjustPoint = curRect.topLeft();
        endAdjustPoint = QPoint(curRect.bottomRight().x(), endMovePoint.y());
    }
    else if (adjustState == leftCenterPoint) {
        beginAdjustPoint = QPoint(endMovePoint.x(), curRect.topLeft().y());
        endAdjustPoint = curRect.bottomRight();
    }
    else {
        beginAdjustPoint = curRect.topLeft();
        endAdjustPoint = QPoint(endMovePoint.x(), curRect.bottomRight().y());
    }

    // 调整后更新beginPoint和endPoint
    if (state == endAdjustImage) {
        beginPoint = beginAdjustPoint;
        endPoint = endAdjustPoint;
    }

    // 返回调整后的矩形
    return getRect(beginAdjustPoint, endAdjustPoint);

}

QRect Capture::getSelectRect()
{
    // 根据截屏状态获取当前矩形区域
    if (state == beginSlideCapture || state == endSlideCapture)
        return getRect(beginPoint, endPoint);
    if (state == beginMoveImage || state == endMoveImage)
        return getMoveRect();
    if (state == beginAdjustImage || state == endAdjustImage)
        return getAdjustRect();

    return QRect(0, 0, 0, 0);

}

void Capture::drawCapture()
{
    // 绘制选取的矩形区域


}

void Capture::drawAdjustCapture()
{
    // 绘制各调整处的矩形
    QColor color = QColor(0, 125, 225);
    int wid = 6;

    // 四个角的坐标
    QPoint topLeft = curRect.topLeft();
    QPoint topRight = curRect.topRight();
    QPoint bottomLeft = curRect.bottomLeft();
    QPoint bottomRight = curRect.bottomRight();

    // 四边中点坐标
    QPoint topCenter = QPoint((topLeft.x() + topRight.x()) / 2, topLeft.y());
    QPoint bottomCenter = QPoint((bottomLeft.x() + bottomRight.x()) / 2, bottomRight.y());
    QPoint leftCenter = QPoint(topLeft.x(), (topLeft.y() + bottomLeft.y()) / 2);
    QPoint rightCenter = QPoint(topRight.x(), (topRight.y() + bottomRight.y()) / 2);

    topLeftRect = QRect(topLeft.x() - wid / 2, topLeft.y() - wid / 2, wid, wid);
    topRightRect = QRect(topRight.x() - wid / 2, topRight.y() - wid / 2, wid, wid);
    bottomLeftRect = QRect(bottomLeft.x() - wid / 2, bottomLeft.y() - wid / 2, wid, wid);
    bottomRightRect = QRect(bottomRight.x() - wid / 2, bottomRight.y() - wid / 2, wid, wid);
    topCenterRect = QRect(topCenter.x() - wid / 2, topCenter.y() - wid / 2, wid, wid);
    bottomCenterRect = QRect(bottomCenter.x() - wid / 2, bottomCenter.y() - wid / 2, wid, wid);
    leftCenterRect = QRect(leftCenter.x() - wid / 2, leftCenter.y() - wid / 2, wid, wid);
    rightCenterRect = QRect(rightCenter.x() - wid / 2, rightCenter.y() - wid / 2, wid, wid);

    //绘制填充的调整处的小矩形
    painter.fillRect(topLeftRect, color);
    painter.fillRect(topRightRect, color);
    painter.fillRect(bottomLeftRect, color);
    painter.fillRect(bottomRightRect, color);
    painter.fillRect(topCenterRect, color);
    painter.fillRect(bottomCenterRect, color);
    painter.fillRect(leftCenterRect, color);
    painter.fillRect(rightCenterRect, color);



}


QPoint Capture::getMvePoint()
{
    // 获取拖动截取的图片后的鼠标
    QPoint movePoint = endMovePoint - beginMovePoint;

    return movePoint;

}

QPoint Capture::getAdjustPoint()
{
    // 获取开始调整处鼠标的位置

    return beginAdjustPoint;
}

QPoint Capture::getOffset(QPoint beginPos, QPoint endPos)
{
    // 获取起始点的偏移
    return QPoint(endPos.x() - beginPos.x(), endPos.y() - beginPos.y());

}

adjustCaptureState Capture::getAdjustStateOfPoint(QPoint curPoint)
{
    adjustCaptureState adjustState = initAdjustCapture;

    // 获取调整时鼠标的位置状态
    if (topLeftRect.contains(curPoint))
        adjustState = topLeftPoint;
    else if (topRightRect.contains(curPoint))
        adjustState = topRightPoint;
    else if (bottomLeftRect.contains(curPoint))
        adjustState = bottomLeftPoint;
    else if (bottomRightRect.contains(curPoint))
        adjustState = bottomRightPoint;
    else if (topCenterRect.contains(curPoint))
        adjustState = topCenterPoint;
    else if (bottomCenterRect.contains(curPoint))
        adjustState = bottomCenterPoint;
    else if (leftCenterRect.contains(curPoint))
        adjustState = leftCenterPoint;
    else if (rightCenterRect.contains(curPoint))
        adjustState = rightCenterPoint;

    return adjustState;
}

void Capture::setAdjustCursor(adjustCaptureState adjustState)
{
    switch (adjustState) {
    case initAdjustCapture:
        setCursor(Qt::ArrowCursor);
        break;
    case topLeftPoint:
    case bottomRightPoint:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case topRightPoint:
    case bottomLeftPoint:
        setCursor(Qt::SizeBDiagCursor);
    case topCenterPoint:
    case bottomCenterPoint:
        setCursor(Qt::SizeVerCursor);
        break;
    case leftCenterPoint:
    case rightCenterPoint:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        break;
    }
}




