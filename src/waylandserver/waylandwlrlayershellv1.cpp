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

#include <QWaylandCompositor>
#include <QWaylandOutput>
#include <QWaylandSurface>

#include "waylandwlrlayershellv1_p.h"
#include "logging_p.h"

WaylandWlrLayerShellV1Private::WaylandWlrLayerShellV1Private(WaylandWlrLayerShellV1 *self)
    : QtWaylandServer::zwlr_layer_shell_v1()
    , q_ptr(self)
{
}

void WaylandWlrLayerShellV1Private::zwlr_layer_shell_v1_get_layer_surface(
        QtWaylandServer::zwlr_layer_shell_v1::Resource *resource,
        uint32_t id, wl_resource *surfaceRes, wl_resource *outputRes,
        uint32_t layer, const QString &nameSpace)
{
    Q_Q(WaylandWlrLayerShellV1);

    auto surface = QWaylandSurface::fromResource(surfaceRes);
    if (!surface) {
        qCWarning(lcWaylandServer, "Resource wl_surface@%d doesn't exist",
                  wl_resource_get_id(surfaceRes));
        return;
    }

    // Set role
    if (!surface->setRole(WaylandWlrLayerSurfaceV1::role(), resource->handle, error_role))
        return;

    // Output from resource
    QWaylandOutput *output = nullptr;
    if (outputRes)
        output = QWaylandOutput::fromResource(outputRes);

    // Create layer surface
    auto *layerSurface = new WaylandWlrLayerSurfaceV1(surface, output, static_cast<WaylandWlrLayerShellV1::Layer>(layer), nameSpace);
    WaylandWlrLayerSurfaceV1Private::get(layerSurface)->init(resource->client(), id, resource->version());
    Q_EMIT q->layerSurfaceCreated(layerSurface);
}


WaylandWlrLayerShellV1::WaylandWlrLayerShellV1()
    : QWaylandCompositorExtensionTemplate<WaylandWlrLayerShellV1>()
    , d_ptr(new WaylandWlrLayerShellV1Private(this))
{
}

WaylandWlrLayerShellV1::WaylandWlrLayerShellV1(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<WaylandWlrLayerShellV1>(compositor)
    , d_ptr(new WaylandWlrLayerShellV1Private(this))
{
}

WaylandWlrLayerShellV1::~WaylandWlrLayerShellV1()
{
    delete d_ptr;
}

void WaylandWlrLayerShellV1::initialize()
{
    Q_D(WaylandWlrLayerShellV1);

    QWaylandCompositorExtensionTemplate::initialize();
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qCWarning(lcWaylandServer) << "Failed to find QWaylandCompositor when initializing WaylandWlrLayerShellV1";
        return;
    }
    d->init(compositor->display(), WaylandWlrLayerShellV1Private::interfaceVersion());
}

const wl_interface *WaylandWlrLayerShellV1::interface()
{
    return WaylandWlrLayerShellV1Private::interface();
}

QByteArray WaylandWlrLayerShellV1::interfaceName()
{
    return WaylandWlrLayerShellV1Private::interfaceName();
}


QWaylandSurfaceRole WaylandWlrLayerSurfaceV1Private::s_role("zwlr_layer_surface_v1");

WaylandWlrLayerSurfaceV1Private::WaylandWlrLayerSurfaceV1Private(WaylandWlrLayerSurfaceV1 *self)
    : QtWaylandServer::zwlr_layer_surface_v1()
    , q_ptr(self)
{
}

WaylandWlrLayerSurfaceV1Private::ConfigureEvent WaylandWlrLayerSurfaceV1Private::lastSentConfigure() const
{
    return pendingConfigures.isEmpty() ? lastAckedConfigure : pendingConfigures.last();
}

void WaylandWlrLayerSurfaceV1Private::unmap()
{
    Q_Q(WaylandWlrLayerSurfaceV1);

    Q_EMIT q->unmapped();

    pendingConfigures.clear();

    configured = mapped = false;
}

void WaylandWlrLayerSurfaceV1Private::zwlr_layer_surface_v1_set_size(QtWaylandServer::zwlr_layer_surface_v1::Resource *resource, uint32_t width, uint32_t height)
{
    Q_UNUSED(resource)
    clientPending.desiredSize = QSize(width, height);
}

