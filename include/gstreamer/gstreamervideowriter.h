#ifndef GSTREAMERVIDEOWRITER_H
#define GSTREAMERVIDEOWRITER_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QImage>
#include <QTimer>
#include <gst/gst.h>
#include <gst/gstbuffer.h>
#include <gst/app/gstappsrc.h>

class GstreamerVideoWriter : public QThread
{
    Q_OBJECT
public:
    explicit GstreamerVideoWriter(QObject *parent = nullptr);
    ~GstreamerVideoWriter();
public slots:
    void pushImage(QImage pImage);
    void record(int pWidth, int pHeight, float pFPS);
    void close();
private:
    void init();
    void clean();
    QString createPipeline();
private slots:
    void recording();
private:
    QImage mImage;
    QString mPipelineString = "";
    QString mFileName = "";
    QString mFormat = "BGRA";
    QTimer mRecordTimer;
    GstElement *mPipeline = nullptr;
    GstAppSrc  *mAppSrc = nullptr;

    int mWidth = 640;
    int mHeight = 480;
    float mFrameRate = 30;
    int mNumFrames = 0;

    static const QString APPSRC_NAME;
signals:
    void recordVideo();
    void closeRecordVideo();
};

#endif // GSTREAMERVIDEOWRITER_H
