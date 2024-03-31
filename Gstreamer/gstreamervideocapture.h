#ifndef GSTREAMERVIDEOCAPTURE_H
#define GSTREAMERVIDEOCAPTURE_H

#include <QThread>
#include <QString>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>

class GstreamerVideoCapture : public QThread
{
    Q_OBJECT
public:
    explicit GstreamerVideoCapture(QObject *parent = nullptr);

    ~GstreamerVideoCapture();

public:
    void play(QString str);
    void pause();
    void close();
private:
    void init();
    void clean();
    static gboolean busCallback(GstBus* bus, GstMessage* message, gpointer data);
    static GstFlowReturn newBufferCallback(GstElement* appsink, gpointer data);
    static void padAddedCallback(GstElement* appsink, GstPad* pad, gpointer data);

protected:
    void run() override;
private:
    QString pipelineString;
    GstElement* pipeline;
    GstBus* bus;
    GstElement* appsink;
    GMainLoop* loop;

    gint width;
    gint height;
    double frameRate;
    std::string format;

signals:
    void newImage(QImage &);
};

#endif // GSTREAMERVIDEOCAPTURE_H
