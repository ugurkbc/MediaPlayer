#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include <QObject>
#include <gstreamer/gstreamervideocapture.h>
#include <gstreamer/gstreamervideowriter.h>

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
    Q_INVOKABLE void startRecord();
    Q_INVOKABLE void closeRecord();
private:
    GstreamerVideoCapture mVideoCapture;
    GstreamerVideoWriter mVideoWriter;
    VideoItem *mVideoItem;
    QString mURL = "";
signals:
};

#endif // VIDEOCONTROL_H
