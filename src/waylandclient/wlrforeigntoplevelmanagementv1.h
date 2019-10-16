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

#ifndef LIRI_WLRFOREIGNTOPLEVELMANAGEMENTV1_CLIENT_H
#define LIRI_WLRFOREIGNTOPLEVELMANAGEMENTV1_CLIENT_H

#include <QWaylandClientExtension>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

#include <wayland-client.h>

QT_FORWARD_DECLARE_CLASS(QWindow)
QT_FORWARD_DECLARE_CLASS(QScreen)

class WlrForeignToplevelManagerV1Private;
class WlrForeignToplevelHandleV1;
class WlrForeignToplevelHandleV1Private;

class LIRIWAYLANDCLIENT_EXPORT WlrForeignToplevelManagerV1 : public QWaylandClientExtensionTemplate<WlrForeignToplevelManagerV1>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrForeignToplevelManagerV1)
public:
    explicit WlrForeignToplevelManagerV1();
    ~WlrForeignToplevelManagerV1();

    void init(struct ::wl_registry *registry, int id, int version);

    static const wl_interface *interface();

Q_SIGNALS:
    void toplevel(WlrForeignToplevelHandleV1 *handle);

private:
    WlrForeignToplevelManagerV1Private *const d_ptr;
};

class LIRIWAYLANDCLIENT_EXPORT WlrForeignToplevelHandleV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrForeignToplevelHandleV1)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString appId READ appId NOTIFY appIdChanged)
    Q_PROPERTY(bool minimized READ isMinimized NOTIFY minimizedChanged)
    Q_PROPERTY(bool maximized READ isMaximized NOTIFY maximizedChanged)
    Q_PROPERTY(bool fullscreen READ isFullscreen NOTIFY fullscreenChanged)
    Q_PROPERTY(bool actived READ isActived NOTIFY activatedChanged)
public:
    explicit WlrForeignToplevelHandleV1(QObject *parent = nullptr);
    ~WlrForeignToplevelHandleV1();

    QString title() const;
    QString appId() const;

    bool isMinimized() const;
    bool isMaximized() const;
    bool isFullscreen() const;
    bool isActived() const;

    Q_INVOKABLE void setMinimized();
    Q_INVOKABLE void unsetMinimized();

    Q_INVOKABLE void setMaximized();
    Q_INVOKABLE void unsetMaximized();

    Q_INVOKABLE void setFullscreen(QScreen *screen);
    Q_INVOKABLE void unsetFullscreen();

    Q_INVOKABLE void activate(struct ::wl_seat *seat);

    Q_INVOKABLE void setRectangle(QWindow *window, const QRect &rect);

    Q_INVOKABLE void close();

Q_SIGNALS:
    void titleChanged(const QString &title);
    void appIdChanged(const QString &oldAppId, const QString &newAppId);
    void minimizedChanged(bool minimized);
    void maximizedChanged(bool maximized);
    void fullscreenChanged(bool fullscreen);
    void activatedChanged(bool activated);
    void enteredOutput(QScreen *screen);
    void leftOutput(QScreen *screen);
    void closed();

private:
    WlrForeignToplevelHandleV1Private *const d_ptr;

    friend class WlrForeignToplevelManagerV1Private;
};

#endif // LIRI_WLRFOREIGNTOPLEVELMANAGEMENTV1_CLIENT_H
