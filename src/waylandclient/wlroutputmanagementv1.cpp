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

#include "wlroutputmanagementv1_p.h"

WlrOutputManagerV1Private::WlrOutputManagerV1Private(WlrOutputManagerV1 *self)
    : QtWayland::zwlr_output_manager_v1()
    , q_ptr(self)
{
}

void WlrOutputManagerV1Private::zwlr_output_manager_v1_head(zwlr_output_head_v1 *object)
{
    Q_Q(WlrOutputManagerV1);

    auto head = new WlrOutputHeadV1(q);
    head->d_func()->init(object);
    pendingHeads.append(head);
}

void WlrOutputManagerV1Private::zwlr_output_manager_v1_done(uint32_t serial)
{
    Q_Q(WlrOutputManagerV1);

    for (auto head : qAsConst(pendingHeads)) {
        heads.append(head);
        Q_EMIT q->headAdded(head);
        Q_EMIT q->headsChanged();

        auto headPrivate = WlrOutputHeadV1Private::get(head);
        for (auto mode : qAsConst(headPrivate->pendingModes)) {
            headPrivate->modes.append(mode);
            Q_EMIT head->modeAdded(mode);
            Q_EMIT head->modesChanged();
        }
        headPrivate->pendingModes.clear();

        headPrivate->currentMode = headPrivate->currentModePending;
        if (headPrivate->currentMode)
            Q_EMIT head->currentModeChanged(headPrivate->currentMode);
    }
    pendingHeads.clear();

    lastSerial = serial;
}

void WlrOutputManagerV1Private::zwlr_output_manager_v1_finished()
{
}


WlrOutputManagerV1::WlrOutputManagerV1()
    : QWaylandClientExtensionTemplate(1)
    , d_ptr(new WlrOutputManagerV1Private(this))
{
}

WlrOutputManagerV1::~WlrOutputManagerV1()
{
    delete d_ptr;
}

void WlrOutputManagerV1::init(wl_registry *registry, int id, int version)
{
    Q_D(WlrOutputManagerV1);
    d->init(registry, id, version);
}

QVector<WlrOutputHeadV1 *> WlrOutputManagerV1::heads() const
{
    Q_D(const WlrOutputManagerV1);
    return d->heads;
}

QQmlListProperty<WlrOutputHeadV1> WlrOutputManagerV1::headsList()
{
    return QQmlListProperty<WlrOutputHeadV1>(this, this,
                                             &WlrOutputManagerV1::headsCount,
                                             &WlrOutputManagerV1::getHead);
}

WlrOutputConfigurationV1 *WlrOutputManagerV1::createConfiguration()
{
    Q_D(WlrOutputManagerV1);

    auto configuration = new WlrOutputConfigurationV1(d->lastSerial, this);
    configuration->d_func()->init(d->create_configuration(d->lastSerial));
    return configuration;
}

void WlrOutputManagerV1::stop()
{
    Q_D(WlrOutputManagerV1);
    d->stop();
}

const wl_interface *WlrOutputManagerV1::interface()
{
    return WlrOutputManagerV1Private::interface();
}

int WlrOutputManagerV1::headsCount(QQmlListProperty<WlrOutputHeadV1> *prop)
{
    auto *manager = reinterpret_cast<WlrOutputManagerV1 *>(prop->data);
    return manager->d_func()->heads.count();
}

WlrOutputHeadV1 *WlrOutputManagerV1::getHead(QQmlListProperty<WlrOutputHeadV1> *prop, int index)
{
    auto *manager = reinterpret_cast<WlrOutputManagerV1 *>(prop->data);
    return manager->d_func()->heads.at(index);
}


WlrOutputHeadV1Private::WlrOutputHeadV1Private(WlrOutputHeadV1 *self)
    : QtWayland::zwlr_output_head_v1()
    , q_ptr(self)
{
}

void WlrOutputHeadV1Private::zwlr_output_head_v1_name(const QString &name)
{
    Q_Q(WlrOutputHeadV1);

    this->name = name;
    Q_EMIT q->nameChanged();
}

