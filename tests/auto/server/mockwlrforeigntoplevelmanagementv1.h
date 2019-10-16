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

#ifndef MOCKWLRFOREIGNTOPLEVELMANAGEMENTV1_H
#define MOCKWLRFOREIGNTOPLEVELMANAGEMENTV1_H

#include <QVector>

#include "qwayland-wlr-foreign-toplevel-management-unstable-v1.h"

class MockWlrForeignToplevelHandleV1;

class MockWlrForeignToplevelManagerV1 : public QtWayland::zwlr_foreign_toplevel_manager_v1
{
public:
    MockWlrForeignToplevelManagerV1(struct ::wl_registry *registry, uint32_t name, uint32_t version);

    QVector<MockWlrForeignToplevelHandleV1 *> handles;

protected:
    void zwlr_foreign_toplevel_manager_v1_toplevel(struct ::zwlr_foreign_toplevel_handle_v1 *toplevel) override;
    void zwlr_foreign_toplevel_manager_v1_finished() override;
};

class MockWlrForeignToplevelHandleV1 : public QtWayland::zwlr_foreign_toplevel_handle_v1
{
public:
    MockWlrForeignToplevelHandleV1(struct ::zwlr_foreign_toplevel_handle_v1 *object);

    bool closed = false;

    struct {
        QString title;
        QString appId;
        bool minimized = false;
        bool maximized = false;
        bool fullscreen = false;
        bool activated = false;
    } pending;

    struct {
        QString title;
        QString appId;
        bool minimized = false;
        bool maximized = false;
        bool fullscreen = false;
        bool activated = false;
    } effective;

    struct ::wl_output *lastOutputEntered = nullptr;
    struct ::wl_output *lastOutputLeft = nullptr;

protected:
    void zwlr_foreign_toplevel_handle_v1_title(const QString &title) override;
    void zwlr_foreign_toplevel_handle_v1_app_id(const QString &app_id) override;
    void zwlr_foreign_toplevel_handle_v1_output_enter(struct ::wl_output *output) override;
    void zwlr_foreign_toplevel_handle_v1_output_leave(struct ::wl_output *output) override;
    void zwlr_foreign_toplevel_handle_v1_state(wl_array *rawData) override;
    void zwlr_foreign_toplevel_handle_v1_done() override;
    void zwlr_foreign_toplevel_handle_v1_closed() override;
};

#endif // MOCKWLRFOREIGNTOPLEVELMANAGEMENTV1_H
