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

#ifndef LIRIDECORATION_H
#define LIRIDECORATION_H

#include <QPointer>
#include <QWaylandCompositorExtension>
#include <QWaylandResource>
#include <QWaylandSurface>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

struct wl_client;

class LiriDecorationManagerPrivate;
class LiriDecoration;
class LiriDecorationPrivate;

class LIRIWAYLANDSERVER_EXPORT LiriDecorationManager
        : public QWaylandCompositorExtensionTemplate<LiriDecorationManager>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LiriDecorationManager)
public:
    LiriDecorationManager();
    LiriDecorationManager(QWaylandCompositor *compositor);
    ~LiriDecorationManager();

    void initialize() override;

    void unregisterDecoration(LiriDecoration *decoration);

    static const struct wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void decorationCreated(LiriDecoration *decoration);

private:
    LiriDecorationManagerPrivate *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT LiriDecoration : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LiriDecoration)
    Q_PROPERTY(QWaylandSurface *surface READ surface CONSTANT)
    Q_PROPERTY(QColor foregroundColor READ foregroundColor NOTIFY foregroundColorChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY backgroundColorChanged)
public:
    ~LiriDecoration();

    QWaylandSurface *surface() const;
    wl_resource *surfaceResource() const;

    QColor foregroundColor() const;
    QColor backgroundColor() const;

Q_SIGNALS:
    void foregroundColorChanged(const QColor &color);
    void backgroundColorChanged(const QColor &color);

private:
    LiriDecorationPrivate *const d_ptr;

    explicit LiriDecoration(LiriDecorationManager *manager,
                            QWaylandSurface *surface,
                            wl_client *client,
                            quint32 id, quint32 version);

    friend class LiriDecorationManagerPrivate;
};

#endif // LIRIDECORATION_H
