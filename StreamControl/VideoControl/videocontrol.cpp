#include "videocontrol.h"
#include <QFile>
#include <QDebug>
#include <videoitem.h>

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
    //const char *pipeline = "filesrc location=C:/Users/ugurk/Videos/video.mp4 ! qtdemux ! h264parse ! avdec_h264 ! videoconvert ! video/x-raw,format=RGB ! appsink name=sink";

    if (QFile::exists(url)) {
        QString str = QString("filesrc location=%1 ! qtdemux ! h264parse ! avdec_h264 ! videoconvert ! video/x-raw,format=RGB ! appsink name=sink").arg(url);
        videoCapture.play(str);
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
