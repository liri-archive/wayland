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

#include "mockregistry.h"

MockRegistry::MockRegistry(struct ::wl_registry *object)
    : QtWayland::wl_registry(object)
{
}

QtWayland::wl_surface *MockRegistry::createSurface()
{
    return new QtWayland::wl_surface(compositor->create_surface());
}

void MockRegistry::registry_global(uint32_t name, const QString &interface, uint32_t version)
{
    if (interface == QLatin1String("wl_compositor")) {
        compositor = new QtWayland::wl_compositor(object(), name, qMin<uint32_t>(version, 3));
    } else if (interface == QLatin1String("wl_output")) {
        auto *output = new QtWayland::wl_output(object(), name, qMin<uint32_t>(version, 2));
        outputs.insert(name, output);
    } else if (interface == QLatin1String("zwlr_layer_shell_v1")) {
        wlrLayerShell = new MockWlrLayerShellV1(object(), name, qMin<uint32_t>(version, 1));
    } else if (interface == QLatin1String("zwlr_output_manager_v1")) {
        wlrOutputManager = new MockWlrOutputManagerV1(object(), name, qMin<uint32_t>(version, 1));
    }
}

void MockRegistry::registry_global_remove(uint32_t name)
{
    outputs.remove(name);
}
