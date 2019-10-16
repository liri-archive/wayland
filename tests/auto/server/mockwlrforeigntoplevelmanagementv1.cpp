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

#include "mockwlrforeigntoplevelmanagementv1.h"

MockWlrForeignToplevelManagerV1::MockWlrForeignToplevelManagerV1(struct ::wl_registry *registry, uint32_t name, uint32_t version)
    : QtWayland::zwlr_foreign_toplevel_manager_v1(registry, name, version)
{
}

void MockWlrForeignToplevelManagerV1::zwlr_foreign_toplevel_manager_v1_toplevel(struct ::zwlr_foreign_toplevel_handle_v1 *toplevel)
{
    handles.append(new MockWlrForeignToplevelHandleV1(toplevel));
}

void MockWlrForeignToplevelManagerV1::zwlr_foreign_toplevel_manager_v1_finished()
{
    delete this;
}

MockWlrForeignToplevelHandleV1::MockWlrForeignToplevelHandleV1(struct ::zwlr_foreign_toplevel_handle_v1 *object)
    : QtWayland::zwlr_foreign_toplevel_handle_v1(object)
{
}

void MockWlrForeignToplevelHandleV1::zwlr_foreign_toplevel_handle_v1_title(const QString &title)
{
    if (closed)
        return;

    pending.title = title;
}

void MockWlrForeignToplevelHandleV1::zwlr_foreign_toplevel_handle_v1_app_id(const QString &app_id)
{
    if (closed)
        return;

    pending.appId = app_id;
}

void MockWlrForeignToplevelHandleV1::zwlr_foreign_toplevel_handle_v1_output_enter(struct ::wl_output *output)
{
    if (closed)
        return;

    lastOutputEntered = output;
}

void MockWlrForeignToplevelHandleV1::zwlr_foreign_toplevel_handle_v1_output_leave(struct ::wl_output *output)
{
    if (closed)
        return;

    lastOutputLeft = output;
}

void MockWlrForeignToplevelHandleV1::zwlr_foreign_toplevel_handle_v1_state(wl_array *rawData)
{
    if (closed)
        return;

    if (rawData) {
        pending.minimized = pending.maximized = pending.fullscreen = pending.activated = false;

        const auto rawStateCount = rawData->size / sizeof(uint32_t);
        uint32_t *s = static_cast<uint32_t *>(rawData->data);
        for (auto i = 0; i < rawStateCount; ++i) {
            uint32_t state = *s++;

            if (state == state_minimized)
                pending.minimized = true;
            if (state == state_maximized)
                pending.maximized = true;
            if (state == state_fullscreen)
                pending.fullscreen = true;
            if (state == state_activated)
                pending.activated = true;
        }
    }
}

void MockWlrForeignToplevelHandleV1::zwlr_foreign_toplevel_handle_v1_done()
{
    effective.title = pending.title;
    effective.appId = pending.appId;
    effective.minimized = pending.minimized;
    effective.maximized = pending.maximized;
    effective.fullscreen = pending.fullscreen;
    effective.activated = pending.activated;
}

void MockWlrForeignToplevelHandleV1::zwlr_foreign_toplevel_handle_v1_closed()
{
    closed = true;
}
