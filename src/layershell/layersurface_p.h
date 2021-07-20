// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_LAYERSURFACEINTERFACE_P_H
#define LIRI_LAYERSURFACEINTERFACE_P_H

#include <QLoggingCategory>

#include <LiriWaylandClientLayerShell/LayerSurface>

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

Q_DECLARE_LOGGING_CATEGORY(lcLayerShell)

namespace Liri {

namespace WaylandClient {

class LayerSurfacePrivate
{
public:
    LayerSurfacePrivate() = default;

    bool initialized = false;
    bool setLayerEnabled = false;
    QWindow *window = nullptr;
    LayerSurface::Layer layer = LayerSurface::TopLayer;
    QString role = QStringLiteral("unknown");
    LayerSurface::Anchors anchors;
    qint32 exclusiveZone = 0;
    QMargins margins;
    LayerSurface::KeyboardInteractivity keyboardInteractivity = LayerSurface::ExclusiveKeyboardInteractivity;
};

} // namespace WaylandClient

} // namespace Liri

#endif // LIRI_LAYERSURFACEINTERFACE_P_H
