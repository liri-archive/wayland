/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2019 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef LIRI_WAYLANDWLRFOREIGNTOPLEVELMANAGEMENTV1QML_H
#define LIRI_WAYLANDWLRFOREIGNTOPLEVELMANAGEMENTV1QML_H

#include <QQmlParserStatus>

#include <LiriWaylandServer/WaylandWlrForeignToplevelManagementV1>

class WaylandWlrForeignToplevelHandleV1Qml : public WaylandWlrForeignToplevelHandleV1, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
public:
    explicit WaylandWlrForeignToplevelHandleV1Qml(QObject *parent = nullptr);

protected:
    void classBegin() override {}
    void componentComplete() override;
};

#endif // LIRI_WAYLANDWLRFOREIGNTOPLEVELMANAGEMENTV1QML_H
