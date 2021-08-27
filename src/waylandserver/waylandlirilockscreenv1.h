// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_WAYLANDLIRILOCKSCREENV1_H
#define LIRI_WAYLANDLIRILOCKSCREENV1_H

#include <QWaylandCompositorExtension>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

class WaylandLiriLockScreenV1Private;

class LIRIWAYLANDSERVER_EXPORT WaylandLiriLockScreenV1
        : public QWaylandCompositorExtensionTemplate<WaylandLiriLockScreenV1>
{
    Q_OBJECT
    Q_PROPERTY(bool clientAvailable READ isClientAvailable NOTIFY clientAvailableChanged)
    Q_DECLARE_PRIVATE(WaylandLiriLockScreenV1)
public:
    WaylandLiriLockScreenV1();
    WaylandLiriLockScreenV1(QWaylandCompositor *compositor);
    ~WaylandLiriLockScreenV1();

    void initialize() override;

    bool isClientAvailable() const;

    Q_INVOKABLE void requestLock();

    static const wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void clientAvailableChanged(bool available);
    void unlocked();

private:
    WaylandLiriLockScreenV1Private *const d_ptr = nullptr;
};

#endif // LIRI_WAYLANDLIRILOCKSCREENV1_H
