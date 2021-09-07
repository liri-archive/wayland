// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_WAYLANDSURFACELAYOUT_P_H
#define LIRI_WAYLANDSURFACELAYOUT_P_H

#include <QMutexLocker>

#include <LiriWaylandServer/WaylandWlrLayerShellV1>
#include <LiriWaylandServer/WaylandSurfaceLayout>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class LIRIWAYLANDSERVER_EXPORT WaylandSurfaceLayoutPrivate
{
    Q_DECLARE_PUBLIC(WaylandSurfaceLayout)
public:
    WaylandSurfaceLayoutPrivate(WaylandSurfaceLayout *self);

    void applyExclusive(WaylandWlrLayerSurfaceV1 *layerSurface, QRectF *availableGeometry);
    void arrangeLayer(WaylandWlrLayerShellV1::Layer layer,
                      bool exclusive, QRectF *availableGeometry);
    void layoutItems();

    QMutex mutex;
    QWaylandOutput *output = nullptr;

protected:
    WaylandSurfaceLayout *q_ptr = nullptr;
};

#endif // LIRI_WAYLANDSURFACELAYOUT_P_H
