#include "streamcontrol/videocontrol/videocontrol.h"

#include <QFile>
#include <QDebug>
#include <QMetaObject>
#include "ui/videoitem.h"

VideoControl::VideoControl(QObject *parent)
    : QObject{parent}
{
}

QObject* VideoControl::videoItemQObject() const 
{ 
    return mVideoItem.data(); 
}

void VideoControl::setVideoItemQObject(QObject* obj)
{
    auto vi = qobject_cast<VideoItem*>(obj);
    setVideoItem(vi);
    emit videoItemChanged(obj);
}

void VideoControl::setVideoItem(VideoItem *pVideoItem)
{
    if (mVideoItem == pVideoItem)
        return;

    if (mVideoItem) {
        mVideoCapture.disconnect(mVideoItem);
        mVideoItem->disconnect(&mVideoWriter);
    }

    mVideoItem = pVideoItem;

    if (!mVideoItem)
        return;

    connect(&mVideoCapture, &GstreamerVideoCapture::newImage, mVideoItem.data(), &VideoItem::newImage, Qt::DirectConnection);

    connect(mVideoItem.data(), &VideoItem::onNewFrame, &mVideoWriter, &GstreamerVideoWriter::pushImage, Qt::QueuedConnection);
}

void VideoControl::setUrl(QString url)
{
    mURL = url;
    qDebug() << "The url set:" << mURL;
}

void VideoControl::playVideoStream()
{
    if (mVideoCapture.isRunning()) {
        mVideoCapture.play();
    } else {
        if (QFile::exists(mURL)) {
            const QString pipeline =
                QStringLiteral("filesrc location=%1 ! qtdemux ! h264parse ! avdec_h264 ! videoconvert ! video/x-raw,format=RGB ! appsink name=sink")
                .arg(mURL);
            mVideoCapture.play(pipeline);
        } else {
            qDebug() << "Error: File does not exist or URL is invalid!";
        }
    }
}

void VideoControl::pauseVideoStream()
{
    mVideoCapture.pause();
}

void VideoControl::closeVideoStream()
{
    mVideoCapture.close();
}

void VideoControl::startRecord()
{
    QMetaObject::invokeMethod(&mVideoWriter, "record", Qt::QueuedConnection,
        Q_ARG(QString,  QStringLiteral("video")),
        Q_ARG(int,      640),
        Q_ARG(int,      480),
        Q_ARG(float,    30.0f));
}

void VideoControl::closeRecord()
{
    QMetaObject::invokeMethod(&mVideoWriter, "close", Qt::QueuedConnection);
}
