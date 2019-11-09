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

#include <QLoggingCategory>

#include "waylandwlroutputmanagementv1qml.h"

Q_DECLARE_LOGGING_CATEGORY(lcOutputManagement)
Q_LOGGING_CATEGORY(lcOutputManagement, "liri.waylandserver.outputmanagement", QtInfoMsg)

WaylandWlrOutputHeadV1Qml::WaylandWlrOutputHeadV1Qml(QObject *parent)
    : WaylandWlrOutputHeadV1(parent)
{
}

QQmlListProperty<WaylandWlrOutputModeV1> WaylandWlrOutputHeadV1Qml::modesList()
{
    auto countFunc = [](QQmlListProperty<WaylandWlrOutputModeV1> *prop) {
        return static_cast<WaylandWlrOutputHeadV1Qml *>(prop->object)->modes().size();
    };
    auto atFunc = [](QQmlListProperty<WaylandWlrOutputModeV1> *prop, int index) {
        return static_cast<WaylandWlrOutputHeadV1Qml *>(prop->object)->modes().at(index);
    };
    return QQmlListProperty<WaylandWlrOutputModeV1>(this, this, countFunc, atFunc);
}

void WaylandWlrOutputHeadV1Qml::componentComplete()
{
    if (!isInitialized()) {
        initialize();

        if (!isInitialized())
            qCWarning(lcOutputManagement,
                      "Unable to find WlrOutputManagerV1: %p head will not be registered",
                      this);
    }
}


WaylandWlrOutputConfigurationV1Qml::WaylandWlrOutputConfigurationV1Qml(QObject *parent)
    : WaylandWlrOutputConfigurationV1(parent)
{
}

QQmlListProperty<WaylandWlrOutputConfigurationHeadV1> WaylandWlrOutputConfigurationV1Qml::enabledHeadsList()
{
    auto countFunc = [](QQmlListProperty<WaylandWlrOutputConfigurationHeadV1> *prop) {
        return static_cast<WaylandWlrOutputConfigurationV1Qml *>(prop->object)->enabledHeads().size();
    };
    auto atFunc = [](QQmlListProperty<WaylandWlrOutputConfigurationHeadV1> *prop, int index) {
        return static_cast<WaylandWlrOutputConfigurationV1Qml *>(prop->object)->enabledHeads().at(index);
    };
    return QQmlListProperty<WaylandWlrOutputConfigurationHeadV1>(this, this, countFunc, atFunc);
}

QQmlListProperty<WaylandWlrOutputHeadV1> WaylandWlrOutputConfigurationV1Qml::disabledHeadsList()
{
    auto countFunc = [](QQmlListProperty<WaylandWlrOutputHeadV1> *prop) {
        return static_cast<WaylandWlrOutputConfigurationV1Qml *>(prop->object)->disabledHeads().size();
    };
    auto atFunc = [](QQmlListProperty<WaylandWlrOutputHeadV1> *prop, int index) {
        return static_cast<WaylandWlrOutputConfigurationV1Qml *>(prop->object)->disabledHeads().at(index);
    };
    return QQmlListProperty<WaylandWlrOutputHeadV1>(this, this, countFunc, atFunc);
}
