#include "gstreamer/gstreamervideocapture.h"
#include <QImage>
#include <QDebug>
#include <QFile>
#include <QUrl>
#include <QRegularExpression>
#include <gst/pbutils/gstdiscoverer.h>

GstreamerVideoCapture::GstreamerVideoCapture(QObject *parent)
    : QThread{parent}
{
    bufferCallbacks[static_cast<unsigned int>(VideoSource::FILESOURCE)] = GstreamerVideoCapture::newBufferCallbackFileSource;
    bufferCallbacks[static_cast<unsigned int>(VideoSource::REALTIME)] = GstreamerVideoCapture::newBufferCallbackRealTime;
}

GstreamerVideoCapture::~GstreamerVideoCapture()
{
    if(isRunning()){
        close();
        wait();
    }
}

static void cleanUpGstBuffer(void *imageBuffer){
   gst_buffer_unref((GstBuffer *) imageBuffer);
}

void GstreamerVideoCapture::play(QString str)
{
    pipelineString = str;

    if(!isRunning()){
        start();
    }
    else{
        if(paused){
            gst_element_set_state(pipeline, GST_STATE_PLAYING);
            paused = false;
        }
    }
}

void GstreamerVideoCapture::play()
{
    if(!isRunning()){
        start();
    }
    else{
        if(paused){
            gst_element_set_state(pipeline, GST_STATE_PLAYING);
            paused = false;
        }
    }
}

void GstreamerVideoCapture::pause()
{
    if(isRunning()){
        gst_element_set_state(pipeline, GST_STATE_PAUSED);
        paused = true;
    }
}

void GstreamerVideoCapture::close()
{
    if(isRunning()){
        gst_element_send_event(pipeline, gst_event_new_eos());

        if(paused){
            gst_element_set_state(pipeline, GST_STATE_PLAYING);
            paused = false;
        }
    }
}

void GstreamerVideoCapture::init()
{
    QString lLocationFile = extractFilePathFromPipeline(pipelineString);
    unsigned int index;

    if(lLocationFile.size() == 0){
        index = static_cast<unsigned int>(VideoSource::REALTIME);
    }
    else{
        index = static_cast<unsigned int>(VideoSource::FILESOURCE);

        if(!getVideoFeaturesFromFileLocation(lLocationFile)){
            return;
        }
    }

    pipeline = gst_parse_launch(pipelineString.toStdString().c_str(), nullptr);
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_signal_watch(bus);

    appsink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    g_object_set(appsink,
             "emit-signals", TRUE,
             "sync", TRUE,                 // ✅ let the pipeline clock pace samples
             "max-buffers", 2,             // small queue; prevents latency buildup
             "drop", TRUE,                 // drop if the UI can’t keep up
             "enable-last-sample", FALSE,  // don’t keep a cached sample
             nullptr);

    g_signal_connect(bus, "message", G_CALLBACK(busCallback), this);
    g_signal_connect(appsink, "new-sample", G_CALLBACK(bufferCallbacks[index]), this);
}

void GstreamerVideoCapture::clean()
{
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);

    pipelineString = nullptr;
    pipeline = nullptr;
    bus = nullptr;
    appsink = nullptr;
    loop = nullptr;

    width = 0;
    height = 0;
    frameRate = 0;
    paused = 0;
}

bool GstreamerVideoCapture::getVideoFeaturesFromFileLocation(const QString &fileLocation){
    QString lGSTPath = QUrl::fromLocalFile(fileLocation).toString();

    if(lGSTPath.size() == 0) return false;

    GstDiscoverer *lDiscoverer = gst_discoverer_new(5 * GST_SECOND, nullptr);
    GstDiscovererInfo *lInfo = gst_discoverer_discover_uri(lDiscoverer, lGSTPath.toStdString().c_str(), nullptr);
    const GList *lVideoStreams = gst_discoverer_info_get_video_streams(lInfo);

    for (const GList *item = lVideoStreams; item != nullptr; item = item->next) {
            GstDiscovererVideoInfo *lVideoInfo = static_cast<GstDiscovererVideoInfo *>(item->data);
            guint num = gst_discoverer_video_info_get_framerate_num(lVideoInfo);
            guint denom = gst_discoverer_video_info_get_framerate_denom(lVideoInfo);
            width = gst_discoverer_video_info_get_width(lVideoInfo);
            height = gst_discoverer_video_info_get_height(lVideoInfo);
            if (denom > 0) {
                qDebug() << "Framerate:" << num << "/" << denom;
                frameRate = (float)num / denom;
            }
        }

    gst_object_unref(lDiscoverer);
    return true;
}

