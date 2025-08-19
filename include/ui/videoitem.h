#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include <QQuickItem>
#include <QImage>
#include <QMutex>

class VideoItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool keepAspect READ keepAspect WRITE setKeepAspect NOTIFY keepAspectChanged)

public:
    explicit VideoItem(QQuickItem *parent = nullptr);

    bool keepAspect() const { return mKeepAspect; }
    void setKeepAspect(bool v);

public slots:
    void newImage(const QImage &pImage);

signals:
    void onNewFrame(const QImage &pImage);
    void keepAspectChanged();

protected:
    QSGNode* updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;
    void releaseResources() override;
    void geometryChange(const QRectF &newG, const QRectF &oldG) override;
    void itemChange(ItemChange change, const ItemChangeData &d) override;

private:
    QRectF fittedRect(const QSize &imgSize, const QRectF &box) const;

private:
    QImage mImagePending;
    bool   mHasNewFrame = false;
    QMutex mMutex;

    bool mKeepAspect = true;
    bool mGeometryDirty = true;
};

#endif // VIDEOITEM_H
