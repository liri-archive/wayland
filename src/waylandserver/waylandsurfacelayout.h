// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_WAYLANDSURFACELAYOUT_H
#define LIRI_WAYLANDSURFACELAYOUT_H

#include <QQuickItem>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

QT_FORWARD_DECLARE_CLASS(QWaylandOutput)

class WaylandWlrLayerSurfaceV1;
class WaylandSurfaceLayoutPrivate;

class LIRIWAYLANDSERVER_EXPORT WaylandSurfaceLayout : public QQuickItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandSurfaceLayout)
    Q_PROPERTY(QWaylandOutput *output READ output WRITE setOutput NOTIFY outputChanged)
    QML_ELEMENT
public:
    enum Layer {
        NoLayer = 0,
        BackgroundLayer,
        BottomLayer,
        WindowsLayer,
        TopLayer,
        OverlayLayer
    };
    Q_ENUM(Layer);

    WaylandSurfaceLayout(QQuickItem *parent = nullptr);
    ~WaylandSurfaceLayout();

    QWaylandOutput *output() const;
    void setOutput(QWaylandOutput *output);

Q_SIGNALS:
    void outputChanged(QWaylandOutput *output);

protected:
    void itemChange(ItemChange change,
                    const ItemChangeData &data) override;
    void geometryChanged(const QRectF &newGeometry,
                         const QRectF &oldGeometry) override;
    void updatePolish() override;
    void componentComplete() override;

    virtual WaylandSurfaceLayout::Layer getLayer(QQuickItem *item) const;
    virtual bool sortItems(QQuickItem *left, QQuickItem *right);

private:
    WaylandSurfaceLayoutPrivate *const d_ptr = nullptr;
};

#endif // LIRI_WAYLANDSURFACELAYOUT_H