void WaylandWlrLayerSurfaceV1Private::zwlr_layer_surface_v1_set_anchor(QtWaylandServer::zwlr_layer_surface_v1::Resource *resource, uint32_t anchor)
{
    Q_UNUSED(resource)

    const uint32_t maxAnchor = anchor_top | anchor_bottom | anchor_left | anchor_right;
    if (anchor > maxAnchor) {
        wl_resource_post_error(resource->handle, error_invalid_anchor,
                               "invalid anchor %d", anchor);
        return;
    }

    clientPending.anchors = static_cast<WaylandWlrLayerSurfaceV1::Anchors>(anchor);
}

void WaylandWlrLayerSurfaceV1Private::zwlr_layer_surface_v1_set_exclusive_zone(QtWaylandServer::zwlr_layer_surface_v1::Resource *resource, int32_t zone)
{
    Q_UNUSED(resource)
    clientPending.exclusiveZone = zone;
}

void WaylandWlrLayerSurfaceV1Private::zwlr_layer_surface_v1_set_margin(QtWaylandServer::zwlr_layer_surface_v1::Resource *resource, int32_t top, int32_t right, int32_t bottom, int32_t left)
{
    Q_UNUSED(resource)
    clientPending.margins = QMargins(left, top, right, bottom);
}

void WaylandWlrLayerSurfaceV1Private::zwlr_layer_surface_v1_set_keyboard_interactivity(QtWaylandServer::zwlr_layer_surface_v1::Resource *resource, uint32_t keyboard_interactivity)
{
    Q_UNUSED(resource)
    clientPending.keyboardInteractivity = !!keyboard_interactivity;
}

void WaylandWlrLayerSurfaceV1Private::zwlr_layer_surface_v1_get_popup(QtWaylandServer::zwlr_layer_surface_v1::Resource *resource, wl_resource *popup)
{
    Q_UNUSED(resource)
}

void WaylandWlrLayerSurfaceV1Private::zwlr_layer_surface_v1_ack_configure(QtWaylandServer::zwlr_layer_surface_v1::Resource *resource, uint32_t serial)
{
    Q_Q(WaylandWlrLayerSurfaceV1);

    ConfigureEvent config;
    bool found = false;

    Q_FOREVER {
        if (pendingConfigures.isEmpty()) {
            qCWarning(lcWaylandServer, "Received an unexpected ack_configure!");
            return;
        }

        config = pendingConfigures.takeFirst();
        if (config.serial == serial) {
            found = true;
            break;
        }
    }

    if (!found) {
        wl_resource_post_error(resource->handle, error_invalid_surface_state,
                               "wrong configure serial: %u", serial);
        return;
    }

    lastAckedConfigure = config;
    configured = true;
}


WaylandWlrLayerSurfaceV1::WaylandWlrLayerSurfaceV1(QWaylandSurface *surface,
                                                   QWaylandOutput *output,
                                                   WaylandWlrLayerShellV1::Layer layer,
                                                   const QString &nameSpace,
                                                   QObject *parent)
    : QObject(parent)
    , d_ptr(new WaylandWlrLayerSurfaceV1Private(this))
{
    Q_D(WaylandWlrLayerSurfaceV1);
    d->surface = surface;
    d->output = output;
    d->layer = layer;
    d->nameSpace = nameSpace;

    connect(surface, &QWaylandSurface::redraw, this, [this, d] {
        if (d->closed)
            return;

        if (d->surface->hasContent() && !d->configured) {
            qCWarning(lcWaylandServer, "layer_surface@%u has not yet been configured",
                      wl_resource_get_id(d->resource()->handle));
            wl_resource_post_error(d->resource()->handle,
                                   QtWaylandServer::zwlr_layer_shell_v1::error_already_constructed,
                                   "layer_surface@%u has never been configured",
                                   wl_resource_get_id(d->resource()->handle));
            return;
        }

        // Set double-buffered properties
        bool hasChanged = false;
        if (d->current.desiredSize != d->clientPending.desiredSize) {
            d->current.desiredSize = d->clientPending.desiredSize;
            Q_EMIT sizeChanged();
            hasChanged = true;
        }
        if (d->current.anchors != d->clientPending.anchors) {
            d->current.anchors = d->clientPending.anchors;
            Q_EMIT anchorsChanged();
            hasChanged = true;
        }
        if (d->current.exclusiveZone != d->clientPending.exclusiveZone) {
            d->current.exclusiveZone = d->clientPending.exclusiveZone;
            Q_EMIT exclusiveZoneChanged();
            hasChanged = true;
        }
        if (d->current.margins != d->clientPending.margins) {
            d->current.margins = d->clientPending.margins;
            Q_EMIT leftMarginChanged();
            Q_EMIT rightMarginChanged();
            Q_EMIT topMarginChanged();
            Q_EMIT bottomMarginChanged();
            hasChanged = true;
        }
        if (d->current.keyboardInteractivity != d->clientPending.keyboardInteractivity) {
            d->current.keyboardInteractivity = d->clientPending.keyboardInteractivity;
            Q_EMIT keyboardInteractivityChanged();
            hasChanged = true;
        }
        if (hasChanged || !d->configured)
            Q_EMIT changed();

        if (!d->added)
            d->added = true;

        if (d->configured && d->surface->hasContent() && !d->mapped) {
            d->mapped = true;
            Q_EMIT mapped();
        }

        if (d->configured && !d->surface->hasContent() && d->mapped)
            d->unmap();
    });
}

