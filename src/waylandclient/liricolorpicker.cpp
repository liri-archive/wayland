// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "liricolorpicker_p.h"
#include "logging_p.h"
#include "utils_p.h"

#include <wayland-client.h>

// lcColorPickerClient

/*
 * LiriColorPicker
 */

LiriColorPicker::LiriColorPicker(struct ::liri_color_picker *object, QObject *parent)
    : QObject(parent)
    , QtWayland::liri_color_picker(object)
{
}

void LiriColorPicker::liri_color_picker_picked(uint32_t serial, uint32_t value)
{
    auto rgba = static_cast<QRgb>(value);
    Q_EMIT manager->colorPicked(serial, QColor::fromRgba(rgba));
}

/*
 * LiriColorPickerManager
 */

LiriColorPickerManager::LiriColorPickerManager()
    : QWaylandClientExtensionTemplate<LiriColorPickerManager>(1)
    , d_ptr(new LiriColorPickerManagerPrivate)
{
}

LiriColorPickerManager::~LiriColorPickerManager()
{
    delete d_ptr;
}

quint32 LiriColorPickerManager::pickAtLocation(QScreen *screen, const QPoint &location)
{
    Q_D(LiriColorPickerManager);

    auto serial = d->atomicInteger++;
    auto *picker = new LiriColorPicker(d->create_picker(getWlOutput(screen)), this);
    picker->manager = this;
    picker->pick_at_location(serial, location.x(), location.y());

    return serial;
}

quint32 LiriColorPickerManager::pickInteractively(QScreen *screen)
{
    Q_D(LiriColorPickerManager);

    auto serial = d->atomicInteger++;
    auto *picker = new LiriColorPicker(d->create_picker(getWlOutput(screen)), this);
    picker->manager = this;
    picker->pick_interactively(getWlSeat(), serial);

    return serial;
}

void LiriColorPickerManager::init(struct ::wl_registry *registry, int id, int version)
{
    Q_D(LiriColorPickerManager);
    d->init(registry, id, version);
}

const struct ::wl_interface *LiriColorPickerManager::interface()
{
    return LiriColorPickerManagerPrivate::interface();
}
