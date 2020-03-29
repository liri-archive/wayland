// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_LIRICOLORPICKER_CLIENT_H
#define LIRI_LIRICOLORPICKER_CLIENT_H

#include <QColor>
#include <QtWaylandClient/QWaylandClientExtension>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

#include <wayland-client.h>

QT_FORWARD_DECLARE_CLASS(QScreen)

class LiriColorPickerManagerPrivate;

class LIRIWAYLANDCLIENT_EXPORT LiriColorPickerManager
        : public QWaylandClientExtensionTemplate<LiriColorPickerManager>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LiriColorPickerManager)
public:
    LiriColorPickerManager();
    ~LiriColorPickerManager();

    Q_INVOKABLE quint32 pickAtLocation(QScreen *screen, const QPoint &location);
    Q_INVOKABLE quint32 pickInteractively(QScreen *screen);

    void init(struct ::wl_registry *registry, int id, int version);

    static const struct ::wl_interface *interface();

Q_SIGNALS:
    void colorPicked(quint32 serial, const QColor &color);

private:
    LiriColorPickerManagerPrivate *const d_ptr = nullptr;
};

#endif // LIRI_LIRICOLORPICKER_CLIENT_H
