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

#include <QWaylandOutput>
#include <QWaylandSurface>

#include "testcompositor.h"

#include <wayland-server-core.h>

TestCompositor::TestCompositor()
    : QWaylandCompositor()
{
    setSocketName("wayland-liri-test-0");
}

void TestCompositor::create()
{
    auto *output = new QWaylandOutput(this, nullptr);
    setDefaultOutput(output);

    QWaylandCompositor::create();

    connect(this, &QWaylandCompositor::surfaceCreated,
            this, &TestCompositor::handleSurfaceCreated);
    connect(this, &QWaylandCompositor::surfaceAboutToBeDestroyed,
            this, &TestCompositor::handleSurfaceAboutToBeDestroyed);
}

void TestCompositor::flushClients()
{
    wl_display_flush_clients(display());
}

void TestCompositor::handleSurfaceCreated(QWaylandSurface *surface)
{
    surfaces.append(surface);
}

void TestCompositor::handleSurfaceAboutToBeDestroyed(QWaylandSurface *surface)
{
    surfaces.removeOne(surface);
}
