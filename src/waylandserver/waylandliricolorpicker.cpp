// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QQuickItem>
#include <QQuickItemGrabResult>
#include <QQuickWindow>
#include <QScreen>
#include <QTimer>
#include <QWaylandCompositor>

#include "logging_p.h"
#include "waylandliricolorpicker_p.h"

/*
 * WaylandLiriColorPicker
 */

WaylandLiriColorPicker::WaylandLiriColorPicker(QObject *parent)
    : QObject(parent)
    , QtWaylandServer::liri_color_picker()
{
}

void WaylandLiriColorPicker::liri_color_picker_destroy_resource(Resource *resource)
{
    Q_UNUSED(resource)

    WaylandLiriColorPickerManagerPrivate::get(manager)->pickers.remove(output);
    deleteLater();
}

void WaylandLiriColorPicker::liri_color_picker_pick_at_location(Resource *resource,
                                                                uint32_t serial,
                                                                uint32_t x, uint32_t y)
{
    Q_UNUSED(resource)

    QColor color;

    if (output->window()) {
        auto *quickWindow = qobject_cast<QQuickWindow *>(output->window());
        if (quickWindow) {
            color = quickWindow->grabWindow().pixelColor(x, y);
        } else {
            auto winId = output->window()->winId();
            auto pixmap = output->window()->screen()->grabWindow(winId);
            color = pixmap.toImage().pixelColor(x, y);
        }
    }

    if (color.isValid())
        send_picked(serial, static_cast<uint32_t>(color.rgba()));
}

void WaylandLiriColorPicker::liri_color_picker_pick_interactively(Resource *resource,
                                                                  struct ::wl_resource *seat_res,
                                                                  uint32_t serial)
{
    Q_UNUSED(resource)

    if (output->window()) {
        auto *seat = QWaylandSeat::fromSeatResource(seat_res);
        if (!seat) {
            auto seatId = wl_resource_get_id(seat_res);
            qCWarning(lcColorPickerServer, "Resource wl_seat@%d doesn't exist", seatId);
            wl_resource_post_error(resource->handle, WL_DISPLAY_ERROR_INVALID_OBJECT,
                                   "resource wl_seat@%d doesn't exit", seatId);
            return;
        }

        if (filter) {
            qCWarning(lcColorPickerServer, "Previous interactive color picking is still in progress");
            wl_resource_post_error(resource->handle, error_still_in_progress,
                                   "previous interactive color picking is still in progress");
            return;
        }

        filter = new WaylandLiriColorPickerEventFilter(this, seat, serial, this);
        output->window()->installEventFilter(filter.data());
    }
}

void WaylandLiriColorPicker::liri_color_picker_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

/*
 * WaylandLiriColorPickerEventFilter
 */

WaylandLiriColorPickerEventFilter::WaylandLiriColorPickerEventFilter(WaylandLiriColorPicker *picker,
                                                                     QWaylandSeat *seat,
                                                                     quint32 serial,
                                                                     QObject *parent)
    : QObject(parent)
    , m_picker(picker)
    , m_seat(seat)
    , m_serial(serial)
{
}

bool WaylandLiriColorPickerEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    // When the mouse button is pressed, we select the pixel at the pointer
    // location and then send the result back to the client when the button
    // is released
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease) {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);

        // Verify that this mouse event is for the target seat
        auto *detectedSeat = m_seat->compositor()->seatFor(mouseEvent);
        if (detectedSeat == m_seat) {
            if (event->type() == QEvent::MouseButtonPress) {
                auto pos = mouseEvent->localPos();

                auto *quickWindow = qobject_cast<QQuickWindow *>(m_picker->output->window());
                if (quickWindow) {
                    auto layerName = m_picker->manager->layerName();
                    auto *item = quickWindow->contentItem();
                    auto *uiItem = layerName.isEmpty() ? item : item->findChild<QQuickItem *>(layerName);

                    // Synchronously capture the layer contents within 5 seconds
                    QSharedPointer<QQuickItemGrabResult> result = uiItem->grabToImage();
                    if (result.isNull() || result->image().isNull()) {
                        QEventLoop loop;
                        connect(result.data(), &QQuickItemGrabResult::ready,
                                &loop, &QEventLoop::quit);
                        QTimer::singleShot(5000, &loop, &QEventLoop::quit);
                        loop.exec();
                    }

                    m_color = result->image().pixelColor(pos.toPoint());
                } else {
                    auto winId = m_picker->output->window()->winId();
                    auto pixmap = m_picker->output->window()->screen()->grabWindow(winId);
                    m_color = pixmap.toImage().pixelColor(pos.toPoint());
                }

                return true;
            } else if (event->type() == QEvent::MouseButtonRelease) {
                m_picker->send_picked(m_serial, static_cast<uint32_t>(m_color.rgba()));

                m_picker->output->removeEventFilter(this);
                deleteLater();

                return true;
            }
        }
    }

    return QObject::eventFilter(watched, event);
}

