// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_WAYLANDWLRLAYERSURFACEV1ITEM_H
#define LIRI_WAYLANDWLRLAYERSURFACEV1ITEM_H

#include <QWaylandQuickItem>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

class WaylandWlrLayerSurfaceV1;
class WaylandWlrLayerSurfaceV1ItemPrivate;

class LIRIWAYLANDSERVER_EXPORT WaylandWlrLayerSurfaceV1Item : public QWaylandQuickItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrLayerSurfaceV1Item)
    Q_PROPERTY(WaylandWlrLayerSurfaceV1 *layerSurface READ layerSurface WRITE setLayerSurface NOTIFY layerSurfaceChanged)
    QML_ELEMENT
public:
    WaylandWlrLayerSurfaceV1Item(QQuickItem *parent = nullptr);
    ~WaylandWlrLayerSurfaceV1Item();

    WaylandWlrLayerSurfaceV1 *layerSurface() const;
    void setLayerSurface(WaylandWlrLayerSurfaceV1 *layerSurface);

Q_SIGNALS:
    void layerSurfaceChanged();

private:
    WaylandWlrLayerSurfaceV1ItemPrivate *const d_ptr = nullptr;

    Q_PRIVATE_SLOT(d_func(), void _q_configure());
};

#endif // LIRI_WAYLANDWLRLAYERSURFACEV1ITEM_H
