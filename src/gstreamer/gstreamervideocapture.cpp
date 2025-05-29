#include "gstreamer/gstreamervideocapture.h"
#include <iostream>
#include <QImage>

GstreamerVideoCapture::GstreamerVideoCapture(QObject *parent)
    : pipeline(nullptr), bus(nullptr),
    appsink(nullptr), width(0), height(0), frameRate(0.0), loop(nullptr),
    format(""), paused(false), QThread{parent}
{
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
    pipeline = gst_parse_launch(pipelineString.toStdString().c_str(), nullptr);
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_signal_watch(bus);

    appsink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");
    g_object_set(appsink, "emit-signals", TRUE, "sync", FALSE, nullptr);

    g_signal_connect(bus, "message", G_CALLBACK(busCallback), this);
    g_signal_connect(appsink, "new-sample", G_CALLBACK(newBufferCallback), this);
    g_signal_connect(appsink, "pad-added", G_CALLBACK(padAddedCallback), this);
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
    format = "";

    paused = false;
}

gboolean GstreamerVideoCapture::busCallback(GstBus* bus, GstMessage* message, gpointer data) {
    auto capture = static_cast<GstreamerVideoCapture*>(data);

    switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_EOS:
        std::cout << "End of Stream" << std::endl;
        g_main_loop_quit(capture->loop);
        break;

    case GST_MESSAGE_ERROR: {
        GError* error = nullptr;
        gchar* debugInfo = nullptr;
        gst_message_parse_error(message, &error, &debugInfo);
        std::cerr << "Error: " << error->message << std::endl;
        if (debugInfo) {
            std::cerr << "Debug information: " << debugInfo << std::endl;
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

         // Print the details of the state change
         std::cout << "State changed from " << gst_element_state_get_name(oldState);
         std::cout << " to " << gst_element_state_get_name(newState);

         // Check if there is a pending state (transition in progress)
         if (pendingState != GST_STATE_VOID_PENDING) {
             std::cout << " (pending: " << gst_element_state_get_name(pendingState) << ")";
         }

         std::cout << std::endl;
         break;
     }


    default:
        break;
    }

    return TRUE;
}

GstFlowReturn GstreamerVideoCapture::newBufferCallback(GstElement* appsink, gpointer data) {
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

                QImage image(map.data, capture->width, capture->height, QImage::Format_RGB888, cleanUpGstBuffer, buffer);

                emit capture->newImage(image);

                gst_buffer_ref(buffer);
                gst_buffer_unmap(buffer, &map);
            }
        }
        gst_sample_unref(sample);
    }

    return GST_FLOW_OK;
}

void GstreamerVideoCapture::padAddedCallback(GstElement* appsink, GstPad* pad, gpointer data) {
    auto capture = static_cast<GstreamerVideoCapture*>(data);

    GstCaps* padCaps = gst_pad_get_current_caps(pad);
    if (padCaps) {
        GstStructure* structure = gst_caps_get_structure(padCaps, 0);

        gst_structure_get(structure, "width", G_TYPE_INT, &capture->width, nullptr);
        gst_structure_get(structure, "height", G_TYPE_INT, &capture->height, nullptr);

        // Retrieve frame rate directly
        gst_structure_get(structure, "framerate", GST_TYPE_FRACTION, &capture->frameRate, nullptr);

        const gchar* formatString = gst_structure_get_string(structure, "format");
        if (formatString) {
            capture->format = formatString;
            std::cout << "Image Format: " << formatString << std::endl;
        }

        gst_caps_unref(padCaps);
    }
}

void GstreamerVideoCapture::run()
{
    init();

    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    loop = g_main_loop_new(nullptr, FALSE);

    if(ret != GST_STATE_CHANGE_FAILURE){
        g_main_loop_run(loop);
    }

    clean();
}