/*
 * WaylandLiriColorPickerManagerPrivate
 */

WaylandLiriColorPickerManagerPrivate::WaylandLiriColorPickerManagerPrivate(WaylandLiriColorPickerManager *self)
    : QtWaylandServer::liri_color_picker_manager()
    , q_ptr(self)
{
}

void WaylandLiriColorPickerManagerPrivate::liri_color_picker_manager_create_picker(Resource *resource,
                                                                                   uint32_t id,
                                                                                   struct ::wl_resource *output_res)
{
    Q_Q(WaylandLiriColorPickerManager);

    auto *output = QWaylandOutput::fromResource(output_res);
    auto outputId = wl_resource_get_id(output_res);
    if (!output) {
        qCWarning(lcColorPickerServer, "Resource wl_output@%d doesn't exist", outputId);
        wl_resource_post_error(resource->handle, WL_DISPLAY_ERROR_INVALID_OBJECT,
                               "resource wl_output@%d doesn't exit", outputId);
        return;
    }

    if (pickers.contains(output)) {
        qCWarning(lcColorPickerServer, "A color picker for the same output already exist");
        wl_resource_post_error(resource->handle, error_already_constructed,
                               "a color picker already exist for wl_output@%d", outputId);
        return;
    }

    auto *picker = new WaylandLiriColorPicker(q);
    picker->manager = q;
    picker->output = output;
    picker->init(resource->client(), id, resource->version());
    pickers[output] = picker;
}

/*
 * WaylandLiriColorPickerManager
 */

WaylandLiriColorPickerManager::WaylandLiriColorPickerManager()
    : QWaylandCompositorExtensionTemplate<WaylandLiriColorPickerManager>()
    , d_ptr(new WaylandLiriColorPickerManagerPrivate(this))
{
}

WaylandLiriColorPickerManager::WaylandLiriColorPickerManager(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<WaylandLiriColorPickerManager>(compositor)
    , d_ptr(new WaylandLiriColorPickerManagerPrivate(this))
{
}

WaylandLiriColorPickerManager::~WaylandLiriColorPickerManager()
{
    delete d_ptr;
}

QString WaylandLiriColorPickerManager::layerName() const
{
    Q_D(const WaylandLiriColorPickerManager);
    return d->layerName;
}

void WaylandLiriColorPickerManager::setLayerName(const QString &layer)
{
    Q_D(WaylandLiriColorPickerManager);

    if (d->layerName == layer)
        return;

    d->layerName = layer;
    Q_EMIT layerNameChanged();
}

void WaylandLiriColorPickerManager::initialize()
{
    Q_D(WaylandLiriColorPickerManager);

    QWaylandCompositorExtensionTemplate::initialize();
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qCWarning(lcColorPickerServer) << "Failed to find QWaylandCompositor when initializing WaylandLiriColorPickerManager";
        return;
    }
    d->init(compositor->display(), WaylandLiriColorPickerManagerPrivate::interfaceVersion());
}

const wl_interface *WaylandLiriColorPickerManager::interface()
{
    return WaylandLiriColorPickerManagerPrivate::interface();
}

QByteArray WaylandLiriColorPickerManager::interfaceName()
{
    return WaylandLiriColorPickerManagerPrivate::interfaceName();
}
