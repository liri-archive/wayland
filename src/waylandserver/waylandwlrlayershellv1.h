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

#ifndef LIRI_WAYLANDWLRLAYERSHELLV1_H
#define LIRI_WAYLANDWLRLAYERSHELLV1_H

#include <QMargins>
#include <QLoggingCategory>
#include <QQmlEngine>
#include <QWaylandCompositorExtension>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

QT_FORWARD_DECLARE_CLASS(QWaylandCompositor)
QT_FORWARD_DECLARE_CLASS(QWaylandOutput)
QT_FORWARD_DECLARE_CLASS(QWaylandSurface)
QT_FORWARD_DECLARE_CLASS(QWaylandSurfaceRole)

Q_DECLARE_LOGGING_CATEGORY(lcLayerShell)

class WaylandWlrLayerShellV1Private;
class WaylandWlrLayerSurfaceV1;
class WaylandWlrLayerSurfaceV1Private;

class LIRIWAYLANDSERVER_EXPORT WaylandWlrLayerShellV1 : public QWaylandCompositorExtensionTemplate<WaylandWlrLayerShellV1>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrLayerShellV1)
public:
    enum Layer {
        BackgroundLayer = 0,
        BottomLayer = 1,
        TopLayer = 2,
        OverlayLayer = 3
    };
    Q_ENUM(Layer)

    WaylandWlrLayerShellV1();
    WaylandWlrLayerShellV1(QWaylandCompositor *compositor);
    ~WaylandWlrLayerShellV1();

    void initialize() override;

    Q_INVOKABLE void closeAllSurfaces();

    static const wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void layerSurfaceCreated(WaylandWlrLayerSurfaceV1 *layerSurface);

private:
    WaylandWlrLayerShellV1Private *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrLayerSurfaceV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrLayerSurfaceV1)
    Q_PROPERTY(QWaylandSurface *surface READ surface CONSTANT)
    Q_PROPERTY(QWaylandOutput *output READ output CONSTANT)
    Q_PROPERTY(WaylandWlrLayerShellV1::Layer layer READ layer NOTIFY layerChanged)
    Q_PROPERTY(QString nameSpace READ nameSpace CONSTANT)
    Q_PROPERTY(quint32 width READ width NOTIFY sizeChanged)
    Q_PROPERTY(quint32 height READ height NOTIFY sizeChanged)
    Q_PROPERTY(WaylandWlrLayerSurfaceV1::Anchors anchors READ anchors NOTIFY anchorsChanged)
    Q_PROPERTY(int exclusiveZone READ exclusiveZone NOTIFY exclusiveZoneChanged)
    Q_PROPERTY(qint32 leftMargin READ leftMargin NOTIFY leftMarginChanged)
    Q_PROPERTY(qint32 topMargin READ topMargin NOTIFY topMarginChanged)
    Q_PROPERTY(qint32 rightMargin READ rightMargin NOTIFY rightMarginChanged)
    Q_PROPERTY(qint32 bottomMargin READ bottomMargin NOTIFY bottomMarginChanged)
    Q_PROPERTY(KeyboardInteractivity keyboardInteractivity READ keyboardInteractivity NOTIFY keyboardInteractivityChanged)
    Q_PROPERTY(bool mapped READ isMapped NOTIFY mappedChanged)
    Q_PROPERTY(bool configured READ isConfigured NOTIFY configuredChanged)
public:
    enum Anchor {
        TopAnchor = 1,
        BottomAnchor = 2,
        LeftAnchor = 4,
        RightAnchor = 8
    };
    Q_ENUM(Anchor)
    Q_DECLARE_FLAGS(Anchors, Anchor)
    Q_FLAG(Anchors)

    enum KeyboardInteractivity {
        NoKeyboardInteractivity = 0,
        ExclusiveKeyboardInteractivity = 1,
        OnDemandKeyboardInteractivity = 2
    };
    Q_ENUM(KeyboardInteractivity)

    ~WaylandWlrLayerSurfaceV1();

    QWaylandSurface *surface() const;
    QWaylandOutput *output() const;
    WaylandWlrLayerShellV1::Layer layer() const;
    QString nameSpace() const;

    QSize size() const;
    quint32 width() const;
    quint32 height() const;
    Anchors anchors() const;
    int exclusiveZone() const;
    QMargins margins() const;
    qint32 leftMargin() const;
    qint32 topMargin() const;
    qint32 rightMargin() const;
    qint32 bottomMargin() const;
    KeyboardInteractivity keyboardInteractivity() const;
    bool isMapped() const;
    bool isConfigured() const;

    Q_INVOKABLE quint32 sendConfigure(const QSize &size);
    Q_INVOKABLE quint32 sendConfigure(int width, int height);
    Q_INVOKABLE void close();

    static QWaylandSurfaceRole *role();

Q_SIGNALS:
    void layerChanged();
    void sizeChanged();
    void anchorsChanged();
    void exclusiveZoneChanged();
    void leftMarginChanged();
    void topMarginChanged();
    void rightMarginChanged();
    void bottomMarginChanged();
    void keyboardInteractivityChanged();
    void changed();
    void mappedChanged();
    void configuredChanged();

private:
    WaylandWlrLayerSurfaceV1Private *const d_ptr;

    explicit WaylandWlrLayerSurfaceV1(QWaylandSurface *surface,
                                      QWaylandOutput *output,
                                      WaylandWlrLayerShellV1::Layer layer,
                                      const QString &nameSpace,
                                      QObject *parent = nullptr);

    friend class WaylandWlrLayerShellV1Private;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(WaylandWlrLayerSurfaceV1::Anchors)

QML_DECLARE_TYPE(WaylandWlrLayerSurfaceV1)

#endif // LIRI_WAYLANDWLRLAYERSHELLV1_H
