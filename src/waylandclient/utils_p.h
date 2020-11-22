/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2019 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef LIRI_WAYLANDCLIENT_UTILS_P_H
#define LIRI_WAYLANDCLIENT_UTILS_P_H

#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>
#include <QWindow>
#include <QScreen>

#include <wayland-client.h>

static inline struct ::wl_seat *getWlSeat()
{
    void *seat = QGuiApplication::platformNativeInterface()->nativeResourceForIntegration("wl_seat");
    return static_cast<struct ::wl_seat *>(seat);
}

static inline struct ::wl_surface *getWlSurface(QWindow *window)
{
    if (!window)
        return nullptr;
    void *surface = QGuiApplication::platformNativeInterface()->nativeResourceForWindow("surface", window);
    return static_cast<struct ::wl_surface *>(surface);
}

static inline struct ::wl_output *getWlOutput(QScreen *screen)
{
    if (!screen)
        return nullptr;
    void *output = QGuiApplication::platformNativeInterface()->nativeResourceForScreen("output", screen);
    return static_cast<struct ::wl_output *>(output);
}

static inline QScreen *getScreen(struct ::wl_output *output)
{
    const auto screens = QGuiApplication::screens();
    for (auto screen : screens) {
        if (getWlOutput(screen) == output)
            return screen;
    }

    return nullptr;
}

#endif // LIRI_WAYLANDCLIENT_UTILS_P_H
