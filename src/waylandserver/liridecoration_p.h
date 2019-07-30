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

#ifndef LIRI_LIRIDECORATION_P_H
#define LIRI_LIRIDECORATION_P_H

#include <LiriWaylandServer/LiriDecoration>
#include <LiriWaylandServer/private/qwayland-server-liri-decoration.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class LIRIWAYLANDSERVER_EXPORT LiriDecorationManagerPrivate
        : public QtWaylandServer::liri_decoration_manager
{
    Q_DECLARE_PUBLIC(LiriDecorationManager)
public:
    LiriDecorationManagerPrivate(LiriDecorationManager *self);

    bool initialized = false;
    QMap<wl_resource *, LiriDecoration *> decorations;

protected:
    LiriDecorationManager *q_ptr;

    void liri_decoration_manager_create(Resource *resource, uint32_t id, struct ::wl_resource *surfaceResource) override;
    void liri_decoration_manager_destroy(Resource *resource) override;
};

class LIRIWAYLANDSERVER_EXPORT LiriDecorationPrivate
        : public QtWaylandServer::liri_decoration
{
    Q_DECLARE_PUBLIC(LiriDecoration)
public:
    LiriDecorationPrivate(LiriDecoration *self,
                          LiriDecorationManager *_manager,
                          QWaylandSurface *_surface,
                          wl_client *client,
                          quint32 id, quint32 version);

    LiriDecorationManager *manager = nullptr;
    QWaylandSurface *surface = nullptr;
    QColor fgColor = Qt::transparent;
    QColor bgColor = Qt::transparent;

protected:
    LiriDecoration *q_ptr;

    void liri_decoration_destroy_resource(Resource *resource) override;
    void liri_decoration_set_foreground(Resource *resource, const QString &colorName) override;
    void liri_decoration_set_background(Resource *resource, const QString &colorName) override;
    void liri_decoration_destroy(Resource *resource) override;
};

#endif // LIRI_LIRIDECORATION_P_H
