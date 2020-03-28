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

#include <QDateTime>
#include <QEventLoop>
#include <QPainter>
#include <QQuickItem>
#include <QQuickItemGrabResult>
#include <QQuickWindow>
#include <QWaylandCompositor>
#include <QWaylandOutput>

#include "waylandwlrscreencopyv1_p.h"
#include "logging_p.h"

static inline QImage::Format fromWaylandShmFormat(wl_shm_format format)
{
    switch (format) {
    case WL_SHM_FORMAT_XRGB8888: return QImage::Format_RGB32;
    case WL_SHM_FORMAT_ARGB8888: return QImage::Format_ARGB32_Premultiplied;
    case WL_SHM_FORMAT_RGB565: return QImage::Format_RGB16;
    case WL_SHM_FORMAT_XRGB1555: return QImage::Format_RGB555;
    case WL_SHM_FORMAT_RGB888: return QImage::Format_RGB888;
    case WL_SHM_FORMAT_XRGB4444: return QImage::Format_RGB444;
    case WL_SHM_FORMAT_ARGB4444: return QImage::Format_ARGB4444_Premultiplied;
    case WL_SHM_FORMAT_XBGR8888: return QImage::Format_RGBX8888;
    case WL_SHM_FORMAT_ABGR8888: return QImage::Format_RGBA8888_Premultiplied;
    case WL_SHM_FORMAT_XBGR2101010: return QImage::Format_BGR30;
    case WL_SHM_FORMAT_ABGR2101010: return QImage::Format_A2BGR30_Premultiplied;
    case WL_SHM_FORMAT_XRGB2101010: return QImage::Format_RGB30;
    case WL_SHM_FORMAT_ARGB2101010: return QImage::Format_A2RGB30_Premultiplied;
    case WL_SHM_FORMAT_C8: return QImage::Format_Alpha8;
    default: return QImage::Format_Invalid;
    }
}

/*
 * WaylandWlrScreencopyFrameEventFilter
 */

WaylandWlrScreencopyFrameEventFilter::WaylandWlrScreencopyFrameEventFilter(QObject *parent)
    : QObject(parent)
{
}

bool WaylandWlrScreencopyFrameEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Expose) {
        auto *exposeEvent = static_cast<QExposeEvent *>(event);
        if (!exposeEvent->region().isEmpty()) {
            frame->output->window()->removeEventFilter(this);
            frame->damageRect = exposeEvent->region().boundingRect();
            frame->copy(resource, buffer_res);
            return true;
        }
    }

    return QObject::eventFilter(watched, event);
}

/*
 * WaylandWlrScreencopyManagerV1Private
 */

WaylandWlrScreencopyManagerV1Private::WaylandWlrScreencopyManagerV1Private(WaylandWlrScreencopyManagerV1 *self)
    : QtWaylandServer::zwlr_screencopy_manager_v1()
    , q_ptr(self)
{
}

void WaylandWlrScreencopyManagerV1Private::zwlr_screencopy_manager_v1_capture_output(
        Resource *resource, uint32_t frame, int32_t overlay_cursor,
        struct ::wl_resource *output_res)
{
    Q_Q(WaylandWlrScreencopyManagerV1);

    auto *output = QWaylandOutput::fromResource(output_res);
    if (!output) {
        auto id = wl_resource_get_id(output_res);
        qCWarning(lcScreencopy, "Resource wl_output@%d doesn't exist", id);
        wl_resource_post_error(resource->handle, WL_DISPLAY_ERROR_INVALID_OBJECT,
                               "resource wl_output@%d doesn't exit", id);
        return;
    }

    auto *screencopyFrame = new WaylandWlrScreencopyFrameV1(q);
    auto *screencopyFramePriv = WaylandWlrScreencopyFrameV1Private::get(screencopyFrame);
    screencopyFramePriv->overlayCursor = overlay_cursor == 1;
    screencopyFramePriv->output = output;
    screencopyFramePriv->rect = QRect(QPoint(0, 0), output->geometry().size());
    screencopyFramePriv->stride = 4 * output->geometry().width();
    screencopyFramePriv->init(resource->client(), frame, resource->version());

    emit q->captureOutputRequested(screencopyFrame);

    // This will send the buffer to the client, which in turn will ask us
    // to make a copy therefore we'll emit a signal: so let's do this after
    // having emitted captureOutputRequested() to give the compositor a
    // chance to connect to the frame copied() signal
    screencopyFramePriv->setup();
}

