// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "wlrlayersurfacev1qml.h"

WlrLayerSurfaceV1Qml::WlrLayerSurfaceV1Qml(QObject *parent)
    : WlrLayerSurfaceV1(parent)
{
}

void WlrLayerSurfaceV1Qml::componentComplete()
{
    if (isInitialized())
        return;

    // Find the window from the parent, if not specified
    if (!window()) {
        for (auto *p = parent(); p != nullptr; p = p->parent()) {
            if (auto *w = qobject_cast<QWindow *>(p)) {
                setWindow(w);
                break;
            }
        }
    }

    // Initialize
    initialize();
}
