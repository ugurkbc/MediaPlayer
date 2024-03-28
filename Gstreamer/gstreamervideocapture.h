#ifndef GSTREAMERVIDEOCAPTURE_H
#define GSTREAMERVIDEOCAPTURE_H

#include <QThread>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>

class GstreamerVideoCapture : public QThread
{
    Q_OBJECT
public:
    explicit GstreamerVideoCapture(const char* pipelineString, QObject *parent = nullptr);
    ~GstreamerVideoCapture();

public:
    void play();
    void play(const char* pipelineString);
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
    const char* pipelineString;
    GstElement* pipeline;
    GstBus* bus;
    GstElement* appsink;
    GMainLoop* loop;

    gint width;
    gint height;
    double frameRate;
    std::string format;

signals:
    void newImage(const QImage &);
};

#endif // GSTREAMERVIDEOCAPTURE_H