void WaylandWlrScreencopyManagerV1Private::zwlr_screencopy_manager_v1_capture_output_region(
        Resource *resource, uint32_t frame, int32_t overlay_cursor,
        struct ::wl_resource *output_res, int32_t x, int32_t y,
        int32_t width, int32_t height)
{
    Q_Q(WaylandWlrScreencopyManagerV1);

    auto *output = QWaylandOutput::fromResource(output_res);
    if (!output) {
        auto id = wl_resource_get_id(output_res);
        qCWarning(lcScreencopy, "Resource wl_output@%d doesn't exist", id);
        wl_resource_post_error(resource->handle, WL_DISPLAY_ERROR_INVALID_OBJECT,
                               "resource wl_output@%d doesn't exit", id);
        return;
    }

    auto *screencopyFrame = new WaylandWlrScreencopyFrameV1(q);
    auto *screencopyFramePriv = WaylandWlrScreencopyFrameV1Private::get(screencopyFrame);
    screencopyFramePriv->overlayCursor = overlay_cursor == 1;
    screencopyFramePriv->output = output;
    screencopyFramePriv->rect = QRect(x, y, width, height);
    screencopyFramePriv->stride = 4 * width;
    screencopyFramePriv->init(resource->client(), frame, resource->version());

    emit q->captureOutputRequested(screencopyFrame);

    // This will send the buffer to the client, which in turn will ask us
    // to make a copy therefore we'll emit a signal: so let's do this after
    // having emitted captureOutputRequested() to give the compositor a
    // chance to connect to the frame copied() signal
    screencopyFramePriv->setup();
}

