#include "videocontrol.h"
#include <QFile>
#include <QDebug>
#include <videoitem.h>

VideoControl::VideoControl(QObject *parent)
    : QObject{parent}
{
    //connect(&videoCapture, &GstreamerVideoCapture::newImage, videoItem, &VideoItem::newImage, Qt::DirectConnection);
}

void VideoControl::setUrl(QString url)
{
    this->url = url;
    qDebug() << "The url set:" << this->url;
}

void VideoControl::playVideoStream()
{
    if (QFile::exists(url)) {
        QString pipeline = QString("filesrc location=%1 ! qtdemux ! h264parse ! avdec_h264 ! videoconvert ! video/x-raw,format=RGB ! appsink name=sink").arg(url);
        videoCapture.play(pipeline.toUtf8().constData());
    } else {
        qDebug() << "Error: File does not exist or URL is invalid!";
    }
}

void VideoControl::pauseVideoStream()
{
    videoCapture.pause();
}

void VideoControl::closeVideoStream()
{
    videoCapture.close();
}
