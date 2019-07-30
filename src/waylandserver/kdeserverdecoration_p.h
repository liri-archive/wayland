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

#ifndef LIRI_KDESERVERDECORATION_P_H
#define LIRI_KDESERVERDECORATION_P_H

#include <LiriWaylandServer/KdeServerDecoration>
#include <LiriWaylandServer/private/qwayland-server-server-decoration.h>

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

class LIRIWAYLANDSERVER_EXPORT KdeServerDecorationManagerPrivate
        : public QtWaylandServer::org_kde_kwin_server_decoration_manager
{
    Q_DECLARE_PUBLIC(KdeServerDecorationManager)
public:
    KdeServerDecorationManagerPrivate(KdeServerDecorationManager *self);

    bool initialized = false;
    KdeServerDecorationManager::Mode defaultMode = KdeServerDecorationManager::None;

    static KdeServerDecorationManagerPrivate *get(KdeServerDecorationManager *manager) { return manager ? manager->d_func() : nullptr; }

protected:
    KdeServerDecorationManager *q_ptr;

    void org_kde_kwin_server_decoration_manager_bind_resource(Resource *resource) override;
    void org_kde_kwin_server_decoration_manager_create(Resource *resource,
                                                       uint32_t id,
                                                       struct ::wl_resource *surfaceResource) override;
};

class LIRIWAYLANDSERVER_EXPORT KdeServerDecorationPrivate
        : public QtWaylandServer::org_kde_kwin_server_decoration
{
    Q_DECLARE_PUBLIC(KdeServerDecoration)
public:
    KdeServerDecorationPrivate(KdeServerDecoration *self,
                               KdeServerDecorationManager *_manager,
                               QWaylandSurface *_surface,
                               wl_client *client,
                               quint32 id, quint32 version);

    KdeServerDecorationManager *manager = nullptr;
    QWaylandSurface *surface = nullptr;
    KdeServerDecorationManager::Mode mode = KdeServerDecorationManager::None;

protected:
    KdeServerDecoration *q_ptr;

    void org_kde_kwin_server_decoration_destroy_resource(Resource *resource) override;
    void org_kde_kwin_server_decoration_release(Resource *resource) override;
    void org_kde_kwin_server_decoration_request_mode(Resource *resource, uint32_t mode) override;
};

#endif // LIRI_KDESERVERDECORATION_P_H
