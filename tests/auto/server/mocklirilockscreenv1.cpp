// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mocklirilockscreenv1.h"

MockLiriLockScreenHandleV1::MockLiriLockScreenHandleV1(struct ::zliri_lockscreen_handle_v1 *object)
    : QObject()
    , QtWayland::zliri_lockscreen_handle_v1(object)
{
}

void MockLiriLockScreenHandleV1::zliri_lockscreen_handle_v1_locked()
{
    Q_EMIT lockRequested();
}

MockLiriLockScreenV1::MockLiriLockScreenV1(struct ::wl_registry *registry, uint32_t name, uint32_t version)
    : QtWayland::zliri_lockscreen_v1(registry, name, version)
{
}

MockLiriLockScreenHandleV1 *MockLiriLockScreenV1::createHandle()
{
    return new MockLiriLockScreenHandleV1(get_handle());
}

QString MockLiriLockScreenV1::interfaceName()
{
    return QString::fromLocal8Bit(QtWayland::zliri_lockscreen_v1::interface()->name);
}
