// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mockliricolorpicker.h"

MockLiriColorPickerManager::MockLiriColorPickerManager(struct ::wl_registry *registry, uint32_t name, uint32_t version)
    : QtWayland::liri_color_picker_manager(registry, name, version)
{
}

MockLiriColorPickerManager::~MockLiriColorPickerManager()
{
    destroy();
}

MockLiriColorPicker *MockLiriColorPickerManager::pickAtLocation(QtWayland::wl_output *output, const QPoint &location)
{
    return new MockLiriColorPicker(pick_at_location(output->object(), location.x(), location.y()));
}

MockLiriColorPicker *MockLiriColorPickerManager::pickInteractively(QtWayland::wl_seat *seat)
{
    return new MockLiriColorPicker(pick_interactively(seat->object()));
}

MockLiriColorPicker::MockLiriColorPicker(struct ::liri_color_picker *object)
    : QtWayland::liri_color_picker(object)
{
}

uint32_t MockLiriColorPicker::lastPickedValue() const
{
    return m_lastValue;
}

void MockLiriColorPicker::liri_color_picker_picked(uint32_t value)
{
    m_lastValue = value;
}
