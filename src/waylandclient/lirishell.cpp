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

#include <QtGui/QGuiApplication>
#include <QtGui/QPlatformSurfaceEvent>
#include <QtGui/QWindow>
#include <QtGui/qpa/qplatformnativeinterface.h>

#include "lirishell_p.h"

static inline struct ::wl_surface *getWlSurface(QWindow *window)
{
    void *surface = QGuiApplication::platformNativeInterface()->nativeResourceForWindow("surface", window);
    return static_cast<struct ::wl_surface *>(surface);
}

// LiriShellPrivate

LiriShellPrivate::LiriShellPrivate(LiriShell *qq)
    : QtWayland::liri_shell()
    , q_ptr(qq)
{
}

void LiriShellPrivate::sendGrabSurfaceRegistration(QWindow *window)
{
    Q_Q(LiriShell);

    if (window->handle())
        setGrabSurface(window);
    else
        window->installEventFilter(q);

    activated = true;
}

void LiriShellPrivate::setGrabSurface(QWindow *window)
{
    set_grab_surface(getWlSurface(window));
    window->setCursor(QCursor(Qt::ArrowCursor));
}

void LiriShellPrivate::handleActivation()
{
    Q_Q(LiriShell);

    if (q->isActive() && !activated && grabWindow)
        sendGrabSurfaceRegistration(grabWindow);
}

void LiriShellPrivate::liri_shell_grab_cursor(uint32_t cursor)
{
    Q_Q(LiriShell);
    Q_EMIT q->cursorChangeRequested(static_cast<LiriShell::GrabCursor>(cursor));
}

// LiriShell

LiriShell::LiriShell()
    : QWaylandClientExtensionTemplate(1)
    , d_ptr(new LiriShellPrivate(this))
{
    connect(this, &LiriShell::activeChanged, this, [this] {
        Q_D(LiriShell);
        d->handleActivation();
    });
}

LiriShell::~LiriShell()
{
    delete d_ptr;
}

void LiriShell::init(wl_registry *registry, int id, int version)
{
    Q_D(LiriShell);
    d->init(registry, id, version);
}

void LiriShell::registerGrabSurface(QWindow *window)
{
    Q_D(LiriShell);

    d->grabWindow = window;
    if (isActive())
        d->sendGrabSurfaceRegistration(window);
}

const wl_interface *LiriShell::interface()
{
    return QtWayland::liri_shell::interface();
}

bool LiriShell::eventFilter(QObject *watched, QEvent *event)
{
    Q_D(LiriShell);

    if (event->type() == QEvent::PlatformSurface
            && static_cast<QPlatformSurfaceEvent *>(event)->surfaceEventType() == QPlatformSurfaceEvent::SurfaceCreated) {
        QWindow *window = qobject_cast<QWindow*>(watched);
        Q_ASSERT(window);
        window->removeEventFilter(this);
        d->setGrabSurface(window);
    }

    return false;
}

#include "moc_lirishell.cpp"
