// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MOCKLIRILOCKSCREENV1_H
#define MOCKLIRILOCKSCREENV1_H

#include <QObject>

#include "qwayland-liri-lockscreen-unstable-v1.h"

class MockLiriLockScreenHandleV1
        : public QObject
        , public QtWayland::zliri_lockscreen_handle_v1
{
    Q_OBJECT
public:
    MockLiriLockScreenHandleV1(struct ::zliri_lockscreen_handle_v1 *object);

Q_SIGNALS:
    void lockRequested();

protected:
    void zliri_lockscreen_handle_v1_locked() override;
};

class MockLiriLockScreenV1 : public QtWayland::zliri_lockscreen_v1
{
public:
    MockLiriLockScreenV1(struct ::wl_registry *registry, uint32_t name, uint32_t version);

    MockLiriLockScreenHandleV1 *createHandle();

    static QString interfaceName();
};

#endif // MOCKLIRILOCKSCREENV1_H
