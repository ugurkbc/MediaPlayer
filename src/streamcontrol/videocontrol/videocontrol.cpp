#include "streamcontrol/videocontrol/videocontrol.h"
#include <QFile>
#include <QDebug>
#include "qml/videoitem.h"

VideoControl::VideoControl(QObject *parent)
    : QObject{parent}
{

}

void VideoControl::setVideoItem(VideoItem *pVideoItem)
{
    videoItem = pVideoItem;

    connect(&videoCapture, &GstreamerVideoCapture::newImage, videoItem, &VideoItem::newImage, Qt::DirectConnection);
}

void VideoControl::setUrl(QString url)
{
    this->url = url;
    qDebug() << "The url set:" << this->url;
}

void VideoControl::playVideoStream()
{
    if(videoCapture.isRunning()){
        videoCapture.play();
    }
    else{
        if (QFile::exists(url)) {
            QString str = QString("filesrc location=%1 ! qtdemux ! h264parse ! avdec_h264 ! videoconvert ! video/x-raw,format=RGB ! appsink name=sink").arg(url);
            videoCapture.play(str);
        } else {
            qDebug() << "Error: File does not exist or URL is invalid!";
        }
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
