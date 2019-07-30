/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2019 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QtQml>
#include <QWaylandQuickExtension>

#include <LiriWaylandServer/GtkShell>
#include <LiriWaylandServer/KdeServerDecoration>
#include <LiriWaylandServer/LiriDecoration>
#include <LiriWaylandServer/ShellHelper>

Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(GtkShell)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(KdeServerDecorationManager)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(LiriDecorationManager)
Q_COMPOSITOR_DECLARE_QUICK_EXTENSION_CLASS(ShellHelper)

class WaylandServerPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char *uri)
    {
        // @uri Liri.WaylandServer
        Q_ASSERT(strcmp(uri, "Liri.WaylandServer") == 0);

        const int versionMajor = 1;
        const int versionMinor = 0;

        qmlRegisterType<GtkShellQuickExtension>(uri, versionMajor, versionMinor, "GtkShell");
        qmlRegisterType<GtkSurface>(uri, versionMajor, versionMinor, "GtkSurface");

        qmlRegisterType<KdeServerDecorationManagerQuickExtension>(uri, versionMajor, versionMinor, "KdeServerDecorationManager");
        qmlRegisterUncreatableType<KdeServerDecoration>(uri, versionMajor, versionMinor, "KdeServerDecoration",
                                                        QStringLiteral("Cannot create instance of KdeServerDecoration"));

        qmlRegisterType<LiriDecorationManagerQuickExtension>(uri, versionMajor, versionMinor, "LiriDecorationManager");
        qmlRegisterUncreatableType<LiriDecoration>(uri, versionMajor, versionMinor, "LiriDecoration",
                                                   QStringLiteral("Cannot create instance of LiriDecoration"));

        qmlRegisterType<ShellHelperQuickExtension>(uri, versionMajor, versionMinor, "ShellHelper");
    }
};

#include "plugin.moc"
