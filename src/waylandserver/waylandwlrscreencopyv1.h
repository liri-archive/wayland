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

#ifndef LIRI_WAYLANDWLRSCREENCOPYV1_H
#define LIRI_WAYLANDWLRSCREENCOPYV1_H

#include <QtWaylandCompositor/QWaylandCompositorExtension>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

QT_FORWARD_DECLARE_CLASS(QQuickItem)
QT_FORWARD_DECLARE_CLASS(QWaylandOutput)

class WaylandWlrScreencopyManagerV1Private;
class WaylandWlrScreencopyFrameV1;
class WaylandWlrScreencopyFrameV1Private;

class LIRIWAYLANDSERVER_EXPORT WaylandWlrScreencopyManagerV1
        : public QWaylandCompositorExtensionTemplate<WaylandWlrScreencopyManagerV1>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrScreencopyManagerV1)
public:
    WaylandWlrScreencopyManagerV1();
    WaylandWlrScreencopyManagerV1(QWaylandCompositor *compositor);
    ~WaylandWlrScreencopyManagerV1();

    void initialize() override;

    static const wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void captureOutputRequested(WaylandWlrScreencopyFrameV1 *frame);

private:
    WaylandWlrScreencopyManagerV1Private *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrScreencopyFrameV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrScreencopyFrameV1)
    Q_PROPERTY(bool overlayCursor READ overlayCursor CONSTANT)
    Q_PROPERTY(QWaylandOutput *output READ output CONSTANT)
    Q_PROPERTY(QRect region READ region CONSTANT)
    Q_PROPERTY(WaylandWlrScreencopyFrameV1::Flags flags READ flags WRITE setFlags NOTIFY flagsChanged)
public:
    enum Flag {
        YInvert = 1
    };
    Q_ENUM(Flag)
    Q_DECLARE_FLAGS(Flags, Flag)

    ~WaylandWlrScreencopyFrameV1();

    bool overlayCursor() const;
    QWaylandOutput *output() const;
    QRect region() const;

    Flags flags() const;
    void setFlags(Flags flags);

    Q_INVOKABLE void grabCursorItem(QQuickItem *item);
    Q_INVOKABLE void copy();

Q_SIGNALS:
    void flagsChanged();
    void ready();

private:
    WaylandWlrScreencopyFrameV1Private *const d_ptr;

    explicit WaylandWlrScreencopyFrameV1(QObject *parent = nullptr);

    friend class WaylandWlrScreencopyManagerV1Private;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(WaylandWlrScreencopyFrameV1::Flags)

#endif // LIRI_WAYLANDWLRSCREENCOPYV1_H
