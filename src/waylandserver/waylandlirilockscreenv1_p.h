// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_WAYLANDLIRILOCKSCREENV1_P_H
#define LIRI_WAYLANDLIRILOCKSCREENV1_P_H

#include <QLoggingCategory>

#include <LiriWaylandServer/WaylandLiriLockScreenV1>
#include <LiriWaylandServer/private/qwayland-server-liri-lockscreen-unstable-v1.h>

Q_DECLARE_LOGGING_CATEGORY(lcLockScreen)

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

class LIRIWAYLANDSERVER_EXPORT WaylandLiriScreenLockerHandleV1
        : public QtWaylandServer::zliri_lockscreen_handle_v1
{
public:
    WaylandLiriScreenLockerHandleV1() = default;

    WaylandLiriLockScreenV1 *manager = nullptr;

protected:
    void zliri_lockscreen_handle_v1_destroy_resource(Resource *resource) override;
    void zliri_lockscreen_handle_v1_destroy(Resource *resource) override;
    void zliri_lockscreen_handle_v1_unlock(Resource *resource) override;
};

class LIRIWAYLANDSERVER_EXPORT WaylandLiriLockScreenV1Private
        : public QtWaylandServer::zliri_lockscreen_v1
{
    Q_DECLARE_PUBLIC(WaylandLiriLockScreenV1)
public:
    WaylandLiriLockScreenV1Private(WaylandLiriLockScreenV1 *self);

    static WaylandLiriLockScreenV1Private *get(WaylandLiriLockScreenV1 *self) { return self->d_func(); }

    QScopedPointer<WaylandLiriScreenLockerHandleV1> handle;

protected:
    WaylandLiriLockScreenV1 *q_ptr = nullptr;

    void zliri_lockscreen_v1_bind_resource(Resource *resource) override;
    void zliri_lockscreen_v1_destroy_resource(Resource *resource) override;
    void zliri_lockscreen_v1_destroy(Resource *resource) override;
    void zliri_lockscreen_v1_get_handle(Resource *resource, uint32_t id) override;
};

#endif // LIRI_WAYLANDLIRILOCKSCREENV1_P_H
