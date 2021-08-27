// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QWaylandCompositor>

#include "waylandlirilockscreenv1_p.h"
#include "waylandlirilockscreenv1.h"

Q_LOGGING_CATEGORY(lcLockScreen, "liri.waylandserver.lockscreen", QtInfoMsg)

/*
 * WaylandLiriScreenLockerHandleV1
 */

void WaylandLiriScreenLockerHandleV1::zliri_lockscreen_handle_v1_destroy_resource(Resource *resource)
{
    Q_UNUSED(resource)

    WaylandLiriLockScreenV1Private::get(manager)->handle.reset();
    emit manager->clientAvailableChanged(false);
    delete this;
}

void WaylandLiriScreenLockerHandleV1::zliri_lockscreen_handle_v1_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void WaylandLiriScreenLockerHandleV1::zliri_lockscreen_handle_v1_unlock(Resource *resource)
{
    Q_UNUSED(resource)

    Q_EMIT manager->unlocked();
}

/*
 * WaylandLiriLockScreenV1Private
 */

WaylandLiriLockScreenV1Private::WaylandLiriLockScreenV1Private(WaylandLiriLockScreenV1 *self)
    : q_ptr(self)
{
}

void WaylandLiriLockScreenV1Private::zliri_lockscreen_v1_bind_resource(Resource *resource)
{
    Q_UNUSED(resource)

    // TODO: Only whitelisted clients are allowed
}

void WaylandLiriLockScreenV1Private::zliri_lockscreen_v1_destroy_resource(Resource *resource)
{
    Q_UNUSED(resource)
    delete this;
}

void WaylandLiriLockScreenV1Private::zliri_lockscreen_v1_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void WaylandLiriLockScreenV1Private::zliri_lockscreen_v1_get_handle(Resource *resource, uint32_t id)
{
    Q_Q(WaylandLiriLockScreenV1);

    if (handle) {
        // Only one client is allowed to bind to this interface
        qCWarning(lcLockScreen, "Only one client is allowed to bind");
        wl_resource_post_error(resource->handle, error_already_bound,
                               "already bound");
        return;
    }

    handle.reset(new WaylandLiriScreenLockerHandleV1());
    handle->manager = q;
    handle->init(resource->client(), id, resource->version());

    emit q->clientAvailableChanged(true);
}

/*
 * WaylandLiriLockScreenV1
 */

WaylandLiriLockScreenV1::WaylandLiriLockScreenV1()
    : QWaylandCompositorExtensionTemplate<WaylandLiriLockScreenV1>()
    , d_ptr(new WaylandLiriLockScreenV1Private(this))
{
}

WaylandLiriLockScreenV1::WaylandLiriLockScreenV1(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<WaylandLiriLockScreenV1>(compositor)
    , d_ptr(new WaylandLiriLockScreenV1Private(this))
{
}

WaylandLiriLockScreenV1::~WaylandLiriLockScreenV1()
{
    delete d_ptr;
}

void WaylandLiriLockScreenV1::initialize()
{
    Q_D(WaylandLiriLockScreenV1);

    QWaylandCompositorExtensionTemplate::initialize();
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qCWarning(lcLockScreen) << "Failed to find QWaylandCompositor when initializing WaylandLiriLockScreenV1";
        return;
    }
    d->init(compositor->display(), WaylandLiriLockScreenV1Private::interfaceVersion());
}

bool WaylandLiriLockScreenV1::isClientAvailable() const
{
    Q_D(const WaylandLiriLockScreenV1);
    return !d->handle.isNull();
}

void WaylandLiriLockScreenV1::requestLock()
{
    Q_D(WaylandLiriLockScreenV1);

    if (d->handle)
        d->handle->send_locked();
    else
        qCWarning(lcLockScreen, "Lock screen will not be visible because there is no client bound yet");
}

const wl_interface *WaylandLiriLockScreenV1::interface()
{
    return WaylandLiriLockScreenV1Private::interface();
}

QByteArray WaylandLiriLockScreenV1::interfaceName()
{
    return WaylandLiriLockScreenV1Private::interfaceName();
}
