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

#ifndef LIRI_WAYLANDWLRFOREIGNTOPLEVELMANAGEMENTV1_H
#define LIRI_WAYLANDWLRFOREIGNTOPLEVELMANAGEMENTV1_H

#include <QWaylandCompositorExtension>
#include <qwaylandquickchildren.h>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

QT_FORWARD_DECLARE_CLASS(QWaylandClient)
QT_FORWARD_DECLARE_CLASS(QWaylandCompositor)
QT_FORWARD_DECLARE_CLASS(QWaylandOutput)
QT_FORWARD_DECLARE_CLASS(QWaylandSeat)
QT_FORWARD_DECLARE_CLASS(QWaylandSurface)

class WaylandWlrForeignToplevelHandleV1;
class WaylandWlrForeignToplevelManagerV1Private;
class WaylandWlrForeignToplevelHandleV1Private;

class LIRIWAYLANDSERVER_EXPORT WaylandWlrForeignToplevelManagerV1 : public QWaylandCompositorExtensionTemplate<WaylandWlrForeignToplevelManagerV1>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrForeignToplevelManagerV1)
public:
    WaylandWlrForeignToplevelManagerV1();
    WaylandWlrForeignToplevelManagerV1(QWaylandCompositor *compositor);
    ~WaylandWlrForeignToplevelManagerV1();

    void initialize() override;

    static const wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void handleAdded(WaylandWlrForeignToplevelHandleV1 *handle);
    void clientStopped(QWaylandClient *client);

private:
    WaylandWlrForeignToplevelManagerV1Private *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrForeignToplevelHandleV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrForeignToplevelHandleV1)
    Q_WAYLAND_COMPOSITOR_DECLARE_QUICK_CHILDREN(WaylandWlrForeignToplevelHandleV1)
    Q_PROPERTY(QWaylandCompositor *compositor READ compositor WRITE setCompositor NOTIFY compositorChanged)
    Q_PROPERTY(WaylandWlrForeignToplevelManagerV1 *manager READ manager WRITE setManager NOTIFY managerChanged)
    Q_PROPERTY(bool maximized READ isMaximized WRITE setMaximized NOTIFY maximizedChanged)
    Q_PROPERTY(bool minimized READ isMinimized WRITE setMinimized NOTIFY minimizedChanged)
    Q_PROPERTY(bool activated READ isActivated WRITE setActivated NOTIFY activatedChanged)
    Q_PROPERTY(bool fullscreen READ isFullscreen WRITE setFullscreen NOTIFY fullscreenChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString appId READ appId WRITE setAppId NOTIFY appIdChanged)
    Q_PROPERTY(QWaylandSurface *rectangleSurface READ rectangleSurface NOTIFY rectangleChanged)
    Q_PROPERTY(qint32 rectangleX READ rectangleX NOTIFY rectangleChanged)
    Q_PROPERTY(qint32 rectangleY READ rectangleY NOTIFY rectangleChanged)
    Q_PROPERTY(qint32 rectangleWidth READ rectangleWidth NOTIFY rectangleChanged)
    Q_PROPERTY(qint32 rectangleHeight READ rectangleHeight NOTIFY rectangleChanged)
    Q_PROPERTY(WaylandWlrForeignToplevelHandleV1 *parent READ parent WRITE setParent NOTIFY parentChanged)
public:
    explicit WaylandWlrForeignToplevelHandleV1(QObject *parent = nullptr);
    ~WaylandWlrForeignToplevelHandleV1();

    QWaylandCompositor *compositor() const;
    void setCompositor(QWaylandCompositor *compositor);

    WaylandWlrForeignToplevelManagerV1 *manager() const;
    void setManager(WaylandWlrForeignToplevelManagerV1 *manager);

    bool isMaximized() const;
    void setMaximized(bool maximized);

    bool isMinimized() const;
    void setMinimized(bool minimized);

    bool isFullscreen() const;
    void setFullscreen(bool fullscreen);

    bool isActivated() const;
    void setActivated(bool activated);

    QString title() const;
    void setTitle(const QString &title);

    QString appId() const;
    void setAppId(const QString &appId);

    QWaylandSurface *rectangleSurface() const;
    QRect rectangle() const;
    qint32 rectangleX() const;
    qint32 rectangleY() const;
    qint32 rectangleWidth() const;
    qint32 rectangleHeight() const;

    WaylandWlrForeignToplevelHandleV1 *parent() const;
    void setParent(WaylandWlrForeignToplevelHandleV1 *parentHandle);

    Q_INVOKABLE void sendOutputEnter(QWaylandOutput *output);
    Q_INVOKABLE void sendOutputLeave(QWaylandOutput *output);
    Q_INVOKABLE void sendClosed();

public Q_SLOTS:
    void initialize();

Q_SIGNALS:
    void compositorChanged();
    void managerChanged();
    void titleChanged();
    void appIdChanged();
    void maximizedChanged();
    void minimizedChanged();
    void fullscreenChanged();
    void activatedChanged();
    void maximizeRequested();
    void unmaximizeRequested();
    void minimizeRequested();
    void unminimizeRequested();
    void fullscreenRequested(QWaylandOutput *output);
    void unfullscreenRequested();
    void activateRequested(QWaylandSeat *seat);
    void closeRequested();
    void rectangleChanged();
    void parentChanged();

private:
    WaylandWlrForeignToplevelHandleV1Private *const d_ptr;
};

#endif // LIRI_WAYLANDWLRFOREIGNTOPLEVELMANAGEMENTV1_H
