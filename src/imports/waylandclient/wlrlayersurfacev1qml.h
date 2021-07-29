// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef WLRLAYERSURFACEV1QML_H
#define WLRLAYERSURFACEV1QML_H

#include <QQmlParserStatus>

#include <LiriWaylandClient/WlrLayerSurfaceV1>

using namespace Liri::WaylandClient;

class WlrLayerSurfaceV1Qml : public WlrLayerSurfaceV1, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
public:
    WlrLayerSurfaceV1Qml(QObject *parent = nullptr);

protected:
    void classBegin() override {}
    void componentComplete() override;
};

#endif // WLRLAYERSURFACEV1QML_H
