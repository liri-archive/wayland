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

#ifndef KDESERVERDECORATION_H
#define KDESERVERDECORATION_H

#include <QWaylandCompositorExtension>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

QT_FORWARD_DECLARE_CLASS(QWaylandSurface)

struct wl_client;

class KdeServerDecorationManagerPrivate;
class KdeServerDecorationPrivate;

class LIRIWAYLANDSERVER_EXPORT KdeServerDecorationManager
        : public QWaylandCompositorExtensionTemplate<KdeServerDecorationManager>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(KdeServerDecorationManager)
    Q_PROPERTY(Mode defaultMode READ defaultMode WRITE setDefaultMode NOTIFY defaultModeChanged)
public:
    enum Mode {
        None = 0,
        Client,
        Server
    };
    Q_ENUM(Mode)

    KdeServerDecorationManager();
    KdeServerDecorationManager(QWaylandCompositor *compositor);

    void initialize() override;

    Mode defaultMode() const;
    void setDefaultMode(Mode mode);

    static const struct wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void defaultModeChanged();
    void decorationCreated(class KdeServerDecoration *decoration);

private:
    KdeServerDecorationManagerPrivate *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT KdeServerDecoration : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(KdeServerDecoration)
    Q_PROPERTY(QWaylandSurface *surface READ surface CONSTANT)
    Q_PROPERTY(KdeServerDecorationManager::Mode mode READ mode WRITE setMode NOTIFY modeChanged)
public:
    QWaylandSurface *surface() const;

    KdeServerDecorationManager::Mode mode() const;
    void setMode(KdeServerDecorationManager::Mode mode);

Q_SIGNALS:
    void modeChanged();
    void modeRequested(KdeServerDecorationManager::Mode mode);

private:
    KdeServerDecorationPrivate *const d_ptr;

    explicit KdeServerDecoration(KdeServerDecorationManager *manager,
                                 QWaylandSurface *surface,
                                 wl_client *client,
                                 quint32 id, quint32 version);

    friend class KdeServerDecorationManagerPrivate;
};

Q_DECLARE_METATYPE(KdeServerDecorationManager::Mode)

#endif // KDESERVERDECORATION_H
