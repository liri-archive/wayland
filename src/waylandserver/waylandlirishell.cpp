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

#include <QFile>

#include <QWaylandCompositor>
#include <QWaylandPointer>
#include <QWaylandSeat>
#include <QWaylandSurface>

#include "waylandlirishell_p.h"
#include "logging_p.h"

#ifndef WL_DISPLAY_ERROR_IMPLEMENTATION
#  define WL_DISPLAY_ERROR_IMPLEMENTATION WL_DISPLAY_ERROR_INVALID_METHOD
#endif

/*
 * WaylandLiriShellPrivate
 */

WaylandLiriShellPrivate::WaylandLiriShellPrivate(WaylandLiriShell *qq)
    : QtWaylandServer::liri_shell()
    , q_ptr(qq)
{
}

WaylandLiriShellPrivate *WaylandLiriShellPrivate::get(WaylandLiriShell *shell)
{
    return shell->d_func();
}

void WaylandLiriShellPrivate::liri_shell_bind_resource(Resource *r)
{
    // Make sure only an authorized client can bind
    pid_t pid;
    wl_client_get_credentials(r->client(), &pid, nullptr, nullptr);
    QFile file(QString::asprintf("/proc/%d/cmdline", pid));
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        QString data = stream.readAll();
        QStringList args = data.split(QLatin1Char('\0'));
        file.close();

        if (!args.first().endsWith(QStringLiteral("liri-shell-helper"))) {
            wl_resource_post_error(r->handle, WL_DISPLAY_ERROR_IMPLEMENTATION,
                                   "unauthorized client program");
            return;
        }
    } else {
        wl_resource_post_error(r->handle, WL_DISPLAY_ERROR_IMPLEMENTATION,
                               "unauthorized client program");
        return;
    }

    // Client can bind only once
    if (resource())
        wl_resource_post_error(r->handle,
                               WL_DISPLAY_ERROR_INVALID_OBJECT,
                               "client can bind only once");
}

void WaylandLiriShellPrivate::liri_shell_set_grab_surface(Resource *resource, struct ::wl_resource *wlSurface)
{
    Q_Q(WaylandLiriShell);

    auto surface = QWaylandSurface::fromResource(wlSurface);
    if (surface) {
        grabSurface = surface;
        Q_EMIT q->grabSurfaceAdded(surface);
    } else {
        qCWarning(lcWaylandServer) << "Couldn't find surface from resource";
        wl_resource_post_error(resource->handle, WL_DISPLAY_ERROR_INVALID_OBJECT,
                               "the specified surface is invalid");
    }
}

void WaylandLiriShellPrivate::liri_shell_ready(QtWaylandServer::liri_shell::Resource *resource)
{
    Q_UNUSED(resource)

    Q_Q(WaylandLiriShell);
    emit q->ready();
}

/*
 * WaylandLiriShell
 */

WaylandLiriShell::WaylandLiriShell()
    : QWaylandCompositorExtensionTemplate<WaylandLiriShell>()
    , d_ptr(new WaylandLiriShellPrivate(this))
{
}

WaylandLiriShell::WaylandLiriShell(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<WaylandLiriShell>(compositor)
    , d_ptr(new WaylandLiriShellPrivate(this))
{
}

WaylandLiriShell::~WaylandLiriShell()
{
    delete d_ptr;
}

void WaylandLiriShell::initialize()
{
    Q_D(WaylandLiriShell);

    QWaylandCompositorExtension::initialize();
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qCWarning(lcWaylandServer) << "Failed to find QWaylandCompositor when initializing WaylandLiriShell";
        return;
    }
    d->init(compositor->display(), 1);
}

void WaylandLiriShell::grabCursor(GrabCursor cursor)
{
    Q_D(WaylandLiriShell);

    if (d->grabSurface) {
        auto resource = d->resourceMap().value(d->grabSurface->waylandClient());
        if (resource)
            d->send_grab_cursor(resource->handle, static_cast<uint32_t>(cursor));
        if (d->grabSurface->views().size() > 0) {
            auto seat = d->grabSurface->compositor()->defaultSeat();
            auto view = d->grabSurface->views().at(0);
            seat->setMouseFocus(view);
            seat->sendMouseMoveEvent(view, QPointF(0, 0), QPointF(0, 0));
        }
    }
}

void WaylandLiriShell::sendQuit()
{
    Q_D(WaylandLiriShell);

    const auto values = d->resourceMap().values();
    for (auto *resource : values)
        d->send_quit(resource->handle);
}

const struct wl_interface *WaylandLiriShell::interface()
{
    return WaylandLiriShellPrivate::interface();
}

QByteArray WaylandLiriShell::interfaceName()
{
    return WaylandLiriShellPrivate::interfaceName();
}
