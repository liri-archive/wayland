// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QWaylandOutput>

#include "waylandsurfacelayout_p.h"
#include "waylandwlrlayershellv1.h"
#include "waylandwlrlayersurfacev1item.h"

/*
 * WaylandSurfaceLayoutPrivate
 */

WaylandSurfaceLayoutPrivate::WaylandSurfaceLayoutPrivate(WaylandSurfaceLayout *self)
    : q_ptr(self)
{
}

void WaylandSurfaceLayoutPrivate::applyExclusive(WaylandWlrLayerSurfaceV1 *layerSurface, QRectF *availableGeometry)
{
    if (layerSurface->exclusiveZone() <= 0)
        return;

    switch (layerSurface->anchors()) {
    case WaylandWlrLayerSurfaceV1::LeftAnchor:
    case WaylandWlrLayerSurfaceV1::LeftAnchor | WaylandWlrLayerSurfaceV1::TopAnchor | WaylandWlrLayerSurfaceV1::BottomAnchor: {
        auto margin = layerSurface->exclusiveZone() + layerSurface->leftMargin();
        availableGeometry->adjust(margin, 0, 0, 0);
        break;
    }
    case WaylandWlrLayerSurfaceV1::RightAnchor:
    case WaylandWlrLayerSurfaceV1::RightAnchor | WaylandWlrLayerSurfaceV1::TopAnchor | WaylandWlrLayerSurfaceV1::BottomAnchor: {
        auto margin = layerSurface->exclusiveZone() + layerSurface->rightMargin();
        availableGeometry->adjust(0, 0, -margin, 0);
        break;
    }
    case WaylandWlrLayerSurfaceV1::TopAnchor:
    case WaylandWlrLayerSurfaceV1::TopAnchor | WaylandWlrLayerSurfaceV1::LeftAnchor | WaylandWlrLayerSurfaceV1::RightAnchor: {
        auto margin = layerSurface->exclusiveZone() + layerSurface->topMargin();
        availableGeometry->adjust(0, margin, 0, 0);
        break;
    }
    case WaylandWlrLayerSurfaceV1::BottomAnchor:
    case WaylandWlrLayerSurfaceV1::BottomAnchor | WaylandWlrLayerSurfaceV1::LeftAnchor | WaylandWlrLayerSurfaceV1::RightAnchor: {
        auto margin = layerSurface->exclusiveZone() + layerSurface->bottomMargin();
        availableGeometry->adjust(0, 0, 0, -margin);
        break;
    }
    default:
        break;
    }
}

void WaylandSurfaceLayoutPrivate::arrangeLayer(WaylandWlrLayerShellV1::Layer layer,
                                               bool exclusive, QRectF *availableGeometry)
{
    Q_Q(WaylandSurfaceLayout);

    for (auto *item : q->childItems()) {
        // We are only interested in layer surface items
        auto *layerItem = qobject_cast<WaylandWlrLayerSurfaceV1Item *>(item);
        if (!layerItem)
            continue;

        auto *layerSurface = layerItem->layerSurface();

        // Skip layer surfaces that belong to another layer
        if (layerSurface->layer() != layer)
            continue;

        // Check if it has an exclusive zone or not
        if (exclusive != layerSurface->exclusiveZone() > 0)
            continue;

        QRectF bounds = layerSurface->exclusiveZone() == -1 ? q->boundingRect() : *availableGeometry;
        QRectF box(QPointF(0, 0), layerSurface->size());

        bool hasLeft = layerSurface->anchors().testFlag(WaylandWlrLayerSurfaceV1::LeftAnchor);
        bool hasRight = layerSurface->anchors().testFlag(WaylandWlrLayerSurfaceV1::RightAnchor);
        bool hasTop = layerSurface->anchors().testFlag(WaylandWlrLayerSurfaceV1::TopAnchor);
        bool hasBottom = layerSurface->anchors().testFlag(WaylandWlrLayerSurfaceV1::BottomAnchor);

        // Horizontal axis
        if (hasLeft && hasRight && box.width() == 0) {
            box.setLeft(bounds.left());
            box.setWidth(bounds.width());
        } else if (hasLeft) {
            box.moveLeft(bounds.left());
        } else if (hasRight) {
            box.moveRight(bounds.right());
        } else {
            box.moveLeft(bounds.left() + (bounds.width() - box.width()) / 2);
        }

        // Vertical axis
        if (hasTop && hasBottom && box.height() == 0) {
            box.setTop(bounds.top());
            box.setHeight(bounds.height());
        } else if (hasTop) {
            box.moveTop(bounds.top());
        } else if (hasBottom) {
            box.moveBottom(bounds.bottom());
        } else {
            box.moveTop(bounds.top() + (bounds.height() - box.height()) / 2);
        }

        // Horizontal margin
        if (hasLeft && hasRight)
            box.adjust(layerSurface->leftMargin(), 0, -layerSurface->rightMargin(), 0);
        else if (hasLeft)
            box.translate(layerSurface->leftMargin(), 0);
        else if (hasRight)
            box.translate(-layerSurface->rightMargin(), 0);

        // Vertical margin
        if (hasTop && hasBottom)
            box.adjust(0, layerSurface->topMargin(), 0, -layerSurface->bottomMargin());
        else if (hasTop)
            box.translate(0, layerSurface->topMargin());
        else if (hasBottom)
            box.translate(0, -layerSurface->bottomMargin());

        // Set geometry
        if (box.isValid()) {
            item->setPosition(box.topLeft());
            applyExclusive(layerSurface, availableGeometry);
            layerSurface->sendConfigure(box.size().toSize());
            qCDebug(lcLayerShell) << "Layer surface" << layerSurface->nameSpace() << "geometry" << box;
        } else {
            qCWarning(lcLayerShell) << "Closing layer surface" << layerSurface->nameSpace() << "due to invalid geometry" << box;
            layerSurface->close();
        }
    }
}

