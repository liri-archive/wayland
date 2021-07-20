// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QtQml>

#include "layersurfaceqml.h"

class LayerShellPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char *uri) override
    {
        // @uri Liri.WaylandClient.LayerShell
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Liri.WaylandClient.LayerShell"));

        const int versionMajor = 1;
        const int versionMinor = 0;

        qmlRegisterType<LayerSurfaceQml>(uri, versionMajor, versionMinor, "LayerSurface");
    }
};

#include "plugin.moc"
