// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MOCKLIRICOLORPICKER_H
#define MOCKLIRICOLORPICKER_H

#include <QPoint>

#include "qwayland-wayland.h"
#include "qwayland-liri-color-picker.h"

class MockLiriColorPicker : public QtWayland::liri_color_picker
{
public:
    MockLiriColorPicker(struct ::liri_color_picker *object);

    uint32_t lastPickedValue() const;

protected:
    void liri_color_picker_picked(uint32_t value) override;

private:
    uint32_t m_lastValue = 0;
};

class MockLiriColorPickerManager : public QtWayland::liri_color_picker_manager
{
public:
    MockLiriColorPickerManager(struct ::wl_registry *registry, uint32_t name, uint32_t version);
    ~MockLiriColorPickerManager();

    MockLiriColorPicker *pickAtLocation(QtWayland::wl_output *output, const QPoint &location);
    MockLiriColorPicker *pickInteractively(QtWayland::wl_seat *seat);
};

#endif // MOCKLIRICOLORPICKER_H
