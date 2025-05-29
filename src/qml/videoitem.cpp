#include "qml/videoitem.h"
#include <QPainter>
#include <QDebug>

QImage CreateCustomImage(){
    int width = 100;
    int height = 100;

    QImage image(width, height, QImage::Format_RGB888);

    QRgb red = qRgb(255, 0, 0);
    image.fill(red);

    return image;
}

VideoItem::VideoItem() {

    //mImage = CreateCustomImage();
}

void VideoItem::paint(QPainter *painter)
{
    mMutex.lock();

    QPointF centerPos = QPointF(boundingRect().center().x() - mImage.width() / 2,
                                boundingRect().center().y() - mImage.height() / 2);

    painter->drawImage(centerPos, mImage);

    emit onNewFrame(mImage);

    mMutex.unlock();
}

void VideoItem::newImage(QImage &pImage)
{
    mMutex.lock();

    mImage = pImage;

    mMutex.unlock();

    QMetaObject::invokeMethod(this, "update");
}
