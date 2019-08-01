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

#ifndef LIRI_WLROUTPUTMANAGERV1_P_H
#define LIRI_WLROUTPUTMANAGERV1_P_H

#include <QPoint>
#include <QPointer>
#include <QSize>

#include <LiriWaylandServer/WaylandWlrOutputManagerV1>
#include <LiriWaylandServer/private/qwayland-server-wlr-output-management-unstable-v1.h>

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

class LIRIWAYLANDSERVER_EXPORT WaylandWlrOutputManagerV1Private
        : public QtWaylandServer::zwlr_output_manager_v1
{
    Q_DECLARE_PUBLIC(WaylandWlrOutputManagerV1)
public:
    explicit WaylandWlrOutputManagerV1Private(WaylandWlrOutputManagerV1 *self);
    ~WaylandWlrOutputManagerV1Private();

    void registerHead(WaylandWlrOutputHeadV1 *head);

    static WaylandWlrOutputManagerV1Private *get(WaylandWlrOutputManagerV1 *manager) { return manager->d_func(); }

    bool initialized = false;
    QWaylandCompositor *compositor = nullptr;
    QVector<WaylandWlrOutputHeadV1 *> heads;
    QVector<wl_client *> stoppedClients;
    QMap<quint32, WaylandWlrOutputConfigurationV1 *> configurations;

protected:
    WaylandWlrOutputManagerV1 *q_ptr;

    void zwlr_output_manager_v1_bind_resource(Resource *resource) override;
    void zwlr_output_manager_v1_create_configuration(Resource *resource, uint32_t id, uint32_t serial) override;
    void zwlr_output_manager_v1_stop(Resource *resource) override;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrOutputHeadV1Private
        : public QtWaylandServer::zwlr_output_head_v1
{
    Q_DECLARE_PUBLIC(WaylandWlrOutputHeadV1)
public:
    explicit WaylandWlrOutputHeadV1Private(WaylandWlrOutputHeadV1 *self);
    ~WaylandWlrOutputHeadV1Private();

    void sendInfo(Resource *resource);

    static WaylandWlrOutputHeadV1 *fromResource(wl_resource *resource);

    static WaylandWlrOutputHeadV1Private *get(WaylandWlrOutputHeadV1 *head) { return head->d_func(); }

    WaylandWlrOutputManagerV1 *manager = nullptr;
    bool initialized = false;
    bool modesSent = false;
    QString name;
    bool nameChanged = false;
    QString description;
    bool descriptionChanged = false;
    QSize physicalSize;
    QVector<WaylandWlrOutputModeV1 *> modes;
    QPointer<WaylandWlrOutputModeV1> currentMode;
    QPointer<WaylandWlrOutputModeV1> preferredMode;
    bool enabled = true;
    QPoint position;
    QWaylandOutput::Transform transform = QWaylandOutput::TransformNormal;
    qreal scale = 1;

protected:
    WaylandWlrOutputHeadV1 *q_ptr;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrOutputModeV1Private
        : public QtWaylandServer::zwlr_output_mode_v1
{
    Q_DECLARE_PUBLIC(WaylandWlrOutputModeV1)
public:
    explicit WaylandWlrOutputModeV1Private(WaylandWlrOutputModeV1 *self);
    ~WaylandWlrOutputModeV1Private();

    static WaylandWlrOutputModeV1 *fromResource(wl_resource *resource);

    static WaylandWlrOutputModeV1Private *get(WaylandWlrOutputModeV1 *mode) { return mode->d_func(); }

    bool initialized = false;
    QSize size;
    qint32 refreshRate = -1;

protected:
    WaylandWlrOutputModeV1 *q_ptr;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrOutputConfigurationV1Private
        : public QtWaylandServer::zwlr_output_configuration_v1
{
    Q_DECLARE_PUBLIC(WaylandWlrOutputConfigurationV1)
public:
    explicit WaylandWlrOutputConfigurationV1Private(WaylandWlrOutputConfigurationV1 *self);

    static WaylandWlrOutputConfigurationV1Private *get(WaylandWlrOutputConfigurationV1 *configuration) { return configuration->d_func(); }

    WaylandWlrOutputManagerV1 *manager = nullptr;
    QVector<WaylandWlrOutputConfigurationHeadV1 *> enabledHeads;
    QVector<WaylandWlrOutputHeadV1 *> disabledHeads;
    QVector<WaylandWlrOutputHeadV1 *> configuredHeads;

protected:
    WaylandWlrOutputConfigurationV1 *q_ptr;

    void zwlr_output_configuration_v1_enable_head(Resource *resource, uint32_t id, struct ::wl_resource *headResource) override;
    void zwlr_output_configuration_v1_disable_head(Resource *resource, struct ::wl_resource *headResource) override;
    void zwlr_output_configuration_v1_apply(Resource *resource) override;
    void zwlr_output_configuration_v1_test(Resource *resource) override;
    void zwlr_output_configuration_v1_destroy(Resource *resource) override;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrOutputConfigurationHeadV1Private
        : public QtWaylandServer::zwlr_output_configuration_head_v1
{
    Q_DECLARE_PUBLIC(WaylandWlrOutputConfigurationHeadV1)
public:
    explicit WaylandWlrOutputConfigurationHeadV1Private(WaylandWlrOutputConfigurationHeadV1 *self);

    static WaylandWlrOutputConfigurationHeadV1Private *get(WaylandWlrOutputConfigurationHeadV1 *configuration) { return configuration->d_func(); }

    WaylandWlrOutputHeadV1 *head = nullptr;
    WaylandWlrOutputModeV1 *mode = nullptr;
    QSize customModeSize = QSize(0, 0);
    qint32 customModeRefresh = 0;
    QPoint position;
    QWaylandOutput::Transform transform = QWaylandOutput::TransformNormal;
    qreal scale = 1;

    bool modeChanged = false;
    bool customModeChanged = false;
    bool positionChanged = false;
    bool transformChanged = false;
    bool scaleChanged = false;

protected:
    WaylandWlrOutputConfigurationHeadV1 *q_ptr;

    void zwlr_output_configuration_head_v1_set_mode(Resource *resource, struct ::wl_resource *modeResource) override;
    void zwlr_output_configuration_head_v1_set_custom_mode(Resource *resource, int32_t width, int32_t height, int32_t refresh) override;
    void zwlr_output_configuration_head_v1_set_position(Resource *resource, int32_t x, int32_t y) override;
    void zwlr_output_configuration_head_v1_set_transform(Resource *resource, int32_t wlTransform) override;
    void zwlr_output_configuration_head_v1_set_scale(Resource *resource, wl_fixed_t scaleFixed) override;
};

#endif // LIRI_WLROUTPUTMANAGERV1_P_H
