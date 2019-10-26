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

#ifndef LIRI_WAYLANDWLRLAYERSHELLV1_P_H
#define LIRI_WAYLANDWLRLAYERSHELLV1_P_H

#include <QWaylandSurfaceRole>

#include <LiriWaylandServer/WaylandWlrLayerShellV1>
#include <LiriWaylandServer/private/qwayland-server-wlr-layer-shell-unstable-v1.h>

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

class WaylandWlrLayerShellV1Private : public QtWaylandServer::zwlr_layer_shell_v1
{
    Q_DECLARE_PUBLIC(WaylandWlrLayerShellV1)
public:
    explicit WaylandWlrLayerShellV1Private(WaylandWlrLayerShellV1 *self);

    static WaylandWlrLayerShellV1Private *get(WaylandWlrLayerShellV1 *shell) { return shell->d_func(); }

    QMap<QWaylandSurface *, WaylandWlrLayerSurfaceV1 *> surfaces;

protected:
    WaylandWlrLayerShellV1 *q_ptr;

    void zwlr_layer_shell_v1_get_layer_surface(Resource *resource, uint32_t id,
                                               struct ::wl_resource *surfaceRes,
                                               struct ::wl_resource *outputRes,
                                               uint32_t layer,
                                               const QString &nameSpace) override;
};

class WaylandWlrLayerSurfaceV1Private : public QtWaylandServer::zwlr_layer_surface_v1
{
    Q_DECLARE_PUBLIC(WaylandWlrLayerSurfaceV1)
public:
    struct State {
        State() = default;

        WaylandWlrLayerShellV1::Layer layer = WaylandWlrLayerShellV1::BackgroundLayer;
        QSize desiredSize = QSize(0, 0);
        WaylandWlrLayerSurfaceV1::Anchors anchors = 0;
        int exclusiveZone = 0;
        QMargins margins;
        bool keyboardInteractivity = true;
    };

    struct ConfigureEvent {
        ConfigureEvent() = default;
        ConfigureEvent(const QSize &incomingSize, quint32 incomingSerial)
            : size(incomingSize)
            , serial(incomingSerial)
        { }

        quint32 serial = 0;
        QSize size = QSize(0, 0);
    };

    explicit WaylandWlrLayerSurfaceV1Private(WaylandWlrLayerSurfaceV1 *self);

    ConfigureEvent lastSentConfigure() const;

    void unmap();

    static WaylandWlrLayerSurfaceV1Private *get(WaylandWlrLayerSurfaceV1 *surface) { return surface->d_func(); }

    QWaylandSurface *surface = nullptr;
    QWaylandOutput *output = nullptr;
    QString nameSpace;

    bool added = false;
    bool configured = false;
    bool mapped = false;
    bool closed = false;

    State clientPending, serverPending, current;

    QVector<ConfigureEvent> pendingConfigures;
    ConfigureEvent lastAckedConfigure;

    static QWaylandSurfaceRole s_role;

protected:
    WaylandWlrLayerSurfaceV1 *q_ptr;

    void zwlr_layer_surface_v1_set_size(Resource *resource, uint32_t width,
                                        uint32_t height) override;
    void zwlr_layer_surface_v1_set_anchor(Resource *resource,
                                          uint32_t anchor) override;
    void zwlr_layer_surface_v1_set_exclusive_zone(Resource *resource, int32_t zone) override;
    void zwlr_layer_surface_v1_set_margin(Resource *resource, int32_t top, int32_t right, int32_t bottom, int32_t left) override;
    void zwlr_layer_surface_v1_set_keyboard_interactivity(Resource *resource, uint32_t keyboard_interactivity) override;
    void zwlr_layer_surface_v1_get_popup(Resource *resource, struct ::wl_resource *popup) override;
    void zwlr_layer_surface_v1_ack_configure(Resource *resource, uint32_t serial) override;
    void zwlr_layer_surface_v1_set_layer(Resource *resource, uint32_t layer) override;
};

#endif // LIRI_WAYLANDWLRLAYERSHELLV1_P_H