void WaylandWlrScreencopyManagerV1Private::zwlr_screencopy_manager_v1_destroy(
        Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

/*
 * WaylandWlrScreencopyManagerV1
 */

WaylandWlrScreencopyManagerV1::WaylandWlrScreencopyManagerV1()
    : QWaylandCompositorExtensionTemplate<WaylandWlrScreencopyManagerV1>()
    , d_ptr(new WaylandWlrScreencopyManagerV1Private(this))
{
}

WaylandWlrScreencopyManagerV1::WaylandWlrScreencopyManagerV1(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<WaylandWlrScreencopyManagerV1>(compositor)
    , d_ptr(new WaylandWlrScreencopyManagerV1Private(this))
{
}

WaylandWlrScreencopyManagerV1::~WaylandWlrScreencopyManagerV1()
{
    delete d_ptr;
}

void WaylandWlrScreencopyManagerV1::initialize()
{
    Q_D(WaylandWlrScreencopyManagerV1);

    QWaylandCompositorExtensionTemplate::initialize();
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qCWarning(lcScreencopy) << "Failed to find QWaylandCompositor when initializing WaylandWlrScreencopyManagerV1";
        return;
    }
    d->init(compositor->display(), WaylandWlrScreencopyManagerV1Private::interfaceVersion());
}

const wl_interface *WaylandWlrScreencopyManagerV1::interface()
{
    return WaylandWlrScreencopyManagerV1Private::interface();
}

QByteArray WaylandWlrScreencopyManagerV1::interfaceName()
{
    return WaylandWlrScreencopyManagerV1Private::interfaceName();
}

/*
 * WaylandWlrScreencopyFrameV1Private
 */

WaylandWlrScreencopyFrameV1Private::WaylandWlrScreencopyFrameV1Private(WaylandWlrScreencopyFrameV1 *self)
    : QtWaylandServer::zwlr_screencopy_frame_v1()
    , q_ptr(self)
{
}

WaylandWlrScreencopyFrameV1Private::~WaylandWlrScreencopyFrameV1Private()
{
    if (filterObject)
        filterObject->deleteLater();
}

void WaylandWlrScreencopyFrameV1Private::setup()
{
    send_buffer(requestedBufferFormat, rect.width(), rect.height(), stride);

    QDateTime dateTime(QDateTime::currentDateTimeUtc());
    qint64 secs = dateTime.toSecsSinceEpoch();
    tv_sec_hi = secs >> 32;
    tv_sec_lo = secs & 0xffffffff;
}

QImage WaylandWlrScreencopyFrameV1Private::grabItem(QQuickItem *item)
{
    QSharedPointer<QQuickItemGrabResult> result = item->grabToImage();
    if (result.isNull() || result->image().isNull()) {
        QEventLoop loop;
        QObject::connect(result.data(), &QQuickItemGrabResult::ready,
                         &loop, &QEventLoop::quit);
        loop.exec();
    }

    return result->image();
}

void WaylandWlrScreencopyFrameV1Private::copy(Resource *resource, wl_resource *buffer_res)
{
    Q_Q(WaylandWlrScreencopyFrameV1);

    QVariant enabledValue = output->property("enabled");
    if (enabledValue.isValid()) {
        const bool isEnabled = enabledValue.toBool();
        if (!isEnabled) {
            qCWarning(lcScreencopy, "Copying a disabled output is not allowed");
            send_failed();
            return;
        }
    }

    auto *shmBuffer = wl_shm_buffer_get(buffer_res);
    if (!shmBuffer) {
        qCWarning(lcScreencopy, "Unsupported buffer type, only shm buffers are supported");
        wl_resource_post_error(resource->handle, error_invalid_buffer,
                               "unsupported buffer type");
        return;
    }

    uint32_t bufferFormat = wl_shm_buffer_get_format(shmBuffer);
    int32_t bufferWidth = wl_shm_buffer_get_width(shmBuffer);
    int32_t bufferHeight = wl_shm_buffer_get_height(shmBuffer);
    int32_t bufferStride = wl_shm_buffer_get_stride(shmBuffer);

    if (bufferFormat != requestedBufferFormat || bufferWidth != rect.width() ||
            bufferHeight != rect.height() || bufferStride != stride) {
        qCWarning(lcScreencopy, "Invalid buffer attributes");
        wl_resource_post_error(resource->handle, error_invalid_buffer,
                               "invalid buffer attributes");
        return;
    }

    if (withDamage)
        send_damage(damageRect.x(), damageRect.y(), damageRect.width(), damageRect.height());

    ready = true;
    buffer = shmBuffer;
    emit q->ready();
}

void WaylandWlrScreencopyFrameV1Private::zwlr_screencopy_frame_v1_destroy_resource(Resource *resource)
{
    Q_UNUSED(resource)

    Q_Q(WaylandWlrScreencopyFrameV1);
    q->deleteLater();
}

void WaylandWlrScreencopyFrameV1Private::zwlr_screencopy_frame_v1_copy(
        Resource *resource, struct ::wl_resource *buffer_res)
{
    copy(resource, buffer_res);
}

void WaylandWlrScreencopyFrameV1Private::zwlr_screencopy_frame_v1_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void WaylandWlrScreencopyFrameV1Private::zwlr_screencopy_frame_v1_copy_with_damage(
        Resource *resource, struct ::wl_resource *buffer_res)
{
    withDamage = true;

    filterObject = new WaylandWlrScreencopyFrameEventFilter();
    filterObject->resource = resource;
    filterObject->buffer_res = buffer_res;
    filterObject->frame = this;
    output->window()->installEventFilter(filterObject);
}

/*
 * WaylandWlrScreencopyFrameV1
 */

WaylandWlrScreencopyFrameV1::WaylandWlrScreencopyFrameV1(QObject *parent)
    : QObject(parent)
    , d_ptr(new WaylandWlrScreencopyFrameV1Private(this))
{
}

WaylandWlrScreencopyFrameV1::~WaylandWlrScreencopyFrameV1()
{
    delete d_ptr;
}

bool WaylandWlrScreencopyFrameV1::overlayCursor() const
{
    Q_D(const WaylandWlrScreencopyFrameV1);
    return d->overlayCursor;
}

QWaylandOutput *WaylandWlrScreencopyFrameV1::output() const
{
    Q_D(const WaylandWlrScreencopyFrameV1);
    return d->output;
}

QRect WaylandWlrScreencopyFrameV1::region() const
{
    Q_D(const WaylandWlrScreencopyFrameV1);
    return d->rect;
}

WaylandWlrScreencopyFrameV1::Flags WaylandWlrScreencopyFrameV1::flags() const
{
    Q_D(const WaylandWlrScreencopyFrameV1);
    return d->flags;
}

void WaylandWlrScreencopyFrameV1::setFlags(WaylandWlrScreencopyFrameV1::Flags flags)
{
    Q_D(WaylandWlrScreencopyFrameV1);

    if (d->flags == flags)
        return;

    d->flags = flags;
    emit flagsChanged();
}

void WaylandWlrScreencopyFrameV1::grabCursorItem(QQuickItem *item)
{
    Q_D(WaylandWlrScreencopyFrameV1);

    if (!d->overlayCursor) {
        qCWarning(lcScreencopy, "Invoking grabCursorItem() on a frame with overlayCursor disabled has no effect");
        return;
    }

    d->cursorPosition = item->position();
    d->cursorImage = d->grabItem(item);
}

void WaylandWlrScreencopyFrameV1::copy()
{
    Q_D(WaylandWlrScreencopyFrameV1);

    if (d->ready) {
        wl_shm_buffer_begin_access(d->buffer);
        void *data = wl_shm_buffer_get_data(d->buffer);

        auto *quickWindow = qobject_cast<QQuickWindow *>(d->output->window());
        if (quickWindow) {
            QImage finalImage = d->grabItem(quickWindow->contentItem());

            if (d->overlayCursor) {
                QPainter painter(&finalImage);
                painter.drawImage(d->cursorPosition, d->cursorImage);
            }

            if (d->rect.x() > 0 || d->rect.y() > 0)
                finalImage = finalImage.copy(d->rect);

            // The buffer format is decided before grabbing the window contents,
            // we don't know the QImage format at that time, so we convert it
            // if needed
            auto bufferFormat = static_cast<wl_shm_format>(wl_shm_buffer_get_format(d->buffer));
            auto imageFormat = fromWaylandShmFormat(bufferFormat);
            if (finalImage.format() != imageFormat)
                finalImage.convertTo(imageFormat);

            memcpy(data, finalImage.bits(), finalImage.sizeInBytes());
        } else {
            QRect rect = d->rect.translated(d->output->position());

            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glReadPixels(rect.x(), rect.y(), rect.width(), rect.height(), GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        wl_shm_buffer_end_access(d->buffer);

        d->send_flags(static_cast<uint32_t>(d->flags));
        d->send_ready(d->tv_sec_hi, d->tv_sec_lo, 0);
    } else {
        qCWarning(lcScreencopy, "Cannot copy a frame that is not ready");
    }
}
