#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include <QObject>
#include <QPointer>
#include <gstreamer/gstreamervideocapture.h>
#include <gstreamer/gstreamervideowriter.h>

class VideoItem;

class VideoControl : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* videoItem READ videoItemQObject WRITE setVideoItemQObject NOTIFY videoItemChanged)

public:
    explicit VideoControl(QObject *parent = nullptr);
    void setVideoItem(VideoItem *pVideoItem);

    Q_INVOKABLE void setUrl(QString url);
    Q_INVOKABLE void playVideoStream();
    Q_INVOKABLE void pauseVideoStream();
    Q_INVOKABLE void closeVideoStream();
    Q_INVOKABLE void startRecord();
    Q_INVOKABLE void closeRecord();

    QObject* videoItemQObject() const;
    void setVideoItemQObject(QObject* obj);

signals:
    void videoItemChanged(QObject* item);

private:
    GstreamerVideoCapture mVideoCapture;
    GstreamerVideoWriter  mVideoWriter;
    QPointer<VideoItem>   mVideoItem;
    QString mURL = "";
    
};

#endif // VIDEOCONTROL_H
