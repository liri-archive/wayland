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

#ifndef LIRI_WLRSCREENCOPYV1_CLIENT_H
#define LIRI_WLRSCREENCOPYV1_CLIENT_H

#include <QtWaylandClient/QWaylandClientExtension>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

#include <wayland-client.h>

QT_FORWARD_DECLARE_CLASS(QScreen)

class WlrScreencopyManagerV1Private;
class WlrScreencopyFrameV1;
class WlrScreencopyFrameV1Private;

class LIRIWAYLANDCLIENT_EXPORT WlrScreencopyManagerV1
        : public QWaylandClientExtensionTemplate<WlrScreencopyManagerV1>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrScreencopyManagerV1)
public:
    explicit WlrScreencopyManagerV1();
    ~WlrScreencopyManagerV1();

    Q_INVOKABLE WlrScreencopyFrameV1 *captureScreen(QScreen *screen, bool overlayCursor = false);
    Q_INVOKABLE WlrScreencopyFrameV1 *captureScreenRegion(QScreen *screen, const QRect &region,
                                                          bool overlayCursor = false);

    void init(struct ::wl_registry *registry, int id, int version);

    static const struct ::wl_interface *interface();

private:
    WlrScreencopyManagerV1Private *const d_ptr;
};

class LIRIWAYLANDCLIENT_EXPORT WlrScreencopyFrameV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrScreencopyFrameV1)
    Q_PROPERTY(QScreen *screen READ screen CONSTANT)
public:
    ~WlrScreencopyFrameV1();

    QScreen *screen() const;

Q_SIGNALS:
    void copied(const QImage &image);
    void failed();

private:
    WlrScreencopyFrameV1Private *const d_ptr;

    explicit WlrScreencopyFrameV1(QObject *parent = nullptr);

    friend class WlrScreencopyManagerV1;
};

#endif // LIRI_WLRSCREENCOPYV1_CLIENT_H
