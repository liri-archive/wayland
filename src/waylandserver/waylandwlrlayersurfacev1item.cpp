// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QWaylandCompositor>
#include <QWaylandOutput>

#include "waylandsurfacelayout.h"
#include "waylandwlrlayershellv1.h"
#include "waylandwlrlayersurfacev1item_p.h"

/*
 * WaylandWlrLayerSurfaceV1ItemPrivate
 */

WaylandWlrLayerSurfaceV1ItemPrivate::WaylandWlrLayerSurfaceV1ItemPrivate(WaylandWlrLayerSurfaceV1Item *self)
    : q_ptr(self)
{
}

void WaylandWlrLayerSurfaceV1ItemPrivate::_q_configure()
{
    Q_Q(WaylandWlrLayerSurfaceV1Item);

    if (auto *layout = qobject_cast<WaylandSurfaceLayout *>(q->parentItem()))
        layout->update();
}

/*
 * WaylandWlrLayerSurfaceV1Item
 */

WaylandWlrLayerSurfaceV1Item::WaylandWlrLayerSurfaceV1Item(QQuickItem *parent)
    : QWaylandQuickItem(parent)
    , d_ptr(new WaylandWlrLayerSurfaceV1ItemPrivate(this))
{
}

WaylandWlrLayerSurfaceV1Item::~WaylandWlrLayerSurfaceV1Item()
{
    delete d_ptr;
}

WaylandWlrLayerSurfaceV1 *WaylandWlrLayerSurfaceV1Item::layerSurface() const
{
    Q_D(const WaylandWlrLayerSurfaceV1Item);
    return d->layerSurface;
}

void WaylandWlrLayerSurfaceV1Item::setLayerSurface(WaylandWlrLayerSurfaceV1 *layerSurface)
{
    Q_D(WaylandWlrLayerSurfaceV1Item);

    if (d->layerSurface == layerSurface)
        return;

    if (layerSurface) {
        if (d->layerSurface)
            disconnect(this, SLOT(_q_configure()));

        setSurface(layerSurface->surface());
        setOutput(layerSurface->output());
        setSizeFollowsSurface(true);
        setFocusOnClick(layerSurface->keyboardInteractivity() > WaylandWlrLayerSurfaceV1::NoKeyboardInteractivity);

        d->layerSurface = layerSurface;
        connect(d->layerSurface, SIGNAL(changed()), this, SLOT(_q_configure()));
        emit layerSurfaceChanged();
    } else {
        qCWarning(lcLayerShell, "Unable to set WaylandWlrLayerSurfaceV1Item::layerSurface to null");
    }
}

#include "moc_waylandwlrlayersurfacev1item.cpp"
