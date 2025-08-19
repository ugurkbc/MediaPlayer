#include "ui/videoitem.h"

#include <QQuickWindow>
#include <QSGSimpleTextureNode>
#include <QSGTexture>
#include <QMutexLocker>

VideoItem::VideoItem(QQuickItem *parent): QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

void VideoItem::setKeepAspect(bool v)
{
    if (mKeepAspect == v) return;
    mKeepAspect = v;
    emit keepAspectChanged();
    mGeometryDirty = true;
    update();
}

void VideoItem::newImage(const QImage &pImage)
{
    if (pImage.isNull())
        return;

    QImage prepared = (pImage.format() == QImage::Format_ARGB32_Premultiplied)
                        ? pImage
                        : pImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    {
        QMutexLocker lock(&mMutex);
        mImagePending = std::move(prepared);
        mHasNewFrame = true;
    }

    QMetaObject::invokeMethod(this, [this](){ this->update();}, Qt::QueuedConnection);
}

QRectF VideoItem::fittedRect(const QSize &imgSize, const QRectF &box) const
{
    if (!mKeepAspect || imgSize.isEmpty())
        return box;

    QSizeF scaled = imgSize;
    scaled.scale(box.size(), Qt::KeepAspectRatio);

    const QPointF tl(
        box.center().x() - scaled.width()  * 0.5,
        box.center().y() - scaled.height() * 0.5
    );
    return QRectF(tl, scaled);
}

QSGNode* VideoItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    auto *node = static_cast<QSGSimpleTextureNode*>(oldNode);

    // If there is no window, we cannot create textures → no node.
    if (!window()) {
        delete node;
        return nullptr;
    }

    // Pull the newest frame (if any)
    QImage frame;
    bool hasNew = false;
    {
        QMutexLocker lock(&mMutex);
        if (mHasNewFrame) {
            frame = mImagePending;
            mHasNewFrame = false;
            hasNew = true;
        }
    }

    if (hasNew && !frame.isNull()) {
        if (!node)
            node = new QSGSimpleTextureNode();

        // Create texture for the new frame
        QSGTexture *tex = window()->createTextureFromImage(frame);

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        // Let the node delete the texture when replaced/destroyed
        node->setOwnsTexture(true);
#else
        // Manual ownership on Qt < 5.12
        if (node->texture())
            delete node->texture();
#endif
        node->setTexture(tex);
        node->setRect(fittedRect(frame.size(), boundingRect()));
        mGeometryDirty = false;
        return node;
    }

    // No new frame:
    if (node && node->texture()) {
        // We already have a texture; maybe geometry changed
        if (mGeometryDirty) {
            node->setRect(fittedRect(node->texture()->textureSize(), boundingRect()));
            mGeometryDirty = false;
        }
        return node;
    }

    // No texture to sample → remove node to avoid the warning
    delete node;
    return nullptr;
}

void VideoItem::releaseResources()
{
    // Node tree is deleted by SG; we free textures when replacing them.
    // If you kept extra GPU resources, release them here (render thread).
}

void VideoItem::geometryChange(const QRectF &newG, const QRectF &oldG)
{
    QQuickItem::geometryChange(newG, oldG);
    if (newG.size() != oldG.size()) {
        mGeometryDirty = true;
        update();
    }
}

void VideoItem::itemChange(ItemChange change, const ItemChangeData &d)
{
    QQuickItem::itemChange(change, d);
    if (change == ItemSceneChange) {
        // Window changed; textures are window-bound; recompute rect next frame
        mGeometryDirty = true;
        update();
    }
}
