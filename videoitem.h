#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QMutexLocker>
#include <QMutex>
#include <QPainter>

class VideoItem : public QQuickPaintedItem
{
    Q_OBJECT
public:
    VideoItem();

private:
    QImage mImage;
    QMutex mMutex;
    QMutexLocker<QMutex> mLocker;

protected:
    void paint(QPainter *painter) override;

public slots:
    void newImage(QImage &pImage);

signals:
    void onNewFrame(QImage);
};

#endif // VIDEOITEM_H
