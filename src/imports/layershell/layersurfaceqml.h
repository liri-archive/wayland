// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LAYERSURFACEQML_H
#define LAYERSURFACEQML_H

#include <QQmlParserStatus>

#include <LiriWaylandClientLayerShell/LayerSurface>

using namespace Liri::WaylandClient;

class LayerSurfaceQml : public LayerSurface, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
public:
    LayerSurfaceQml(QObject *parent = nullptr);

protected:
    void classBegin() override {}
    void componentComplete() override;
};

#endif // LAYERSURFACEQML_H
