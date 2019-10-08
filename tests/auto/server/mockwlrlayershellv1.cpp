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

#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>
#include <QDebug>

#include "mockwlrlayershellv1.h"

MockWlrLayerShellV1::MockWlrLayerShellV1(struct ::wl_registry *registry, uint32_t name, uint32_t version)
    : QtWayland::zwlr_layer_shell_v1(registry, name, version)
{
}

MockWlrLayerSurfaceV1 *MockWlrLayerShellV1::createSurface(struct ::wl_surface *surface, struct ::wl_output *output,
                                                          quint32 layer, const QString &namespace_)
{
    return new MockWlrLayerSurfaceV1(get_layer_surface(surface, output, layer, namespace_));
}

MockWlrLayerSurfaceV1::MockWlrLayerSurfaceV1(struct ::zwlr_layer_surface_v1 *object)
    : QtWayland::zwlr_layer_surface_v1(object)
{
}

MockWlrLayerSurfaceV1::~MockWlrLayerSurfaceV1()
{
    destroy();
}

QSize MockWlrLayerSurfaceV1::size() const
{
    return m_size;
}

void MockWlrLayerSurfaceV1::zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height)
{
    if (width > 0 && height > 0) {
        m_size = QSize(width, height);
        ack_configure(serial);
    }
}

void MockWlrLayerSurfaceV1::zwlr_layer_surface_v1_closed()
{
    delete this;
}
