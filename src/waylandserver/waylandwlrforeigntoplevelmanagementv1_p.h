/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2019 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef LIRI_WAYLANDWLRFOREIGNTOPLEVELMANAGEMENTV1_P_H
#define LIRI_WAYLANDWLRFOREIGNTOPLEVELMANAGEMENTV1_P_H

#include <QWaylandClient>
#include <QWaylandCompositor>
#include <QWaylandOutput>
#include <QWaylandSeat>
#include <QWaylandSurface>

#include <LiriWaylandServer/WaylandWlrForeignToplevelManagerV1>
#include <LiriWaylandServer/private/qwayland-server-wlr-foreign-toplevel-management-unstable-v1.h>

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

class LIRIWAYLANDSERVER_EXPORT WaylandWlrForeignToplevelManagerV1Private : public QtWaylandServer::zwlr_foreign_toplevel_manager_v1
{
    Q_DECLARE_PUBLIC(WaylandWlrForeignToplevelManagerV1)
public:
    explicit WaylandWlrForeignToplevelManagerV1Private(WaylandWlrForeignToplevelManagerV1 *self);

    static WaylandWlrForeignToplevelManagerV1Private *get(WaylandWlrForeignToplevelManagerV1 *self) { return self->d_func(); }

    QWaylandCompositor *compositor = nullptr;
    QVector<WaylandWlrForeignToplevelHandleV1 *> toplevels;
    QVector<wl_client *> stoppedClients;

protected:
    WaylandWlrForeignToplevelManagerV1 *q_ptr;

    void zwlr_foreign_toplevel_manager_v1_bind_resource(Resource *resource) override;
    void zwlr_foreign_toplevel_manager_v1_stop(Resource *resource) override;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrForeignToplevelHandleV1Private : public QtWaylandServer::zwlr_foreign_toplevel_handle_v1
{
    Q_DECLARE_PUBLIC(WaylandWlrForeignToplevelHandleV1)
public:
    explicit WaylandWlrForeignToplevelHandleV1Private(WaylandWlrForeignToplevelHandleV1 *self);

    static WaylandWlrForeignToplevelHandleV1Private *get(WaylandWlrForeignToplevelHandleV1 *self) { return self->d_func(); }

    bool initialized = false;
    QWaylandCompositor *compositor = nullptr;
    WaylandWlrForeignToplevelManagerV1 *manager = nullptr;
    QVector<state> states;
    QString title, appId;
    QWaylandOutput *output = nullptr;
    bool enteredOutput = false;
    QWaylandSurface *rectSurface = nullptr;
    QRect rect;
    WaylandWlrForeignToplevelHandleV1 *parentHandle = nullptr;

protected:
    WaylandWlrForeignToplevelHandleV1 *q_ptr;

    void zwlr_foreign_toplevel_handle_v1_set_maximized(Resource *resource) override;
    void zwlr_foreign_toplevel_handle_v1_unset_maximized(Resource *resource) override;
    void zwlr_foreign_toplevel_handle_v1_set_minimized(Resource *resource) override;
    void zwlr_foreign_toplevel_handle_v1_unset_minimized(Resource *resource) override;
    void zwlr_foreign_toplevel_handle_v1_activate(Resource *resource,
                                                  struct ::wl_resource *seatResource) override;
    void zwlr_foreign_toplevel_handle_v1_close(Resource *resource) override;
    void zwlr_foreign_toplevel_handle_v1_set_rectangle(Resource *resource,
                                                       struct ::wl_resource *surfaceResource,
                                                       int32_t x, int32_t y, int32_t width, int32_t height) override;
    void zwlr_foreign_toplevel_handle_v1_destroy(Resource *resource) override;
    void zwlr_foreign_toplevel_handle_v1_set_fullscreen(Resource *resource,
                                                        struct ::wl_resource *outputResource) override;
    void zwlr_foreign_toplevel_handle_v1_unset_fullscreen(Resource *resource) override;
};

#endif // LIRI_WAYLANDWLRFOREIGNTOPLEVELMANAGEMENTV1_P_H
