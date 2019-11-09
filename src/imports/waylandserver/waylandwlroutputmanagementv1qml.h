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

#ifndef LIRI_WAYLANDWLROUTPUTMANAGEMENTV1QML_H
#define LIRI_WAYLANDWLROUTPUTMANAGEMENTV1QML_H

#include <QQmlComponent>
#include <QQmlParserStatus>
#include <QQmlListProperty>

#include <LiriWaylandServer/WaylandWlrOutputManagerV1>

class WaylandWlrOutputHeadV1Qml
        : public WaylandWlrOutputHeadV1, public QQmlParserStatus
{
    Q_OBJECT
    Q_WAYLAND_COMPOSITOR_DECLARE_QUICK_CHILDREN(WaylandWlrOutputHeadV1Qml)
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QQmlListProperty<WaylandWlrOutputModeV1> modes READ modesList NOTIFY modesChanged)
public:
    explicit WaylandWlrOutputHeadV1Qml(QObject *parent = nullptr);

    QQmlListProperty<WaylandWlrOutputModeV1> modesList();

protected:
    void classBegin() override {}
    void componentComplete() override;
};

class WaylandWlrOutputConfigurationV1Qml : public WaylandWlrOutputConfigurationV1
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<WaylandWlrOutputConfigurationHeadV1> enabledHeads READ enabledHeadsList NOTIFY enabledHeadsChanged)
    Q_PROPERTY(QQmlListProperty<WaylandWlrOutputHeadV1> disabledHeads READ disabledHeadsList NOTIFY disabledHeadsChanged)
public:
    explicit WaylandWlrOutputConfigurationV1Qml(QObject *parent = nullptr);

    QQmlListProperty<WaylandWlrOutputConfigurationHeadV1> enabledHeadsList();
    QQmlListProperty<WaylandWlrOutputHeadV1> disabledHeadsList();
};

QML_DECLARE_TYPE(WaylandWlrOutputConfigurationHeadV1)
QML_DECLARE_TYPE(WaylandWlrOutputHeadV1Qml)
QML_DECLARE_TYPE(WaylandWlrOutputModeV1)

#endif // LIRI_WAYLANDWLROUTPUTMANAGEMENTV1QML_H
