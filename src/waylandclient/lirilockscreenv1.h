// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_LIRILOCKSCREENV1_H
#define LIRI_LIRILOCKSCREENV1_H

#include <QWaylandClientExtension>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

class LiriLockScreenV1Private;

class LIRIWAYLANDCLIENT_EXPORT LiriLockScreenV1
        : public QWaylandClientExtensionTemplate<LiriLockScreenV1>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LiriLockScreenV1)
public:
    LiriLockScreenV1();
    ~LiriLockScreenV1();

    Q_INVOKABLE void unlock();

    void init(struct ::wl_registry *registry, int id, int version);

    static const struct ::wl_interface *interface();

Q_SIGNALS:
    void lockRequested();

private:
    LiriLockScreenV1Private *const d_ptr = nullptr;
};

#endif // LIRI_LIRILOCKSCREENV1_H
