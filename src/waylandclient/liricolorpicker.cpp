// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "liricolorpicker_p.h"
#include "logging_p.h"
#include "utils_p.h"

#include <wayland-client.h>

/*
 * LiriColorPickerManagerPrivate
 */

LiriColorPickerManagerPrivate::~LiriColorPickerManagerPrivate()
{
    qDeleteAll(pickers);
}

/*
 * LiriColorPicker
 */

LiriColorPicker::LiriColorPicker(struct ::liri_color_picker *object, QObject *parent)
    : QObject(parent)
    , QtWayland::liri_color_picker(object)
{
}

LiriColorPicker::~LiriColorPicker()
{
    LiriColorPickerManagerPrivate::get(manager)->pickers.removeOne(this);
    destroy();
}

void LiriColorPicker::liri_color_picker_picked(uint32_t value)
{
    auto rgba = static_cast<QRgb>(value);
    Q_EMIT manager->colorPicked(QColor::fromRgba(rgba));

    deleteLater();
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

void LiriColorPickerManager::pickAtLocation(QScreen *screen, const QPoint &location)
{
    Q_D(LiriColorPickerManager);

    auto *object = d->pick_at_location(getWlOutput(screen), location.x(), location.y());
    auto *picker = new LiriColorPicker(object, this);
    picker->manager = this;
    d->pickers.append(picker);
}

void LiriColorPickerManager::pickInteractively()
{
    Q_D(LiriColorPickerManager);

    auto *object = d->pick_interactively(getWlSeat());
    auto *picker = new LiriColorPicker(object, this);
    picker->manager = this;
    d->pickers.append(picker);
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