void WaylandSurfaceLayoutPrivate::layoutItems()
{
    Q_Q(WaylandSurfaceLayout);

    QMutexLocker locker(&mutex);

    QRectF availableGeometry = q->boundingRect();

    // Arrange exclusive layer surfaces from top to bottom
    arrangeLayer(WaylandWlrLayerShellV1::OverlayLayer, true, &availableGeometry);
    arrangeLayer(WaylandWlrLayerShellV1::TopLayer, true, &availableGeometry);
    arrangeLayer(WaylandWlrLayerShellV1::BottomLayer, true, &availableGeometry);
    arrangeLayer(WaylandWlrLayerShellV1::BackgroundLayer, true, &availableGeometry);

    // Set available geometry
    if (output) {
        output->setAvailableGeometry(availableGeometry.toRect());
        qCDebug(lcLayerShell) << "Set output" << output->model() << "available geometry to" << availableGeometry;
    }

    // Arrange non-exclusive layer surfaces from top to bottom
    arrangeLayer(WaylandWlrLayerShellV1::OverlayLayer, false, &availableGeometry);
    arrangeLayer(WaylandWlrLayerShellV1::TopLayer, false, &availableGeometry);
    arrangeLayer(WaylandWlrLayerShellV1::BottomLayer, false, &availableGeometry);
    arrangeLayer(WaylandWlrLayerShellV1::BackgroundLayer, false, &availableGeometry);

    // Stack items
    auto items = q->childItems();
    std::list<QQuickItem *> sortedItems(items.begin(), items.end());
    sortedItems.sort([q](const QQuickItem *l, const QQuickItem *r) {
        return q->sortItems(const_cast<QQuickItem *>(l), const_cast<QQuickItem *>(r));
    });
    qreal zIndex = 0;
    for (auto *item : sortedItems)
        item->setZ(zIndex++);

    // Find the topmost keyboard interactive surface layer
    WaylandWlrLayerSurfaceV1Item *topmostItem = nullptr;
    std::vector<WaylandWlrLayerShellV1::Layer> layers = {
        WaylandWlrLayerShellV1::OverlayLayer,
        WaylandWlrLayerShellV1::TopLayer
    };
    sortedItems.reverse();
    for (auto layer : qAsConst(layers)) {
        for (auto *item : sortedItems) {
            auto *l = qobject_cast<WaylandWlrLayerSurfaceV1Item *>(item);
            if (!l)
                continue;
            if (l->layerSurface()->layer() != layer)
                continue;

            if (l->layerSurface()->keyboardInteractivity() && l->layerSurface()->isMapped()) {
                topmostItem = l;
                break;
            }
        }
    }
    if (topmostItem)
        topmostItem->takeFocus();
}

/*
 * WaylandWlrLayerShellLayout
 */

WaylandSurfaceLayout::WaylandSurfaceLayout(QQuickItem *parent)
    : QQuickItem(parent)
    , d_ptr(new WaylandSurfaceLayoutPrivate(this))
{
}

WaylandSurfaceLayout::~WaylandSurfaceLayout()
{
    delete d_ptr;
}

QWaylandOutput *WaylandSurfaceLayout::output() const
{
    Q_D(const WaylandSurfaceLayout);
    return d->output;
}

void WaylandSurfaceLayout::setOutput(QWaylandOutput *output)
{
    Q_D(WaylandSurfaceLayout);

    if (d->output == output)
        return;

    d->output = output;
    emit outputChanged(output);
}

void WaylandSurfaceLayout::itemChange(QQuickItem::ItemChange change,
                                      const QQuickItem::ItemChangeData &data)
{
    Q_UNUSED(data)

    if (change == ItemChildAddedChange || change == ItemChildRemovedChange)
        polish();
}

void WaylandSurfaceLayout::geometryChanged(const QRectF &newGeometry,
                                           const QRectF &oldGeometry)
{
    Q_UNUSED(newGeometry)
    Q_UNUSED(oldGeometry)

    polish();
}

void WaylandSurfaceLayout::updatePolish()
{
    Q_D(WaylandSurfaceLayout);
    d->layoutItems();
}

void WaylandSurfaceLayout::componentComplete()
{
    Q_D(WaylandSurfaceLayout);

    // Find output automatically
    if (!d->output) {
        for (auto *p = parent(); p != nullptr; p = p->parent()) {
            if (auto *o = qobject_cast<QWaylandOutput *>(p)) {
                setOutput(o);
                break;
            }
        }
    }

    QQuickItem::componentComplete();
}

WaylandSurfaceLayout::Layer WaylandSurfaceLayout::getLayer(QQuickItem *item) const
{
    static const QHash<WaylandWlrLayerShellV1::Layer, WaylandSurfaceLayout::Layer> layerToIndex {
        { WaylandWlrLayerShellV1::BackgroundLayer, WaylandSurfaceLayout::BackgroundLayer },
        { WaylandWlrLayerShellV1::BottomLayer, WaylandSurfaceLayout::BottomLayer },
        { WaylandWlrLayerShellV1::TopLayer, WaylandSurfaceLayout::TopLayer },
        { WaylandWlrLayerShellV1::OverlayLayer, WaylandSurfaceLayout::OverlayLayer },
    };

    if (auto *l = qobject_cast<WaylandWlrLayerSurfaceV1Item *>(item))
        return layerToIndex.value(l->layerSurface()->layer(), WaylandSurfaceLayout::NoLayer);
    return WaylandSurfaceLayout::NoLayer;
}

bool WaylandSurfaceLayout::sortItems(QQuickItem *left, QQuickItem *right)
{
    return getLayer(left) < getLayer(right);
}