gboolean GstreamerVideoCapture::busCallback(GstBus* bus, GstMessage* message, gpointer data) {
    auto capture = static_cast<GstreamerVideoCapture*>(data);

    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_EOS:
        qDebug() << "End of Stream";
        g_main_loop_quit(capture->loop);
        break;

    case GST_MESSAGE_ERROR: {
        GError* error = nullptr;
        gchar* debugInfo = nullptr;
        gst_message_parse_error(message, &error, &debugInfo);
        qDebug() << "Error: " << error->message;
        if (debugInfo) {
            qDebug() << "Debug information: " << debugInfo;
            g_free(debugInfo);
        }
        g_error_free(error);
        g_main_loop_quit(capture->loop);
        break;
    }

     case GST_MESSAGE_STATE_CHANGED: {
         // Log state changes
         GstState oldState, newState, pendingState;

         // Parse the state change message to obtain old, new, and pending states
         gst_message_parse_state_changed(message, &oldState, &newState, &pendingState);
/*
         // Print the details of the state change
         qDebug() << "State changed from " << gst_element_state_get_name(oldState)
         << " to " << gst_element_state_get_name(newState);

         // Check if there is a pending state (transition in progress)
         if (pendingState != GST_STATE_VOID_PENDING) {
             qDebug() << " (pending: " << gst_element_state_get_name(pendingState) << ")";
         }

         qDebug() << "\n";
         */
         break;
     }


    default:
        break;
    }

    return TRUE;
}

GstFlowReturn GstreamerVideoCapture::newBufferCallbackFileSource(GstElement* appsink, gpointer data) {
    auto capture = static_cast<GstreamerVideoCapture*>(data);

    GstSample* sample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
    if (sample) {
        GstBuffer *buffer = gst_sample_get_buffer(sample);
        if(buffer){
            GstMapInfo map;
            if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {

                QImage image(map.data, capture->width, capture->height, QImage::Format_ARGB32_Premultiplied, cleanUpGstBuffer, buffer);

                emit capture->newImage(image);

                gst_buffer_ref(buffer);
                gst_buffer_unmap(buffer, &map);
            }
        }
        gst_sample_unref(sample);
    }

    return GST_FLOW_OK;
}

GstFlowReturn GstreamerVideoCapture::newBufferCallbackRealTime(GstElement* appsink, gpointer data) {
    auto capture = static_cast<GstreamerVideoCapture*>(data);

    GstSample* sample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
    if (sample) {
        GstBuffer *buffer = gst_sample_get_buffer(sample);
        if(buffer){
            GstMapInfo map;
            if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {

                GstCaps *lCaps = nullptr;
                lCaps = gst_sample_get_caps((GstSample *)sample);

                GstStructure *lStructure = nullptr;
                lStructure = gst_caps_get_structure(lCaps, 0);

                gst_structure_get_int (lStructure, "width", &capture->width);
                gst_structure_get_int (lStructure, "height", &capture->height);

                QImage image(map.data, capture->width, capture->height, QImage::Format_ARGB32_Premultiplied, cleanUpGstBuffer, buffer);

                emit capture->newImage(image);

                gst_buffer_ref(buffer);
                gst_buffer_unmap(buffer, &map);
            }
        }
        gst_sample_unref(sample);
    }

    return GST_FLOW_OK;
}

QString GstreamerVideoCapture::extractFilePathFromPipeline(const QString &pipeline) {
    QRegularExpression regex(R"(filesrc\s+location=([^\s!]+))");
    QRegularExpressionMatch match = regex.match(pipeline);
    if (match.hasMatch()) {
        return match.captured(1); // first capture group
    }
    return QString();
}

void GstreamerVideoCapture::run()
{
    init();

    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    loop = g_main_loop_new(nullptr, FALSE);

    emit videoPlay();

    if(ret != GST_STATE_CHANGE_FAILURE){
        g_main_loop_run(loop);
    }

    QImage lImage(width, height, QImage::Format_ARGB32_Premultiplied);
    lImage.fill(Qt::black);
    emit newImage(lImage);

    emit closeVideoPlay();

    clean();
}


