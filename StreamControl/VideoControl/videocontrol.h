#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include <QObject>
#include <Gstreamer/gstreamervideocapture.h>

class VideoItem;

class VideoControl : public QObject
{
    Q_OBJECT
public:
    explicit VideoControl(QObject *parent = nullptr);
    void setVideoItem(VideoItem *pVideoItem);
    Q_INVOKABLE void setUrl(QString url);
    Q_INVOKABLE void playVideoStream();
    Q_INVOKABLE void pauseVideoStream();
    Q_INVOKABLE void closeVideoStream();
private:
    GstreamerVideoCapture videoCapture;
    VideoItem *videoItem;
    QString url = "";
signals:
};

#endif // VIDEOCONTROL_H