WaylandWlrLayerSurfaceV1::~WaylandWlrLayerSurfaceV1()
{
    delete d_ptr;
}

QWaylandSurface *WaylandWlrLayerSurfaceV1::surface() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->surface;
}

QWaylandOutput *WaylandWlrLayerSurfaceV1::output() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->output;
}

WaylandWlrLayerShellV1::Layer WaylandWlrLayerSurfaceV1::layer() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->layer;
}

QString WaylandWlrLayerSurfaceV1::nameSpace() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->nameSpace;
}

QSize WaylandWlrLayerSurfaceV1::size() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->current.desiredSize;
}

quint32 WaylandWlrLayerSurfaceV1::width() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->current.desiredSize.width();
}

quint32 WaylandWlrLayerSurfaceV1::height() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->current.desiredSize.height();
}

WaylandWlrLayerSurfaceV1::Anchors WaylandWlrLayerSurfaceV1::anchors() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->current.anchors;
}

int WaylandWlrLayerSurfaceV1::exclusiveZone() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->current.exclusiveZone;
}

QMargins WaylandWlrLayerSurfaceV1::margins() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->current.margins;
}

qint32 WaylandWlrLayerSurfaceV1::leftMargin() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->current.margins.left();
}

qint32 WaylandWlrLayerSurfaceV1::topMargin() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->current.margins.top();
}

qint32 WaylandWlrLayerSurfaceV1::rightMargin() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->current.margins.right();
}

qint32 WaylandWlrLayerSurfaceV1::bottomMargin() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->current.margins.bottom();
}

bool WaylandWlrLayerSurfaceV1::keyboardInteractivity() const
{
    Q_D(const WaylandWlrLayerSurfaceV1);
    return d->current.keyboardInteractivity;
}

quint32 WaylandWlrLayerSurfaceV1::sendConfigure(const QSize &size)
{
    Q_D(WaylandWlrLayerSurfaceV1);

    if (!size.isValid()) {
        qCWarning(lcWaylandServer) << "Can't configure WaylandWlrLayerSurfaceV1 with invalid size:" << size;
        return 0;
    }

    quint32 serial = d->surface->compositor()->nextSerial();
    d->pendingConfigures.append(WaylandWlrLayerSurfaceV1Private::ConfigureEvent(size, serial));
    d->send_configure(serial, size.width(), size.height());
    return serial;
}

quint32 WaylandWlrLayerSurfaceV1::sendConfigure(int width, int height)
{
    return sendConfigure(QSize(width, height));
}

void WaylandWlrLayerSurfaceV1::close()
{
    Q_D(WaylandWlrLayerSurfaceV1);

    d->closed = true;
    d->unmap();
    d->send_closed();
}

QWaylandSurfaceRole *WaylandWlrLayerSurfaceV1::role()
{
    return &WaylandWlrLayerSurfaceV1Private::s_role;
}
