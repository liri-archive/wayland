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

#ifndef LIRI_GTKSHELL_H
#define LIRI_GTKSHELL_H

#include <QWaylandCompositorExtension>
#include <QWaylandResource>
#include <QWaylandShellSurface>
#include <qwaylandquickchildren.h>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

QT_FORWARD_DECLARE_CLASS(QWaylandClient)
QT_FORWARD_DECLARE_CLASS(QWaylandSurface)

class GtkShellPrivate;
class GtkSurface;
class GtkSurfacePrivate;

class LIRIWAYLANDSERVER_EXPORT GtkShell : public QWaylandCompositorExtensionTemplate<GtkShell>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GtkShell)
public:
    GtkShell();
    GtkShell(QWaylandCompositor *compositor);
    ~GtkShell();

    void initialize() override;

    static const struct wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void gtkSurfaceRequested(QWaylandSurface *surface,
                             const QWaylandResource &resource);
    void gtkSurfaceCreated(GtkSurface *gtkSurface);

private:
    GtkShellPrivate *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT GtkSurface : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GtkSurface)
    Q_WAYLAND_COMPOSITOR_DECLARE_QUICK_CHILDREN(GtkSurface)
    Q_PROPERTY(QWaylandSurface *surface READ surface NOTIFY surfaceChanged)
    Q_PROPERTY(GtkShell *shell READ shell NOTIFY shellChanged)
    Q_PROPERTY(QString appId READ appId NOTIFY appIdChanged)
public:
    GtkSurface(QObject *parent = nullptr);
    GtkSurface(GtkShell *shell, QWaylandSurface *surface,
               const QWaylandResource &resource,
               QObject *parent = nullptr);
    ~GtkSurface();

    Q_INVOKABLE void initialize(GtkShell *shell, QWaylandSurface *surface,
                                const QWaylandResource &resource);

    QWaylandSurface *surface() const;
    GtkShell *shell() const;

    QString appId() const;
    QString appMenuPath() const;
    QString menuBarPath() const;
    QString windowObjectPath() const;
    QString appObjectPath() const;
    QString uniqueBusName() const;

    static const struct wl_interface *interface();
    static QByteArray interfaceName();

    static GtkSurface *fromResource(wl_resource *resource);

Q_SIGNALS:
    void surfaceChanged();
    void shellChanged();

    void appIdChanged(const QString &appId);
    void appMenuPathChanged(const QString &appMenuPath);
    void menuBarPathChanged(const QString &menuBarPath);
    void windowObjectPathChanged(const QString &windowObjectPath);
    void appObjectPathChanged(const QString &appObjectPath);
    void uniqueBusNameChanged(const QString &uniqueBusName);

    void setModal();
    void unsetModal();

private:
    GtkSurfacePrivate *const d_ptr;
};

#endif // LIRI_GTKSHELL_H
