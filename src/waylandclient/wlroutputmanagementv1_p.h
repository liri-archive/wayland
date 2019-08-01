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

#ifndef LIRI_WLROUTPUTMANAGEMENTV1_P_CLIENT_H
#define LIRI_WLROUTPUTMANAGEMENTV1_P_CLIENT_H

#include <QPointer>
#include <QVector>

#include "wlroutputmanagementv1.h"
#include "qwayland-wlr-output-management-unstable-v1.h"

class WlrOutputManagerV1Private : public QtWayland::zwlr_output_manager_v1
{
    Q_DECLARE_PUBLIC(WlrOutputManagerV1)
public:
    explicit WlrOutputManagerV1Private(WlrOutputManagerV1 *self);

protected:
    WlrOutputManagerV1 *q_ptr;

    void zwlr_output_manager_v1_head(struct ::zwlr_output_head_v1 *headObject) override;
    void zwlr_output_manager_v1_done(uint32_t serial) override;
    void zwlr_output_manager_v1_finished() override;

private:
    QVector<WlrOutputHeadV1 *> heads;
    QVector<WlrOutputHeadV1 *> pendingHeads;
    quint32 lastSerial;
};

class WlrOutputHeadV1Private : public QtWayland::zwlr_output_head_v1
{
    Q_DECLARE_PUBLIC(WlrOutputHeadV1)
public:
    explicit WlrOutputHeadV1Private(WlrOutputHeadV1 *self);

    static WlrOutputHeadV1Private *get(WlrOutputHeadV1 *head) { return head->d_func(); }

    QString name;
    QString description;
    QSize physicalSize;
    bool enabled = false;
    QPoint position;
    WlrOutputHeadV1::Transform transform = WlrOutputHeadV1::TransformNormal;
    qreal scale = 1;
    QVector<WlrOutputModeV1 *> pendingModes;
    QVector<WlrOutputModeV1 *> modes;
    QPointer<WlrOutputModeV1> currentModePending;
    QPointer<WlrOutputModeV1> currentMode;
    QPointer<WlrOutputModeV1> preferredMode;

protected:
    WlrOutputHeadV1 *q_ptr;

    void zwlr_output_head_v1_name(const QString &name) override;
    void zwlr_output_head_v1_description(const QString &description) override;
    void zwlr_output_head_v1_physical_size(int32_t width, int32_t height) override;
    void zwlr_output_head_v1_mode(struct ::zwlr_output_mode_v1 *object) override;
    void zwlr_output_head_v1_enabled(int32_t enabled) override;
    void zwlr_output_head_v1_current_mode(struct ::zwlr_output_mode_v1 *object) override;
    void zwlr_output_head_v1_position(int32_t x, int32_t y) override;
    void zwlr_output_head_v1_transform(int32_t transform) override;
    void zwlr_output_head_v1_scale(wl_fixed_t scale) override;
    void zwlr_output_head_v1_finished() override;

private:
    friend class WlrOutputModeV1Private;
};

class WlrOutputModeV1Private : public QtWayland::zwlr_output_mode_v1
{
    Q_DECLARE_PUBLIC(WlrOutputModeV1)
public:
    explicit WlrOutputModeV1Private(WlrOutputModeV1 *self);

    static WlrOutputModeV1Private *get(WlrOutputModeV1 *mode) { return mode->d_func(); }

    WlrOutputHeadV1 *head = nullptr;
    QSize size;
    qint32 refresh = 0;

protected:
    WlrOutputModeV1 *q_ptr;

    void zwlr_output_mode_v1_size(int32_t width, int32_t height) override;
    void zwlr_output_mode_v1_refresh(int32_t refresh) override;
    void zwlr_output_mode_v1_preferred() override;
    void zwlr_output_mode_v1_finished() override;
};

class WlrOutputConfigurationV1Private : public QtWayland::zwlr_output_configuration_v1
{
    Q_DECLARE_PUBLIC(WlrOutputConfigurationV1)
public:
    explicit WlrOutputConfigurationV1Private(WlrOutputConfigurationV1 *self);

    quint32 serial = 0;

protected:
    WlrOutputConfigurationV1 *q_ptr;

    void zwlr_output_configuration_v1_succeeded() override;
    void zwlr_output_configuration_v1_failed() override;
    void zwlr_output_configuration_v1_cancelled() override;
};

class WlrOutputConfigurationHeadV1Private : public QtWayland::zwlr_output_configuration_head_v1
{
    Q_DECLARE_PUBLIC(WlrOutputConfigurationHeadV1)
public:
    explicit WlrOutputConfigurationHeadV1Private(WlrOutputConfigurationHeadV1 *self);

    static WlrOutputConfigurationHeadV1Private *get(WlrOutputConfigurationHeadV1 *changes) { return changes->d_func(); }

    WlrOutputModeV1 *mode = nullptr;
    WlrOutputModeV1 *customMode = nullptr;
    QPoint position;
    qreal scale = 1;
    WlrOutputHeadV1::Transform transform = WlrOutputHeadV1::TransformNormal;

protected:
    WlrOutputConfigurationHeadV1 *q_ptr;
};

#endif // LIRI_WLROUTPUTMANAGEMENTV1_P_CLIENT_H
