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

#include "liridecoration_p.h"
#include "logging_p.h"

LiriDecorationManagerPrivate::LiriDecorationManagerPrivate(LiriDecorationManager *self)
    : QtWaylandServer::liri_decoration_manager()
    , q_ptr(self)
{
}

void LiriDecorationManagerPrivate::liri_decoration_manager_create(QtWaylandServer::liri_decoration_manager::Resource *resource, uint32_t id, wl_resource *surfaceResource)
{
    Q_Q(LiriDecorationManager);

    auto surface = QWaylandSurface::fromResource(surfaceResource);
    if (!surface) {
        qCWarning(lcWaylandServer) << "Couldn't find surface";
        return;
    }

    if (decorations.contains(surfaceResource)) {
        qCWarning(lcWaylandServer) << "Decoration object already exist for surface";
        wl_resource_post_error(resource->handle, error_already_exists,
                               "liri_decoration already exist for surface");
        return;
    }

    auto decoration = new LiriDecoration(q, surface, resource->client(), id, resource->version());
    decorations[surfaceResource] = decoration;
    Q_EMIT q->decorationCreated(decoration);
}

void LiriDecorationManagerPrivate::liri_decoration_manager_destroy(QtWaylandServer::liri_decoration_manager::Resource *resource)
{
    wl_resource_destroy(resource->handle);
}


LiriDecorationManager::LiriDecorationManager()
    : QWaylandCompositorExtensionTemplate<LiriDecorationManager>()
    , d_ptr(new LiriDecorationManagerPrivate(this))
{
}

LiriDecorationManager::LiriDecorationManager(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<LiriDecorationManager>(compositor)
    , d_ptr(new LiriDecorationManagerPrivate(this))
{
}

LiriDecorationManager::~LiriDecorationManager()
{
    delete d_ptr;
}

void LiriDecorationManager::initialize()
{
    Q_D(LiriDecorationManager);

    if (d->initialized) {
        qCWarning(lcWaylandServer) << "Cannot initialize KdeServerDecorationManager twice!";
        return;
    }

    d->initialized = true;

    QWaylandCompositorExtensionTemplate::initialize();
    auto *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qCWarning(lcWaylandServer) << "Failed to find QWaylandCompositor when initializing LiriDecorationManager";
        return;
    }
    d->init(compositor->display(), QtWaylandServer::liri_decoration_manager::interfaceVersion());
}

void LiriDecorationManager::unregisterDecoration(LiriDecoration *decoration)
{
    Q_D(LiriDecorationManager);
    d->decorations.remove(decoration->surfaceResource());
}

const wl_interface *LiriDecorationManager::interface()
{
    return LiriDecorationManagerPrivate::interface();
}

QByteArray LiriDecorationManager::interfaceName()
{
    return LiriDecorationManagerPrivate::interfaceName();
}


LiriDecorationPrivate::LiriDecorationPrivate(LiriDecoration *self,
                                             LiriDecorationManager *_manager,
                                             QWaylandSurface *_surface,
                                             wl_client *client,
                                             quint32 id, quint32 version)
    : QtWaylandServer::liri_decoration()
    , manager(_manager)
    , surface(_surface)
    , q_ptr(self)
{
    init(client, id, qMin<quint32>(version, interfaceVersion()));
}

void LiriDecorationPrivate::liri_decoration_destroy_resource(QtWaylandServer::liri_decoration::Resource *resource)
{
    Q_UNUSED(resource)

    Q_Q(LiriDecoration);
    manager->unregisterDecoration(q);
    delete this;
}

void LiriDecorationPrivate::liri_decoration_set_foreground(QtWaylandServer::liri_decoration::Resource *resource, const QString &colorName)
{
    Q_UNUSED(resource)

    Q_Q(LiriDecoration);

    QColor color(colorName);
    if (fgColor == color)
        return;
    fgColor = color;
    Q_EMIT q->foregroundColorChanged(fgColor);
}

void LiriDecorationPrivate::liri_decoration_set_background(QtWaylandServer::liri_decoration::Resource *resource, const QString &colorName)
{
    Q_UNUSED(resource)

    Q_Q(LiriDecoration);

    QColor color(colorName);
    if (bgColor == color)
        return;
    bgColor = color;
    Q_EMIT q->backgroundColorChanged(bgColor);
}

void LiriDecorationPrivate::liri_decoration_destroy(QtWaylandServer::liri_decoration::Resource *resource)
{
    wl_resource_destroy(resource->handle);
}


LiriDecoration::LiriDecoration(LiriDecorationManager *manager, QWaylandSurface *surface,
                               wl_client *client,
                               quint32 id, quint32 version)
    : QObject()
    , d_ptr(new LiriDecorationPrivate(this, manager, surface, client, id, version))
{
}

LiriDecoration::~LiriDecoration()
{
    delete d_ptr;
}

QWaylandSurface *LiriDecoration::surface() const
{
    Q_D(const LiriDecoration);
    return d->surface;
}

wl_resource *LiriDecoration::surfaceResource() const
{
    Q_D(const LiriDecoration);
    return d->surface->resource();
}

QColor LiriDecoration::foregroundColor() const
{
    Q_D(const LiriDecoration);
    return d->fgColor;
}

QColor LiriDecoration::backgroundColor() const
{
    Q_D(const LiriDecoration);
    return d->bgColor;
}