void WlrOutputHeadV1Private::zwlr_output_head_v1_description(const QString &description)
{
    Q_Q(WlrOutputHeadV1);

    this->description = description;
    Q_EMIT q->descriptionChanged();
}

void WlrOutputHeadV1Private::zwlr_output_head_v1_physical_size(int32_t width, int32_t height)
{
    Q_Q(WlrOutputHeadV1);

    this->physicalSize = QSize(width, height);
    Q_EMIT q->physicalSizeChanged();
}

void WlrOutputHeadV1Private::zwlr_output_head_v1_mode(zwlr_output_mode_v1 *object)
{
    Q_Q(WlrOutputHeadV1);

    auto mode = new WlrOutputModeV1(q);
    mode->d_func()->head = q;
    mode->d_func()->init(object);
    pendingModes.append(mode);
}

void WlrOutputHeadV1Private::zwlr_output_head_v1_enabled(int32_t enabled)
{
    Q_Q(WlrOutputHeadV1);

    this->enabled = enabled;
    Q_EMIT q->enabledChanged();
}

void WlrOutputHeadV1Private::zwlr_output_head_v1_current_mode(zwlr_output_mode_v1 *object)
{
    Q_Q(WlrOutputHeadV1);

    for (auto mode : qAsConst(pendingModes)) {
        if (mode->d_func()->object() == object) {
            currentModePending = mode;
            break;
        }
    }
}

void WlrOutputHeadV1Private::zwlr_output_head_v1_position(int32_t x, int32_t y)
{
    Q_Q(WlrOutputHeadV1);

    this->position = QPoint(x, y);
    Q_EMIT q->positionChanged();
}

void WlrOutputHeadV1Private::zwlr_output_head_v1_transform(int32_t transform)
{
    Q_Q(WlrOutputHeadV1);

    this->transform = static_cast<WlrOutputHeadV1::Transform>(transform);
    Q_EMIT q->transformChanged();
}

void WlrOutputHeadV1Private::zwlr_output_head_v1_scale(wl_fixed_t scale)
{
    Q_Q(WlrOutputHeadV1);

    this->scale = wl_fixed_to_double(scale);
    Q_EMIT q->scaleChanged();
}

void WlrOutputHeadV1Private::zwlr_output_head_v1_finished()
{

}


WlrOutputHeadV1::WlrOutputHeadV1(QObject *parent)
    : QObject(parent)
    , d_ptr(new WlrOutputHeadV1Private(this))
{
    qRegisterMetaType<Transform>("Transform");
}

WlrOutputHeadV1::~WlrOutputHeadV1()
{
    delete d_ptr;
}

QString WlrOutputHeadV1::name() const
{
    Q_D(const WlrOutputHeadV1);
    return d->name;
}

QString WlrOutputHeadV1::description() const
{
    Q_D(const WlrOutputHeadV1);
    return d->description;
}

QSize WlrOutputHeadV1::physicalSize() const
{
    Q_D(const WlrOutputHeadV1);
    return d->physicalSize;
}

bool WlrOutputHeadV1::isEnabled() const
{
    Q_D(const WlrOutputHeadV1);
    return d->enabled;
}

QPoint WlrOutputHeadV1::position() const
{
    Q_D(const WlrOutputHeadV1);
    return d->position;
}

WlrOutputHeadV1::Transform WlrOutputHeadV1::transform() const
{
    Q_D(const WlrOutputHeadV1);
    return d->transform;
}

qreal WlrOutputHeadV1::scale() const
{
    Q_D(const WlrOutputHeadV1);
    return d->scale;
}

QVector<WlrOutputModeV1 *> WlrOutputHeadV1::modes() const
{
    Q_D(const WlrOutputHeadV1);
    return d->modes;
}

