// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_IRICOLORPICKER_P_CLIENT_H
#define LIRI_IRICOLORPICKER_P_CLIENT_H

#include <QAtomicInteger>

#include <LiriWaylandClient/LiriColorPicker>
#include <LiriWaylandClient/private/qwayland-liri-color-picker.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class LIRIWAYLANDCLIENT_EXPORT LiriColorPickerManagerPrivate
        : public QtWayland::liri_color_picker_manager
{
public:
    LiriColorPickerManagerPrivate() = default;

    QAtomicInteger<quint32> atomicInteger;
};

class LIRIWAYLANDCLIENT_EXPORT LiriColorPicker
        : public QObject
        , public QtWayland::liri_color_picker
{
    Q_OBJECT
public:
    explicit LiriColorPicker(struct ::liri_color_picker *object,
                             QObject *parent = nullptr);

    LiriColorPickerManager *manager = nullptr;

protected:
    void liri_color_picker_picked(uint32_t serial, uint32_t value) override;
};

#endif // LIRI_IRICOLORPICKER_P_CLIENT_H
