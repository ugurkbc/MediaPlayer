#include "gstreamer/GstreamerVideoWriter.h"
#include <utils.h>
#include <QDateTime>
#include <QDir>
#include <QPainter>
#include <QtMath>

const QString GstreamerVideoWriter::APPSRC_NAME = "mysrc";

static void paintTestPattern(QImage &img, int frameIndex)
{
    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Background gradient
    QLinearGradient g(0, 0, img.width(), img.height());
    g.setColorAt(0.0, QColor(20, 20, 40));
    g.setColorAt(1.0, QColor(40, 20, 20));
    p.fillRect(img.rect(), g);

    // Border
    p.setPen(QPen(Qt::yellow, 3));
    p.drawRect(img.rect().adjusted(1,1,-2,-2));

    // Crosshair
    p.setPen(QPen(Qt::green, 1));
    p.drawLine(img.width()/2, 0, img.width()/2, img.height());
    p.setPen(QPen(Qt::red, 1));
    p.drawLine(0, img.height()/2, img.width(), img.height()/2);

    // Moving square
    int r = 50;
    int x = (frameIndex * 5) % qMax(1, img.width() - r);
    int y = (frameIndex * 3) % qMax(1, img.height() - r);
    p.setBrush(QColor(0, 150, 255, 200));
    p.setPen(QPen(Qt::white, 2));
    p.drawRect(x, y, r, r);

    // Moving circle
    int cx = img.width()/2  + int(qCos(frameIndex * 0.1) * img.width()/4.0);
    int cy = img.height()/2 + int(qSin(frameIndex * 0.1) * img.height()/4.0);
    p.setBrush(QColor(255, 120, 0, 200));
    p.setPen(QPen(Qt::white, 2));
    p.drawEllipse(QPoint(cx, cy), 30, 30);

    // Text overlay
    p.setPen(Qt::white);
    QFont f = p.font();
    f.setPointSize(qMax(10, img.width()/32));
    p.setFont(f);
    p.drawText(QRect(8, 8, img.width()-16, 40),
               Qt::AlignLeft | Qt::AlignVCenter,
               QString("%1x%2  frame %3")
                   .arg(img.width()).arg(img.height()).arg(frameIndex));
}

GstreamerVideoWriter::GstreamerVideoWriter(QObject *parent): QThread(parent), mRecordTimer(this)
{
    gst_init(nullptr, nullptr);

    connect(&mRecordTimer, &QTimer::timeout, this, &GstreamerVideoWriter::recording);
}

GstreamerVideoWriter::~GstreamerVideoWriter()
{
    quit();
    wait();

    clean();
}

QString GstreamerVideoWriter::createPipeline()
{
    int fpsNum   = mFrameRate;
    int fpsDenom = 1;

    return QString("appsrc name=" + APPSRC_NAME + " ! video/x-raw, format=(string)" + mFormat + ", width=(int)"
                   + QString::number(mWidth)+ ", height=(int)" + QString::number(mHeight) + ", framerate=(fraction)" + QString::number(fpsNum) + "/"
                   + QString::number(fpsDenom) + " ! videoconvert ! video/x-raw, format=(string)I420 ! x264enc ! h264parse ! mp4mux ! filesink location=" + mFileName);
}

void GstreamerVideoWriter::pushImage(QImage pImage)
{
    mImage = pImage.scaled(mWidth, mHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    //paintTestPattern(mImage, mNumFrames);
}

void GstreamerVideoWriter::record(int pWidth, int pHeight, float pFPS)
{
    if(mRecordTimer.isActive()) return;

    mWidth  = pWidth  > 0 ? pWidth  : mWidth;
    mHeight = pHeight > 0 ? pHeight : mHeight;
    mFrameRate = (pFPS > 0) ? pFPS : mFrameRate;
    mNumFrames = 0;
    mFileName = Utils::getInstance()->generateFileName();
    mPipelineString = createPipeline();

    int intervalMs = static_cast<int>(1000.0 / mFrameRate);
    mRecordTimer.setInterval(intervalMs);

    mImage = QImage(mWidth, mHeight, QImage::QImage::Format_RGB888);
    mImage.fill(Qt::black);
    //paintTestPattern(mImage, 0);

    init();
}

void GstreamerVideoWriter::close()
{
    clean();
}

void GstreamerVideoWriter::init()
{
    GError *error = nullptr;
    mPipeline = gst_parse_launch(mPipelineString.toUtf8().constData(), &error);

    if (!mPipeline || error) {
        qWarning("Failed to create GStreamer pipeline: %s", error ? error->message : "unknown error");
        if (error) g_error_free(error);
        clean();
        return;
    }

    GstElement *element = gst_bin_get_by_name(GST_BIN(mPipeline), APPSRC_NAME.toUtf8().constData());
    if (!element) {
        qWarning("Failed to get appsrc element from pipeline");
        clean();
        return;
    }
    mAppSrc = GST_APP_SRC(element);

    gst_app_src_set_stream_type(mAppSrc, GST_APP_STREAM_TYPE_STREAM);
    g_object_set(G_OBJECT(mAppSrc),
                 "format", GST_FORMAT_TIME,
                 "is-live", TRUE,          // good for real-time push
                 "block", TRUE,            // block when downstream is slow
                 "do-timestamp", TRUE,
                 NULL);
    gst_app_src_set_max_bytes(mAppSrc, static_cast<guint64>(mWidth) * mHeight * 4 * 4);

    GstStateChangeReturn ret = gst_element_set_state(mPipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        qWarning("Failed to set pipeline to PLAYING");
        clean();
        return;
    }

    mRecordTimer.start();
}

void GstreamerVideoWriter::clean()
{
    mRecordTimer.stop();

    if (mAppSrc){
        if(gst_app_src_end_of_stream(mAppSrc) != GST_FLOW_OK){
            qDebug() << "Cannot send EOS to GStreamer pipeline";
        }

        GstBus* bus = gst_element_get_bus(mPipeline);
        GstMessage* msg = gst_bus_timed_pop_filtered(bus, 5 * GST_SECOND, (GstMessageType)(GST_MESSAGE_EOS | GST_MESSAGE_ERROR));
        if (msg) gst_message_unref(msg);
        gst_object_unref(bus);
    }

    if (mPipeline) {
        gst_element_set_state(mPipeline, GST_STATE_NULL);
        gst_object_unref(mPipeline);
        mPipeline = nullptr;
    }

    if (mAppSrc) {
        gst_object_unref(mAppSrc);
        mAppSrc = nullptr;
    }
}

void GstreamerVideoWriter::recording()
{
    if(!mAppSrc) return;

    if(mImage.isNull()) return;

    int lSize = mImage.sizeInBytes();

    GstBuffer *lBuffer = gst_buffer_new_allocate(nullptr, lSize, nullptr);
    GstMapInfo lInfo;
    gst_buffer_map(lBuffer, &lInfo, (GstMapFlags)GST_MAP_WRITE);
    memcpy(lInfo.data, (guint8*)mImage.bits(), lSize);
    gst_buffer_unmap(lBuffer, &lInfo);

    if (gst_app_src_push_buffer(mAppSrc, lBuffer) == GST_FLOW_OK)
    {
        ++mNumFrames;
    }
    else{
        gst_buffer_unref(lBuffer);
    }
}
