// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "lirilockscreenv1.h"
#include "lirilockscreenv1_p.h"

/*
 * LiriLockScreenV1Handle
 */

LiriLockScreenV1Handle::~LiriLockScreenV1Handle()
{
    destroy();
}

void LiriLockScreenV1Handle::zliri_lockscreen_handle_v1_locked()
{
    Q_EMIT manager->lockRequested();
}

/*
 * LiriLockScreenV1Private
 */

LiriLockScreenV1Private::LiriLockScreenV1Private(LiriLockScreenV1 *self)
    : handle(new LiriLockScreenV1Handle())
    , q_ptr(self)
{
    handle->manager = self;
}

LiriLockScreenV1Private::~LiriLockScreenV1Private()
{
    if (handle) {
        delete handle;
        handle = nullptr;
    }

    destroy();
}

/*
 * LiriLockScreenV1
 */

LiriLockScreenV1::LiriLockScreenV1()
    : QWaylandClientExtensionTemplate(1)
    , d_ptr(new LiriLockScreenV1Private(this))
{
}

LiriLockScreenV1::~LiriLockScreenV1()
{
    delete d_ptr;
}

void LiriLockScreenV1::unlock()
{
    Q_D(LiriLockScreenV1);
    d->handle->unlock();
}

void LiriLockScreenV1::init(struct ::wl_registry *registry, int id, int version)
{
    Q_D(LiriLockScreenV1);

    d->init(registry, id, version);
    d->handle->init(d->get_handle());
}

const struct ::wl_interface *LiriLockScreenV1::interface()
{
    return LiriLockScreenV1Private::interface();
}
