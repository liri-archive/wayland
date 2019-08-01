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

#include "mockwlroutputmanagementv1.h"

MockWlrOutputManagerV1::MockWlrOutputManagerV1(struct ::wl_registry *registry, uint32_t name, uint32_t version)
    : QtWayland::zwlr_output_manager_v1(registry, name, version)
{
}

MockWlrOutputConfigurationV1 *MockWlrOutputManagerV1::createConfiguration()
{
    return new MockWlrOutputConfigurationV1(create_configuration(lastSerial));
}

void MockWlrOutputManagerV1::zwlr_output_manager_v1_head(struct ::zwlr_output_head_v1 *head)
{
    pending.heads.append(new MockWlrOutputHeadV1(head));
}

void MockWlrOutputManagerV1::zwlr_output_manager_v1_done(uint32_t serial)
{
    for (auto *head : qAsConst(pending.heads)) {
        actual.heads.append(head);
    }

    lastSerial = serial;
}

void MockWlrOutputManagerV1::zwlr_output_manager_v1_finished()
{
    delete this;
}

MockWlrOutputHeadV1::MockWlrOutputHeadV1(struct ::zwlr_output_head_v1 *object)
    : QtWayland::zwlr_output_head_v1(object)
{
}

void MockWlrOutputHeadV1::zwlr_output_head_v1_name(const QString &name)
{
    this->name = name;
}

void MockWlrOutputHeadV1::zwlr_output_head_v1_description(const QString &description)
{
    this->description = description;
}

void MockWlrOutputHeadV1::zwlr_output_head_v1_physical_size(int32_t width, int32_t height)
{
    this->physicalSize = QSize(width, height);
}

void MockWlrOutputHeadV1::zwlr_output_head_v1_mode(struct ::zwlr_output_mode_v1 *mode)
{
    this->modes.append(new MockWlrOutputModeV1(mode));
}

void MockWlrOutputHeadV1::zwlr_output_head_v1_enabled(int32_t enabled)
{
    this->enabled = enabled > 0;
}

void MockWlrOutputHeadV1::zwlr_output_head_v1_current_mode(struct ::zwlr_output_mode_v1 *mode)
{
    for (auto *mockMode : qAsConst(this->modes)) {
        if (mockMode->object() == mode) {
            this->currentMode = mockMode;
            return;
        }
    }

    auto *mockMode = new MockWlrOutputModeV1(mode);
    this->modes.append(mockMode);
    this->currentMode = mockMode;
}

void MockWlrOutputHeadV1::zwlr_output_head_v1_position(int32_t x, int32_t y)
{
    this->position = QPoint(x, y);
}

void MockWlrOutputHeadV1::zwlr_output_head_v1_transform(int32_t transform)
{
    this->transform = static_cast<Transform>(transform);
}

void MockWlrOutputHeadV1::zwlr_output_head_v1_scale(wl_fixed_t scale)
{
    this->scale = wl_fixed_to_double(scale);
}

void MockWlrOutputHeadV1::zwlr_output_head_v1_finished()
{
    delete this;
}

MockWlrOutputModeV1::MockWlrOutputModeV1(struct ::zwlr_output_mode_v1 *object)
    : QtWayland::zwlr_output_mode_v1(object)
{
}

void MockWlrOutputModeV1::zwlr_output_mode_v1_size(int32_t width, int32_t height)
{
    this->size = QSize(width, height);
}

void MockWlrOutputModeV1::zwlr_output_mode_v1_refresh(int32_t refresh)
{
    this->refresh = refresh;
}

void MockWlrOutputModeV1::zwlr_output_mode_v1_preferred()
{
    this->preferred = true;
}

void MockWlrOutputModeV1::zwlr_output_mode_v1_finished()
{
    delete this;
}

MockWlrOutputConfigurationV1::MockWlrOutputConfigurationV1(struct ::zwlr_output_configuration_v1 *object)
    : QtWayland::zwlr_output_configuration_v1(object)
{
}

MockWlrOutputConfigurationHeadV1 *MockWlrOutputConfigurationV1::enableHead(MockWlrOutputHeadV1 *head)
{
    return new MockWlrOutputConfigurationHeadV1(enable_head(head->object()));
}

void MockWlrOutputConfigurationV1::disableHead(MockWlrOutputHeadV1 *head)
{
    disable_head(head->object());
}

void MockWlrOutputConfigurationV1::zwlr_output_configuration_v1_succeeded()
{
    succeeded = true;
}

void MockWlrOutputConfigurationV1::zwlr_output_configuration_v1_failed()
{
    failed = true;
}

void MockWlrOutputConfigurationV1::zwlr_output_configuration_v1_cancelled()
{
    cancelled = true;
}

MockWlrOutputConfigurationHeadV1::MockWlrOutputConfigurationHeadV1(struct ::zwlr_output_configuration_head_v1 *object)
    : QtWayland::zwlr_output_configuration_head_v1(object)
{
}

void MockWlrOutputConfigurationHeadV1::setMode(MockWlrOutputModeV1 *mode)
{
    set_mode(mode->object());
}

void MockWlrOutputConfigurationHeadV1::setCustomMode(const QSize &size, qint32 refresh)
{
    set_custom_mode(size.width(), size.height(), refresh);
}

void MockWlrOutputConfigurationHeadV1::setPosition(const QPoint &pos)
{
    set_position(pos.x(), pos.y());
}

void MockWlrOutputConfigurationHeadV1::setTransform(MockWlrOutputHeadV1::Transform transform)
{
    set_transform(static_cast<int32_t>(transform));
}

void MockWlrOutputConfigurationHeadV1::setScale(qreal scale)
{
    set_scale(wl_fixed_from_double(scale));
}
