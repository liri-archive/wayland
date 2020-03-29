// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_WAYLANDLIRICOLORPICKER_H
#define LIRI_WAYLANDLIRICOLORPICKER_H

#include <QtWaylandCompositor/QWaylandCompositorExtension>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

class WaylandLiriColorPickerManagerPrivate;

class LIRIWAYLANDSERVER_EXPORT WaylandLiriColorPickerManager
        : public QWaylandCompositorExtensionTemplate<WaylandLiriColorPickerManager>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandLiriColorPickerManager)
    Q_PROPERTY(QString layerName READ layerName WRITE setLayerName NOTIFY layerNameChanged)
public:
    WaylandLiriColorPickerManager();
    WaylandLiriColorPickerManager(QWaylandCompositor *compositor);
    ~WaylandLiriColorPickerManager();

    QString layerName() const;
    void setLayerName(const QString &layer);

    void initialize() override;

    static const wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void layerNameChanged();

private:
    WaylandLiriColorPickerManagerPrivate *const d_ptr = nullptr;
};

#endif // LIRI_WAYLANDLIRICOLORPICKER_H
