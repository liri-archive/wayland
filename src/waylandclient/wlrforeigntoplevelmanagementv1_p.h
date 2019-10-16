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

#ifndef LIRI_WLRFOREIGNTOPLEVELMANAGEMENTV1_P_CLIENT_H
#define LIRI_WLRFOREIGNTOPLEVELMANAGEMENTV1_P_CLIENT_H

#include <LiriWaylandClient/WlrForeignToplevelManagementV1>
#include <LiriWaylandClient/private/qwayland-wlr-foreign-toplevel-management-unstable-v1.h>

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

class LIRIWAYLANDCLIENT_EXPORT WlrForeignToplevelManagerV1Private : public QtWayland::zwlr_foreign_toplevel_manager_v1
{
    Q_DECLARE_PUBLIC(WlrForeignToplevelManagerV1)
public:
    explicit WlrForeignToplevelManagerV1Private(WlrForeignToplevelManagerV1 *self);

protected:
    WlrForeignToplevelManagerV1 *q_ptr;

    void zwlr_foreign_toplevel_manager_v1_toplevel(struct ::zwlr_foreign_toplevel_handle_v1 *toplevel) override;
    void zwlr_foreign_toplevel_manager_v1_finished() override;
};

class LIRIWAYLANDCLIENT_EXPORT WlrForeignToplevelHandleV1Private : public QtWayland::zwlr_foreign_toplevel_handle_v1
{
    Q_DECLARE_PUBLIC(WlrForeignToplevelHandleV1)
public:
    struct HandleInfo {
        QString title;
        QString appId;
        bool minimized = false;
        bool maximized = false;
        bool fullscreen = false;
        bool activated = false;
    };

    explicit WlrForeignToplevelHandleV1Private(WlrForeignToplevelHandleV1 *self);

    HandleInfo pending;
    HandleInfo effective;

protected:
    WlrForeignToplevelHandleV1 *q_ptr;

    void zwlr_foreign_toplevel_handle_v1_title(const QString &title) override;
    void zwlr_foreign_toplevel_handle_v1_app_id(const QString &appId) override;
    void zwlr_foreign_toplevel_handle_v1_output_enter(struct ::wl_output *output) override;
    void zwlr_foreign_toplevel_handle_v1_output_leave(struct ::wl_output *output) override;
    void zwlr_foreign_toplevel_handle_v1_state(wl_array *rawData) override;
    void zwlr_foreign_toplevel_handle_v1_done() override;
    void zwlr_foreign_toplevel_handle_v1_closed() override;
};

#endif // LIRI_WLRFOREIGNTOPLEVELMANAGEMENTV1_P_CLIENT_H
