// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_LAYERSURFACEINTERFACE_H
#define LIRI_LAYERSURFACEINTERFACE_H

#include <QWindow>

#include <LiriWaylandClientLayerShell/liriwaylandclientlayershellglobal.h>

namespace Liri {

namespace WaylandClient {

class LayerSurfacePrivate;

class LIRIWAYLANDCLIENTLAYERSHELL_EXPORT LayerSurface : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LayerSurface)
    Q_PROPERTY(QWindow *window READ window WRITE setWindow NOTIFY windowChanged)
    Q_PROPERTY(Liri::WaylandClient::LayerSurface::Layer layer READ layer WRITE setLayer NOTIFY layerChanged)
    Q_PROPERTY(QString role READ role WRITE setRole NOTIFY roleChanged)
    Q_PROPERTY(Liri::WaylandClient::LayerSurface::Anchors anchors READ anchors WRITE setAnchors NOTIFY anchorsChanged)
    Q_PROPERTY(qint32 exclusiveZone READ exclusiveZone WRITE setExclusiveZone NOTIFY exclusiveZoneChanged)
    Q_PROPERTY(QMargins margins READ margins NOTIFY marginsChanged)
    Q_PROPERTY(qint32 leftMargin READ leftMargin WRITE setLeftMargin NOTIFY leftMarginChanged)
    Q_PROPERTY(qint32 rightMargin READ rightMargin WRITE setRightMargin NOTIFY rightMarginChanged)
    Q_PROPERTY(qint32 topMargin READ topMargin WRITE setTopMargin NOTIFY topMarginChanged)
    Q_PROPERTY(qint32 bottomMargin READ bottomMargin WRITE setBottomMargin NOTIFY bottomMarginChanged)
    Q_PROPERTY(Liri::WaylandClient::LayerSurface::KeyboardInteractivity keyboardInteractivity READ keyboardInteractivity WRITE setKeyboardInteractivity NOTIFY keyboardInteractivityChanged)
public:
    enum Layer {
        BackgroundLayer = 0,
        BottomLayer = 1,
        TopLayer = 2,
        OverlayLayer = 3
    };
    Q_ENUM(Layer)

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
        OnDemandKeyboardInteractivity
    };
    Q_ENUM(KeyboardInteractivity)

    LayerSurface(QObject *parent = nullptr);
    ~LayerSurface();

    bool isInitialized() const;

    QWindow *window() const;
    void setWindow(QWindow *window);

    Liri::WaylandClient::LayerSurface::Layer layer() const;
    void setLayer(Liri::WaylandClient::LayerSurface::Layer layer);

    QString role() const;
    void setRole(const QString &role);

    Liri::WaylandClient::LayerSurface::Anchors anchors() const;
    void setAnchors(Liri::WaylandClient::LayerSurface::Anchors anchors);

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

    Liri::WaylandClient::LayerSurface::KeyboardInteractivity keyboardInteractivity() const;
    void setKeyboardInteractivity(Liri::WaylandClient::LayerSurface::KeyboardInteractivity keyboardInteractivity);

    void initialize();

    void setLayerEnabled(bool enabled);

    static LayerSurface *get(QWindow *window);

Q_SIGNALS:
    void windowChanged(QWindow *window);
    void layerChanged(LayerSurface::Layer layer);
    void roleChanged(const QString &role);
    void anchorsChanged(LayerSurface::Anchors anchors);
    void exclusiveZoneChanged(qint32 exclusiveZone);
    void marginsChanged(const QMargins &margins);
    void leftMarginChanged(qint32 margin);
    void rightMarginChanged(qint32 margin);
    void topMarginChanged(qint32 margin);
    void bottomMarginChanged(qint32 margin);
    void keyboardInteractivityChanged(Liri::WaylandClient::LayerSurface::KeyboardInteractivity keyboardInteractivity);

private:
    LayerSurfacePrivate *const d_ptr = nullptr;
};

} // WaylandClient

} // Liri

Q_DECLARE_OPERATORS_FOR_FLAGS(Liri::WaylandClient::LayerSurface::Anchors)

#endif // LIRI_LAYERSURFACEINTERFACE_H
