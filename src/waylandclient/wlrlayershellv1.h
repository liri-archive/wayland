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

#ifndef LIRI_WLRLAYERSHELLV1_CLIENT_H
#define LIRI_WLRLAYERSHELLV1_CLIENT_H

#include <QMargins>
#include <QQmlEngine>
#include <QWaylandClientExtension>
#include <QWindow>
#include <QScreen>
#include <QSize>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

#include <wayland-client.h>

class XdgPopup;

class WlrLayerShellV1Private;
class WlrLayerSurfaceV1;
class WlrLayerSurfaceV1Private;

class LIRIWAYLANDCLIENT_EXPORT WlrLayerShellV1 : public QWaylandClientExtensionTemplate<WlrLayerShellV1>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrLayerShellV1)
public:
    enum Layer {
        BackgroundLayer = 0,
        BottomLayer = 1,
        TopLayer = 2,
        OverlayLayer = 3
    };
    Q_ENUM(Layer)

    explicit WlrLayerShellV1();
    ~WlrLayerShellV1();

    void init(struct ::wl_registry *registry, int id, int version);

    static const wl_interface *interface();

private:
    WlrLayerShellV1Private *const d_ptr;
};

class LIRIWAYLANDCLIENT_EXPORT WlrLayerSurfaceV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrLayerSurfaceV1)
    Q_PROPERTY(WlrLayerShellV1 *shell READ shell WRITE setShell NOTIFY shellChanged)
    Q_PROPERTY(XdgPopup *xdgPopup READ xdgPopup WRITE setXdgPopup NOTIFY xdgPopupChanged)
    Q_PROPERTY(WlrLayerShellV1::Layer layer READ layer WRITE setLayer NOTIFY layerChanged)
    Q_PROPERTY(QString nameSpace READ nameSpace WRITE setNameSpace NOTIFY nameSpaceChanged)
    Q_PROPERTY(bool showOnAllScreens READ showOnAllScreens WRITE setShowOnAllScreens NOTIFY showOnAllScreensChanged)
    Q_PROPERTY(QWindow *window READ window WRITE setWindow NOTIFY windowChanged)
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(WlrLayerSurfaceV1::Anchors anchors READ anchors WRITE setAnchors NOTIFY anchorsChanged)
    Q_PROPERTY(qint32 exclusiveZone READ exclusiveZone WRITE setExclusiveZone NOTIFY exclusiveZoneChanged)
    Q_PROPERTY(qint32 leftMargin READ leftMargin WRITE setLeftMargin NOTIFY leftMarginChanged)
    Q_PROPERTY(qint32 rightMargin READ rightMargin WRITE setRightMargin NOTIFY rightMarginChanged)
    Q_PROPERTY(qint32 topMargin READ topMargin WRITE setTopMargin NOTIFY topMarginChanged)
    Q_PROPERTY(qint32 bottomMargin READ bottomMargin WRITE setBottomMargin NOTIFY bottomMarginChanged)
    Q_PROPERTY(bool keyboardInteractivity READ keyboardInteractivity WRITE setKeyboardInteractivity NOTIFY keyboardInteractivityChanged)
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

    explicit WlrLayerSurfaceV1(QObject *parent);
    ~WlrLayerSurfaceV1();

    WlrLayerShellV1 *shell() const;
    void setShell(WlrLayerShellV1 *shell);

    XdgPopup *xdgPopup() const;
    void setXdgPopup(XdgPopup *xdgPopup);

    WlrLayerShellV1::Layer layer() const;
    void setLayer(WlrLayerShellV1::Layer layer);

    QString nameSpace() const;
    void setNameSpace(const QString &nameSpace);

    bool showOnAllScreens() const;
    void setShowOnAllScreens(bool show);

    QWindow *window() const;
    void setWindow(QWindow *window);

    QSize size() const;
    void setSize(const QSize &size);

    WlrLayerSurfaceV1::Anchors anchors() const;
    void setAnchors(WlrLayerSurfaceV1::Anchors anchors);

    qint32 exclusiveZone() const;
    void setExclusiveZone(qint32 zone);

    QMargins margins() const;

    qint32 leftMargin() const;
    void setLeftMargin(qint32 margin);

    qint32 rightMargin() const;
    void setRightMargin(qint32 margin);

    qint32 topMargin() const;
    void setTopMargin(qint32 margin);

    qint32 bottomMargin() const;
    void setBottomMargin(qint32 margin);

    bool keyboardInteractivity() const;
    void setKeyboardInteractivity(bool enabled);

    Q_INVOKABLE void ackConfigure(quint32 serial);

    Q_INVOKABLE void addMask(const QRect &rect);
    Q_INVOKABLE void unsetMask();

public Q_SLOTS:
    void initialize();

Q_SIGNALS:
    void configured(quint32 serial, quint32 width, quint32 height);
    void closed();
    void shellChanged();
    void xdgPopupChanged();
    void layerChanged();
    void nameSpaceChanged();
    void showOnAllScreensChanged();
    void windowChanged();
    void sizeChanged();
    void anchorsChanged();
    void exclusiveZoneChanged();
    void leftMarginChanged();
    void rightMarginChanged();
    void topMarginChanged();
    void bottomMarginChanged();
    void keyboardInteractivityChanged();

private:
    WlrLayerSurfaceV1Private *const d_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(WlrLayerSurfaceV1::Anchors)

QML_DECLARE_TYPE(WlrLayerSurfaceV1)

#endif // LIRI_WLRLAYERSHELLV1_CLIENT_H
