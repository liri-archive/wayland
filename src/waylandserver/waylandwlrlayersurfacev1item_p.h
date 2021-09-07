// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_WAYLANDWLRLAYERSURFACEV1ITEM_P_H
#define LIRI_WAYLANDWLRLAYERSURFACEV1ITEM_P_H

#include <QMutexLocker>

#include <LiriWaylandServer/WaylandWlrLayerSurfaceV1Item>

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

class WaylandWlrLayerSurfaceV1;

class LIRIWAYLANDSERVER_EXPORT WaylandWlrLayerSurfaceV1ItemPrivate
{
    Q_DECLARE_PUBLIC(WaylandWlrLayerSurfaceV1Item)
public:
    WaylandWlrLayerSurfaceV1ItemPrivate(WaylandWlrLayerSurfaceV1Item *self);

    void _q_configure();

    WaylandWlrLayerSurfaceV1 *layerSurface = nullptr;

protected:
    WaylandWlrLayerSurfaceV1Item *q_ptr = nullptr;
};

#endif // LIRI_WAYLANDWLRLAYERSURFACEV1ITEM_P_H
