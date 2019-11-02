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

#ifndef MOCKREGISTRY_H
#define MOCKREGISTRY_H

#include <QMap>

#include "qwayland-wayland.h"

#include "mockwlrforeigntoplevelmanagementv1.h"
#include "mockwlrlayershellv1.h"
#include "mockwlroutputmanagementv1.h"
#include "mockwlrscreencopyv1.h"

class MockRegistry : public QtWayland::wl_registry
{
public:
    MockRegistry(struct ::wl_registry *object);

    QtWayland::wl_surface *createSurface();

    QtWayland::wl_compositor *compositor = nullptr;
    QMap<uint32_t, QtWayland::wl_output *> outputs;
    QtWayland::wl_shm *shm = nullptr;
    MockWlrForeignToplevelManagerV1 *wlrForeignToplevel = nullptr;
    MockWlrLayerShellV1 *wlrLayerShell = nullptr;
    MockWlrOutputManagerV1 *wlrOutputManager = nullptr;
    MockWlrScreencopyManagerV1 *wlrScreencopyManager = nullptr;

protected:
    void registry_global(uint32_t name, const QString &interface, uint32_t version) override;
    void registry_global_remove(uint32_t name) override;
};

#endif // MOCKREGISTRY_H
