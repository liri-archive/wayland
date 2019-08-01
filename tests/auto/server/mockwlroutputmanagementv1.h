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

#ifndef MOCKWLROUTPUTMANAGEMENTV1_H
#define MOCKWLROUTPUTMANAGEMENTV1_H

#include <QPoint>
#include <QSize>
#include <QVector>

#include "qwayland-wlr-output-management-unstable-v1.h"

class MockWlrOutputHeadV1;
class MockWlrOutputModeV1;

class MockWlrOutputManagerV1 : public QtWayland::zwlr_output_manager_v1
{
public:
    MockWlrOutputManagerV1(struct ::wl_registry *registry, uint32_t name, uint32_t version);

    class MockWlrOutputConfigurationV1 *createConfiguration();

    quint32 lastSerial = 0;

    struct {
        QVector<MockWlrOutputHeadV1 *> heads;
    } pending;

    struct {
        QVector<MockWlrOutputHeadV1 *> heads;
    } actual;

protected:
    void zwlr_output_manager_v1_head(struct ::zwlr_output_head_v1 *head) override;
    void zwlr_output_manager_v1_done(uint32_t serial) override;
    void zwlr_output_manager_v1_finished() override;
};

class MockWlrOutputHeadV1 : public QtWayland::zwlr_output_head_v1
{
public:
    enum Transform {
        TransformNormal = 0,
        Transform90,
        Transform180,
        Transform270,
        TransformFlipped,
        TransformFlipped90,
        TransformFlipped180,
        TransformFlipped270
    };

    MockWlrOutputHeadV1(struct ::zwlr_output_head_v1 *object);

    bool enabled = false;
    QString name;
    QString description;
    QSize physicalSize;
    QPoint position;
    Transform transform = TransformNormal;
    qreal scale;
    QVector<MockWlrOutputModeV1 *> modes;
    MockWlrOutputModeV1 *currentMode = nullptr;

protected:
    void zwlr_output_head_v1_name(const QString &name) override;
    void zwlr_output_head_v1_description(const QString &description) override;
    void zwlr_output_head_v1_physical_size(int32_t width, int32_t height) override;
    void zwlr_output_head_v1_mode(struct ::zwlr_output_mode_v1 *mode) override;
    void zwlr_output_head_v1_enabled(int32_t enabled) override;
    void zwlr_output_head_v1_current_mode(struct ::zwlr_output_mode_v1 *mode) override;
    void zwlr_output_head_v1_position(int32_t x, int32_t y) override;
    void zwlr_output_head_v1_transform(int32_t transform) override;
    void zwlr_output_head_v1_scale(wl_fixed_t scale) override;
    void zwlr_output_head_v1_finished() override;
};

class MockWlrOutputModeV1 : public QtWayland::zwlr_output_mode_v1
{
public:
    MockWlrOutputModeV1(struct ::zwlr_output_mode_v1 *object);

    QSize size;
    qint32 refresh;
    bool preferred = false;

protected:
    void zwlr_output_mode_v1_size(int32_t width, int32_t height) override;
    void zwlr_output_mode_v1_refresh(int32_t refresh) override;
    void zwlr_output_mode_v1_preferred() override;
    void zwlr_output_mode_v1_finished() override;
};

class MockWlrOutputConfigurationV1 : public QtWayland::zwlr_output_configuration_v1
{
public:
    MockWlrOutputConfigurationV1(struct ::zwlr_output_configuration_v1 *object);

    bool succeeded = false;
    bool failed = false;
    bool cancelled = false;

    class MockWlrOutputConfigurationHeadV1 *enableHead(MockWlrOutputHeadV1 *head);
    void disableHead(MockWlrOutputHeadV1 *head);

protected:
    void zwlr_output_configuration_v1_succeeded() override;
    void zwlr_output_configuration_v1_failed() override;
    void zwlr_output_configuration_v1_cancelled() override;
};

class MockWlrOutputConfigurationHeadV1 : public QtWayland::zwlr_output_configuration_head_v1
{
public:
    MockWlrOutputConfigurationHeadV1(struct ::zwlr_output_configuration_head_v1 *object);

    void setMode(MockWlrOutputModeV1 *mode);
    void setCustomMode(const QSize &size, qint32 refresh);
    void setPosition(const QPoint &pos);
    void setTransform(MockWlrOutputHeadV1::Transform transform);
    void setScale(qreal scale);
};

#endif // MOCKWLROUTPUTMANAGEMENTV1_H
