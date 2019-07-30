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

#include <QWaylandCompositor>
#include <QWaylandSurface>

#include "kdeserverdecoration_p.h"
#include "liridecoration.h"
#include "logging_p.h"

KdeServerDecorationManagerPrivate::KdeServerDecorationManagerPrivate(KdeServerDecorationManager *self)
    : QtWaylandServer::org_kde_kwin_server_decoration_manager()
    , q_ptr(self)
{
}

void KdeServerDecorationManagerPrivate::org_kde_kwin_server_decoration_manager_bind_resource(QtWaylandServer::org_kde_kwin_server_decoration_manager::Resource *resource)
{
    send_default_mode(resource->handle, static_cast<uint32_t>(defaultMode));
}

void KdeServerDecorationManagerPrivate::org_kde_kwin_server_decoration_manager_create(Resource *resource, uint32_t id, wl_resource *surfaceResource)
{
    Q_Q(KdeServerDecorationManager);

    auto surface = QWaylandSurface::fromResource(surfaceResource);
    if (!surface) {
        wl_resource_post_error(resource->handle, WL_DISPLAY_ERROR_INVALID_OBJECT,
                               "missing wl_surface@%d", wl_resource_get_id(surfaceResource));
        return;
    }

    auto decoration = new KdeServerDecoration(q, surface, resource->client(), id, resource->version());
    Q_EMIT q->decorationCreated(decoration);
}


KdeServerDecorationManager::KdeServerDecorationManager()
    : QWaylandCompositorExtensionTemplate<KdeServerDecorationManager>()
    , d_ptr(new KdeServerDecorationManagerPrivate(this))
{
}

KdeServerDecorationManager::KdeServerDecorationManager(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<KdeServerDecorationManager>(compositor)
    , d_ptr(new KdeServerDecorationManagerPrivate(this))
{
}

void KdeServerDecorationManager::initialize()
{
    Q_D(KdeServerDecorationManager);

    if (d->initialized) {
        qCWarning(lcWaylandServer) << "Cannot initialize KdeServerDecorationManager twice!";
        return;
    }

    d->initialized = true;

    QWaylandCompositorExtensionTemplate::initialize();
    auto *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qCWarning(lcWaylandServer) << "Failed to find QWaylandCompositor when initializing KdeServerDecorationManager";
        return;
    }
    d->init(compositor->display(), KdeServerDecorationManagerPrivate::interfaceVersion());
}

KdeServerDecorationManager::Mode KdeServerDecorationManager::defaultMode() const
{
    Q_D(const KdeServerDecorationManager);
    return d->defaultMode;
}

void KdeServerDecorationManager::setDefaultMode(KdeServerDecorationManager::Mode mode)
{
    Q_D(KdeServerDecorationManager);

    if (d->defaultMode == mode)
        return;

    d->defaultMode = mode;
    if (d->initialized)
        d->send_default_mode(static_cast<uint32_t>(mode));
    Q_EMIT defaultModeChanged();
}

const wl_interface *KdeServerDecorationManager::interface()
{
    return KdeServerDecorationManagerPrivate::interface();
}

QByteArray KdeServerDecorationManager::interfaceName()
{
    return KdeServerDecorationManagerPrivate::interfaceName();
}


KdeServerDecorationPrivate::KdeServerDecorationPrivate(KdeServerDecoration *self,
                                                       KdeServerDecorationManager *_manager,
                                                       QWaylandSurface *_surface,
                                                       wl_client *client,
                                                       quint32 id, quint32 version)
    : QtWaylandServer::org_kde_kwin_server_decoration()
    , manager(_manager)
    , surface(_surface)
    , q_ptr(self)
{
    init(client, id, qMin<quint32>(version, interfaceVersion()));
    send_mode(static_cast<uint32_t>(KdeServerDecorationManagerPrivate::get(manager)->defaultMode));
}

void KdeServerDecorationPrivate::org_kde_kwin_server_decoration_destroy_resource(QtWaylandServer::org_kde_kwin_server_decoration::Resource *resource)
{
    Q_UNUSED(resource)
    delete this;
}

void KdeServerDecorationPrivate::org_kde_kwin_server_decoration_release(QtWaylandServer::org_kde_kwin_server_decoration::Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void KdeServerDecorationPrivate::org_kde_kwin_server_decoration_request_mode(QtWaylandServer::org_kde_kwin_server_decoration::Resource *resource, uint32_t wlMode)
{
    Q_UNUSED(resource)

    Q_Q(KdeServerDecoration);

    mode = static_cast<KdeServerDecorationManager::Mode>(wlMode);
    Q_EMIT q->modeRequested(mode);
    Q_EMIT q->modeChanged();
}


KdeServerDecoration::KdeServerDecoration(KdeServerDecorationManager *manager, QWaylandSurface *surface,
                                         wl_client *client, quint32 id, quint32 version)
    : QObject()
    , d_ptr(new KdeServerDecorationPrivate(this, manager, surface, client, id, version))
{
}

QWaylandSurface *KdeServerDecoration::surface() const
{
    Q_D(const KdeServerDecoration);
    return d->surface;
}

KdeServerDecorationManager::Mode KdeServerDecoration::mode() const
{
    Q_D(const KdeServerDecoration);
    return d->mode;
}

void KdeServerDecoration::setMode(KdeServerDecorationManager::Mode mode)
{
    Q_D(KdeServerDecoration);

    if (d->mode == mode)
        return;

    d->mode = mode;
    Q_EMIT modeChanged();

    d->send_mode(static_cast<uint32_t>(mode));
}