QQmlListProperty<WlrOutputModeV1> WlrOutputHeadV1::modesList()
{
    auto countFunc = [](QQmlListProperty<WlrOutputModeV1> *prop) {
        return static_cast<WlrOutputHeadV1 *>(prop->object)->modes().count();
    };
    auto atFunc = [](QQmlListProperty<WlrOutputModeV1> *prop, int index) {
        return static_cast<WlrOutputHeadV1 *>(prop->object)->modes().at(index);
    };
    return QQmlListProperty<WlrOutputModeV1>(this, this, countFunc, atFunc);
}

WlrOutputModeV1 *WlrOutputHeadV1::currentMode() const
{
    Q_D(const WlrOutputHeadV1);
    return d->currentMode;
}

WlrOutputModeV1 *WlrOutputHeadV1::preferredMode() const
{
    Q_D(const WlrOutputHeadV1);
    return d->preferredMode;
}


WlrOutputModeV1Private::WlrOutputModeV1Private(WlrOutputModeV1 *self)
    : QtWayland::zwlr_output_mode_v1()
    , q_ptr(self)
{
}

void WlrOutputModeV1Private::zwlr_output_mode_v1_size(int32_t width, int32_t height)
{
    Q_Q(WlrOutputModeV1);

    this->size = QSize(width, height);
    Q_EMIT q->sizeChanged();
    Q_EMIT q->nameChanged();
}

void WlrOutputModeV1Private::zwlr_output_mode_v1_refresh(int32_t refresh)
{
    Q_Q(WlrOutputModeV1);

    this->refresh = refresh;
    Q_EMIT q->refreshChanged();
    Q_EMIT q->nameChanged();
}

void WlrOutputModeV1Private::zwlr_output_mode_v1_preferred()
{
    Q_Q(WlrOutputModeV1);

    auto headPrivate = WlrOutputHeadV1Private::get(head);
    headPrivate->preferredMode = q;
    Q_EMIT head->preferredModeChanged(q);
}

void WlrOutputModeV1Private::zwlr_output_mode_v1_finished()
{

}


WlrOutputModeV1::WlrOutputModeV1(QObject *parent)
    : QObject(parent)
    , d_ptr(new WlrOutputModeV1Private(this))
{
}

WlrOutputModeV1::~WlrOutputModeV1()
{
    delete d_ptr;
}

QSize WlrOutputModeV1::size() const
{
    Q_D(const WlrOutputModeV1);
    return d->size;
}

qint32 WlrOutputModeV1::refresh() const
{
    Q_D(const WlrOutputModeV1);
    return d->refresh;
}

QString WlrOutputModeV1::name() const
{
    Q_D(const WlrOutputModeV1);

    if (!d->size.isValid() || d->refresh < 1)
        return tr("Unknown mode");
    return tr("%1 x %2 (%3 mHz)").arg(d->size.width(), d->size.height(), d->refresh);
}


WlrOutputConfigurationV1Private::WlrOutputConfigurationV1Private(WlrOutputConfigurationV1 *self)
    : QtWayland::zwlr_output_configuration_v1()
    , q_ptr(self)
{
}

void WlrOutputConfigurationV1Private::zwlr_output_configuration_v1_succeeded()
{
    Q_Q(WlrOutputConfigurationV1);
    Q_EMIT q->succeeded();
}

void WlrOutputConfigurationV1Private::zwlr_output_configuration_v1_failed()
{
    Q_Q(WlrOutputConfigurationV1);
    Q_EMIT q->failed();
}

void WlrOutputConfigurationV1Private::zwlr_output_configuration_v1_cancelled()
{
    Q_Q(WlrOutputConfigurationV1);
    Q_EMIT q->cancelled();
}


WlrOutputConfigurationV1::WlrOutputConfigurationV1(quint32 serial, QObject *parent)
    : QObject(parent)
    , d_ptr(new WlrOutputConfigurationV1Private(this))
{
    d_ptr->serial = serial;
}

WlrOutputConfigurationV1::~WlrOutputConfigurationV1()
{
    delete d_ptr;
}

quint32 WlrOutputConfigurationV1::serial() const
{
    Q_D(const WlrOutputConfigurationV1);
    return d->serial;
}

