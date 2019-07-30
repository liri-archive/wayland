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

#include <QAbstractEventDispatcher>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QSocketNotifier>

#include <private/qguiapplication_p.h>

#include "mockclient.h"

MockClient::MockClient(QObject *parent)
    : QObject(parent)
    , display(wl_display_connect("wayland-liri-test-0"))
{
    if (!display)
        qFatal("MockClient: Unable to connnect to the compositor");

    registry = new MockRegistry(wl_display_get_registry(display));

    int fd = wl_display_get_fd(display);

    QSocketNotifier *readNotifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    connect(readNotifier, &QSocketNotifier::activated, this, &MockClient::readEvents);

    QAbstractEventDispatcher *dispatcher = QGuiApplicationPrivate::eventDispatcher;
    connect(dispatcher, &QAbstractEventDispatcher::awake, this, &MockClient::flushDisplay);

    QElapsedTimer timeout;
    timeout.start();
    do {
        QCoreApplication::processEvents();
    } while (!(registry->compositor && !registry->outputs.isEmpty()) && timeout.elapsed() < 1000);

    if (!registry->compositor || registry->outputs.isEmpty())
        qFatal("MockClient: failed to receive globals from display");
}

MockClient::~MockClient()
{
}

void MockClient::readEvents()
{
    if (error)
        return;
    wl_display_dispatch(display);
}

void MockClient::flushDisplay()
{
    if (error)
        return;

    if (wl_display_prepare_read(display) == 0)
        wl_display_read_events(display);

    if (wl_display_dispatch_pending(display) < 0) {
        error = wl_display_get_error(display);
        if (error == EPROTO) {
            protocolError.code = wl_display_get_protocol_error(display, &protocolError.interface, &protocolError.id);
            return;
        }
    }

    wl_display_flush(display);
}
