/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2019 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef LIRI_WLRLAYERSHELLV1_P_CLIENT_H
#define LIRI_WLRLAYERSHELLV1_P_CLIENT_H

#include <LiriWaylandClient/WlrLayerShellV1>
#include <LiriWaylandClient/private/qwayland-xdg-shell.h>
#include <LiriWaylandClient/private/qwayland-wlr-layer-shell-unstable-v1.h>

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

class WlrLayerShellV1Private : public QtWayland::zwlr_layer_shell_v1
{
public:
    explicit WlrLayerShellV1Private() {}

    struct ::zwlr_layer_surface_v1 *createLayerSurface(
            QWindow *window, QScreen *screen,
            WlrLayerShellV1::Layer layer,
            const QString &nameSpace = QString());

    static WlrLayerShellV1Private *get(WlrLayerShellV1 *shell) { return shell->d_func(); }
};

class WlrLayerSurfaceV1Private : public QtWayland::zwlr_layer_surface_v1
{
    Q_DECLARE_PUBLIC(WlrLayerSurfaceV1)
public:
    explicit WlrLayerSurfaceV1Private(WlrLayerSurfaceV1 *self);

    bool initialized = false;
    WlrLayerShellV1 *shell = nullptr;
    WlrLayerShellV1::Layer layer = WlrLayerShellV1::Layer::BackgroundLayer;
    QString nameSpace;
    QWindow *window = nullptr;
    QSize size;
    WlrLayerSurfaceV1::Anchors anchors = 0;
    qint32 exclusiveZone = 0;
    QMargins margins;
    bool keyboardInteractivity = true;

protected:
    WlrLayerSurfaceV1 *q_ptr;

    void zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height) override;
    void zwlr_layer_surface_v1_closed() override;
};

#endif // LIRI_WLRLAYERSHELLV1_P_CLIENT_H
