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

#ifndef MOCKWLRLAYERSHELLV1_H
#define MOCKWLRLAYERSHELLV1_H

#include <QRect>
#include <QSize>
#include <QWindow>

#include "qwayland-wlr-layer-shell-unstable-v1.h"

class MockWlrLayerSurfaceV1;

class MockWlrLayerShellV1 : public QtWayland::zwlr_layer_shell_v1
{
public:
    MockWlrLayerShellV1(struct ::wl_registry *registry, uint32_t name, uint32_t version);

    MockWlrLayerSurfaceV1 *createSurface(struct ::wl_surface *surface, struct ::wl_output *output,
                                         quint32 layer, const QString &namespace_);
};

class MockWlrLayerSurfaceV1 : public QtWayland::zwlr_layer_surface_v1
{
public:
    MockWlrLayerSurfaceV1(struct ::zwlr_layer_surface_v1 *object);
    ~MockWlrLayerSurfaceV1();

    QSize size() const;

protected:
    void zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height) override;
    void zwlr_layer_surface_v1_closed() override;

private:
    QSize m_size;
};

#endif // MOCKWLRLAYERSHELLV1_H
