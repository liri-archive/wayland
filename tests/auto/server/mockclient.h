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

#ifndef MOCKCLIENT_H
#define MOCKCLIENT_H

#include <QObject>

#include "mockregistry.h"

class MockClient : public QObject
{
    Q_OBJECT
public:
    MockClient(QObject *parent = nullptr);
    ~MockClient() override;

    wl_display *display = nullptr;
    MockRegistry *registry = nullptr;

    int error = 0; // means no error
    struct {
        uint32_t id = 0;
        uint32_t code = 0;
        const wl_interface *interface = nullptr;
    } protocolError;

private slots:
    void readEvents();
    void flushDisplay();
};

#endif // MOCKCLIENT_H
