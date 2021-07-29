// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "wlrlayersurfacev1_p.h"
#include "logging_p.h"

namespace Liri {

namespace WaylandClient {

// Both shell integration and QML plugins will share this global
// since the interface is in a dynamic library
static QMap<QWindow *, WlrLayerSurfaceV1 *> layerSurfaces;

WlrLayerSurfaceV1::WlrLayerSurfaceV1(QObject *parent)
    : QObject(parent)
    , d_ptr(new WlrLayerSurfaceV1Private())
{
}

WlrLayerSurfaceV1::~WlrLayerSurfaceV1()
{
    Q_D(WlrLayerSurfaceV1);
    layerSurfaces.remove(d->window);
    delete d_ptr;
}

bool WlrLayerSurfaceV1::isInitialized() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->initialized;
}

QWindow *WlrLayerSurfaceV1::window() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->window;
}

void WlrLayerSurfaceV1::setWindow(QWindow *window)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->window == window)
        return;

    if (d->initialized) {
        qCWarning(lcLayerShell, "Unable to change WlrLayerSurfaceV1::window after initialization");
        return;
    }

    d->window = window;
    Q_EMIT windowChanged(d->window);

    if (!layerSurfaces.contains(d->window))
        layerSurfaces.insert(d->window, this);
}

Liri::WaylandClient::WlrLayerSurfaceV1::Layer WlrLayerSurfaceV1::layer() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->layer;
}

void WlrLayerSurfaceV1::setLayer(Liri::WaylandClient::WlrLayerSurfaceV1::Layer layer)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->layer == layer)
        return;

    if (d->initialized && !d->setLayerEnabled) {
        qCWarning(lcLayerShell, "Unable to change WlrLayerSurfaceV1::layer after initialization");
        return;
    }

    d->layer = layer;
    Q_EMIT layerChanged(d->layer);
}

QString WlrLayerSurfaceV1::role() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->role;
}

void WlrLayerSurfaceV1::setRole(const QString &role)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->role == role)
        return;

    if (d->initialized) {
        qCWarning(lcLayerShell, "Unable to change WlrLayerSurfaceV1::role after initialization");
        return;
    }

    d->role = role;
    Q_EMIT roleChanged(d->role);
}

Liri::WaylandClient::WlrLayerSurfaceV1::Anchors WlrLayerSurfaceV1::anchors() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->anchors;
}

void WlrLayerSurfaceV1::setAnchors(Liri::WaylandClient::WlrLayerSurfaceV1::Anchors anchors)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->anchors == anchors)
        return;

    d->anchors = anchors;
    Q_EMIT anchorsChanged(d->anchors);
}

qint32 WlrLayerSurfaceV1::exclusiveZone() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->exclusiveZone;
}

void WlrLayerSurfaceV1::setExclusiveZone(qint32 zone)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->exclusiveZone == zone)
        return;

    d->exclusiveZone = zone;
    Q_EMIT exclusiveZoneChanged(d->exclusiveZone);
}

QMargins WlrLayerSurfaceV1::margins() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->margins;
}

qint32 WlrLayerSurfaceV1::leftMargin() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->margins.left();
}

void WlrLayerSurfaceV1::setLeftMargin(qint32 margin)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->margins.left() == margin)
        return;

    d->margins.setLeft(margin);
    Q_EMIT leftMarginChanged(margin);
    Q_EMIT marginsChanged(d->margins);
}

qint32 WlrLayerSurfaceV1::rightMargin() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->margins.right();
}

void WlrLayerSurfaceV1::setRightMargin(qint32 margin)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->margins.right() == margin)
        return;

    d->margins.setRight(margin);
    Q_EMIT rightMarginChanged(margin);
    Q_EMIT marginsChanged(d->margins);
}

qint32 WlrLayerSurfaceV1::topMargin() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->margins.top();
}

void WlrLayerSurfaceV1::setTopMargin(qint32 margin)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->margins.top() == margin)
        return;

    d->margins.setTop(margin);
    Q_EMIT topMarginChanged(margin);
    Q_EMIT marginsChanged(d->margins);
}

qint32 WlrLayerSurfaceV1::bottomMargin() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->margins.bottom();
}

void WlrLayerSurfaceV1::setBottomMargin(qint32 margin)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->margins.bottom() == margin)
        return;

    d->margins.setBottom(margin);
    Q_EMIT bottomMarginChanged(margin);
    Q_EMIT marginsChanged(d->margins);
}

Liri::WaylandClient::WlrLayerSurfaceV1::KeyboardInteractivity WlrLayerSurfaceV1::keyboardInteractivity() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->keyboardInteractivity;
}

void WlrLayerSurfaceV1::setKeyboardInteractivity(Liri::WaylandClient::WlrLayerSurfaceV1::KeyboardInteractivity keyboardInteractivity)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->keyboardInteractivity == keyboardInteractivity)
        return;

    d->keyboardInteractivity = keyboardInteractivity;
    Q_EMIT keyboardInteractivityChanged(d->keyboardInteractivity);
}

void WlrLayerSurfaceV1::initialize()
{
    Q_D(WlrLayerSurfaceV1);

    if (d->initialized)
        return;

    if (!d->window) {
        qCWarning(lcLayerShell, "Window not assigned to WlrLayerSurfaceV1, failed to initialize");
        return;
    }

    d->initialized = true;
}

void WlrLayerSurfaceV1::setLayerEnabled(bool enabled)
{
    Q_D(WlrLayerSurfaceV1);
    d->setLayerEnabled = enabled;
}

WlrLayerSurfaceV1 *WlrLayerSurfaceV1::get(QWindow *window)
{
    return layerSurfaces.value(window, nullptr);
}

} // namespace WaylandClient

} // namespace Liri
