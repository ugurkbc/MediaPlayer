#include "videoitem.h"
#include <QPainter>
#include <QDebug>

VideoItem::VideoItem(): mLocker(&mMutex) {

    int width = 100;
    int height = 100;

    QImage image(width, height, QImage::Format_RGB888);

    QRgb red = qRgb(255, 0, 0);
    image.fill(red);

    mImage = image;
}

void VideoItem::paint(QPainter *painter)
{
    QPointF centerPos = QPointF(boundingRect().center().x() - mImage.width() / 2,
                                boundingRect().center().y() - mImage.height() / 2);

    painter->drawImage(centerPos, mImage);

    emit onNewFrame(mImage);

    if(mLocker.isLocked()){
        mLocker.unlock();
    }
}

void VideoItem::newImage(QImage pImage)
{
    mLocker.relock();

    mImage = pImage;

    update();
}