WlrOutputConfigurationHeadV1 *WlrOutputConfigurationV1::enableHead(WlrOutputHeadV1 *head)
{
    Q_D(WlrOutputConfigurationV1);

    auto changesObject = d->enable_head(WlrOutputHeadV1Private::get(head)->object());
    auto changes = new WlrOutputConfigurationHeadV1(this);
    WlrOutputConfigurationHeadV1Private::get(changes)->init(changesObject);
    return changes;
}

void WlrOutputConfigurationV1::disableHead(WlrOutputHeadV1 *head)
{
    Q_D(WlrOutputConfigurationV1);
    d->disable_head(WlrOutputHeadV1Private::get(head)->object());
}

void WlrOutputConfigurationV1::apply()
{
    Q_D(WlrOutputConfigurationV1);
    d->apply();
}

void WlrOutputConfigurationV1::test()
{
    Q_D(WlrOutputConfigurationV1);
    d->test();
}

void WlrOutputConfigurationV1::destroy()
{
    Q_D(WlrOutputConfigurationV1);
    d->destroy();
}


WlrOutputConfigurationHeadV1Private::WlrOutputConfigurationHeadV1Private(WlrOutputConfigurationHeadV1 *self)
    : QtWayland::zwlr_output_configuration_head_v1()
    , q_ptr(self)
{
}


WlrOutputConfigurationHeadV1::WlrOutputConfigurationHeadV1(QObject *parent)
    : QObject(parent)
    , d_ptr(new WlrOutputConfigurationHeadV1Private(this))
{
}

WlrOutputConfigurationHeadV1::~WlrOutputConfigurationHeadV1()
{
    delete d_ptr;
}

WlrOutputModeV1 *WlrOutputConfigurationHeadV1::mode() const
{
    Q_D(const WlrOutputConfigurationHeadV1);
    return d->mode;
}

void WlrOutputConfigurationHeadV1::setMode(WlrOutputModeV1 *mode)
{
    Q_D(WlrOutputConfigurationHeadV1);

    d->mode = mode;
    Q_EMIT modeChanged();

    d->set_mode(WlrOutputModeV1Private::get(mode)->object());
}

WlrOutputModeV1 *WlrOutputConfigurationHeadV1::customMode() const
{
    Q_D(const WlrOutputConfigurationHeadV1);
    return d->customMode;
}

void WlrOutputConfigurationHeadV1::setCustomMode(WlrOutputModeV1 *mode)
{
    Q_D(WlrOutputConfigurationHeadV1);

    d->customMode = mode;
    Q_EMIT customModeChanged();

    d->set_custom_mode(mode->size().width(), mode->size().height(), mode->refresh());
}

QPoint WlrOutputConfigurationHeadV1::position() const
{
    Q_D(const WlrOutputConfigurationHeadV1);
    return d->position;
}

void WlrOutputConfigurationHeadV1::setPosition(const QPoint &position)
{
    Q_D(WlrOutputConfigurationHeadV1);

    d->position = position;
    Q_EMIT positionChanged();

    d->set_position(position.x(), position.y());
}

WlrOutputHeadV1::Transform WlrOutputConfigurationHeadV1::transform() const
{
    Q_D(const WlrOutputConfigurationHeadV1);
    return d->transform;
}

void WlrOutputConfigurationHeadV1::setTransform(WlrOutputHeadV1::Transform transform)
{
    Q_D(WlrOutputConfigurationHeadV1);

    d->transform = transform;
    Q_EMIT transformChanged();

    d->set_transform(static_cast<int32_t>(transform));
}

qreal WlrOutputConfigurationHeadV1::scale() const
{
    Q_D(const WlrOutputConfigurationHeadV1);
    return d->scale;
}

void WlrOutputConfigurationHeadV1::setScale(qreal scale)
{
    Q_D(WlrOutputConfigurationHeadV1);

    d->scale = scale;
    Q_EMIT scaleChanged();

    d->set_scale(wl_fixed_from_double(scale));
}
