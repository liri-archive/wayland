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

#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

#include "wlrlayershellv1_p.h"
#include "logging_p.h"

static inline struct ::wl_surface *getWlSurface(QWindow *window)
{
    if (!window)
        return nullptr;
    void *surface = QGuiApplication::platformNativeInterface()->nativeResourceForWindow("surface", window);
    return static_cast<struct ::wl_surface *>(surface);
}

static inline struct ::wl_output *getWlOutput(QScreen *screen)
{
    if (!screen)
        return nullptr;
    void *output = QGuiApplication::platformNativeInterface()->nativeResourceForScreen("output", screen);
    return static_cast<struct ::wl_output *>(output);
}


struct ::zwlr_layer_surface_v1 *WlrLayerShellV1Private::createLayerSurface(QWindow *window,
                                                                           QScreen *screen,
                                                                           WlrLayerShellV1::Layer layer,
                                                                           const QString &nameSpace)
{
    auto surface = getWlSurface(window);
    if (!surface) {
        qCWarning(lcWaylandClient, "Unable to create layer surface: window doesn't have a Wayland surface yet");
        return nullptr;
    }

    return get_layer_surface(surface, getWlOutput(screen),
                             static_cast<uint32_t>(layer),
                             nameSpace);
}


WlrLayerShellV1::WlrLayerShellV1()
    : QWaylandClientExtensionTemplate<WlrLayerShellV1>(2)
    , d_ptr(new WlrLayerShellV1Private)
{
}

WlrLayerShellV1::~WlrLayerShellV1()
{
    delete d_ptr;
}

void WlrLayerShellV1::init(wl_registry *registry, int id, int version)
{
    Q_D(WlrLayerShellV1);
    d->init(registry, id, version);
}

const wl_interface *WlrLayerShellV1::interface()
{
    return WlrLayerShellV1Private::interface();
}


WlrLayerSurfaceV1Private::WlrLayerSurfaceV1Private(WlrLayerSurfaceV1 *self)
    : q_ptr(self)
{
}

void WlrLayerSurfaceV1Private::zwlr_layer_surface_v1_configure(uint32_t serial, uint32_t width, uint32_t height)
{
    Q_Q(WlrLayerSurfaceV1);
    Q_EMIT q->configured(serial, width, height);
}

void WlrLayerSurfaceV1Private::zwlr_layer_surface_v1_closed()
{
    Q_Q(WlrLayerSurfaceV1);
    Q_EMIT q->closed();
}


WlrLayerSurfaceV1::WlrLayerSurfaceV1(QObject *parent)
    : QObject(parent)
    , d_ptr(new WlrLayerSurfaceV1Private(this))
{
}

WlrLayerSurfaceV1::~WlrLayerSurfaceV1()
{
    delete d_ptr;
}

WlrLayerShellV1 *WlrLayerSurfaceV1::shell() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->shell;
}

void WlrLayerSurfaceV1::setShell(WlrLayerShellV1 *shell)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->shell == shell)
        return;

    if (d->initialized) {
        qCWarning(lcWaylandClient, "Unable to change WlrLayerSurfaceV1::shell after initialization");
        return;
    }

    d->shell = shell;
    Q_EMIT shellChanged();
}

WlrLayerShellV1::Layer WlrLayerSurfaceV1::layer() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->layer;
}

void WlrLayerSurfaceV1::setLayer(WlrLayerShellV1::Layer layer)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->layer == layer)
        return;

    if (d->initialized && d->interface()->version < ZWLR_LAYER_SURFACE_V1_SET_LAYER_SINCE_VERSION) {
        qCWarning(lcWaylandClient, "Unable to change WlrLayerSurfaceV1::layer after initialization");
        return;
    }

    d->layer = layer;
    Q_EMIT layerChanged();

    if (d->initialized && d->interface()->version >= ZWLR_LAYER_SURFACE_V1_SET_LAYER_SINCE_VERSION) {
        d->set_layer(d->layer);
        if (d->window)
            wl_surface_commit(getWlSurface(d->window));
    }
}

QString WlrLayerSurfaceV1::nameSpace() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->nameSpace;
}

void WlrLayerSurfaceV1::setNameSpace(const QString &nameSpace)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->nameSpace == nameSpace)
        return;

    if (d->initialized) {
        qCWarning(lcWaylandClient, "Unable to change WlrLayerSurfaceV1::nameSpace after initialization");
        return;
    }

    d->nameSpace = nameSpace;
    Q_EMIT nameSpaceChanged();
}

bool WlrLayerSurfaceV1::showOnAllScreens() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->showOnAllScreens;
}

void WlrLayerSurfaceV1::setShowOnAllScreens(bool show)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->showOnAllScreens == show)
        return;

    if (d->initialized) {
        qCWarning(lcWaylandClient, "Unable to change WlrLayerSurfaceV1::showOnAllScreens after initialization");
        return;
    }

    d->showOnAllScreens = show;
    Q_EMIT showOnAllScreensChanged();
}

QWindow *WlrLayerSurfaceV1::window() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->window;
}

void WlrLayerSurfaceV1::setWindow(QWindow *window)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->window == window)
        return;

    if (d->initialized) {
        qCWarning(lcWaylandClient, "Unable to change WlrLayerSurfaceV1::window after initialization");
        return;
    }

    d->window = window;
    Q_EMIT windowChanged();
}

QSize WlrLayerSurfaceV1::size() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->size;
}

void WlrLayerSurfaceV1::setSize(const QSize &size)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->size == size)
        return;

    d->size = size;
    Q_EMIT sizeChanged();

    if (d->initialized && size.isValid()) {
        d->set_size(size.width(), size.height());
        if (d->window)
            wl_surface_commit(getWlSurface(d->window));
    }
}

WlrLayerSurfaceV1::Anchors WlrLayerSurfaceV1::anchors() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->anchors;
}

void WlrLayerSurfaceV1::setAnchors(Anchors anchors)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->anchors == anchors)
        return;

    d->anchors = anchors;
    Q_EMIT anchorsChanged();

    if (d->initialized)
        d->set_anchor(static_cast<uint32_t>(anchors));
}

qint32 WlrLayerSurfaceV1::exclusiveZone() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->exclusiveZone;
}

void WlrLayerSurfaceV1::setExclusiveZone(qint32 zone)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->exclusiveZone == zone)
        return;

    d->exclusiveZone = zone;
    Q_EMIT exclusiveZoneChanged();

    if (d->initialized)
        d->set_exclusive_zone(zone);
}

QMargins WlrLayerSurfaceV1::margins() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->margins;
}

qint32 WlrLayerSurfaceV1::leftMargin() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->margins.left();
}

void WlrLayerSurfaceV1::setLeftMargin(qint32 margin)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->margins.left() == margin)
        return;

    d->margins.setLeft(margin);
    Q_EMIT leftMarginChanged();

    if (d->initialized)
        d->set_margin(d->margins.top(), d->margins.right(), d->margins.bottom(), d->margins.left());
}

qint32 WlrLayerSurfaceV1::rightMargin() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->margins.right();
}

void WlrLayerSurfaceV1::setRightMargin(qint32 margin)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->margins.right() == margin)
        return;

    d->margins.setRight(margin);
    Q_EMIT rightMarginChanged();

    if (d->initialized)
        d->set_margin(d->margins.top(), d->margins.right(), d->margins.bottom(), d->margins.left());
}

qint32 WlrLayerSurfaceV1::topMargin() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->margins.top();
}

void WlrLayerSurfaceV1::setTopMargin(qint32 margin)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->margins.top() == margin)
        return;

    d->margins.setTop(margin);
    Q_EMIT topMarginChanged();

    if (d->initialized)
        d->set_margin(d->margins.top(), d->margins.right(), d->margins.bottom(), d->margins.left());
}

qint32 WlrLayerSurfaceV1::bottomMargin() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->margins.bottom();
}

void WlrLayerSurfaceV1::setBottomMargin(qint32 margin)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->margins.bottom() == margin)
        return;

    d->margins.setBottom(margin);
    Q_EMIT bottomMarginChanged();

    if (d->initialized)
        d->set_margin(d->margins.top(), d->margins.right(), d->margins.bottom(), d->margins.left());
}

bool WlrLayerSurfaceV1::keyboardInteractivity() const
{
    Q_D(const WlrLayerSurfaceV1);
    return d->keyboardInteractivity;
}

void WlrLayerSurfaceV1::setKeyboardInteractivity(bool enabled)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->keyboardInteractivity == enabled)
        return;

    d->keyboardInteractivity = enabled;
    Q_EMIT keyboardInteractivityChanged();

    if (d->initialized)
        d->set_keyboard_interactivity(enabled ? 1 : 0);
}

void WlrLayerSurfaceV1::ackConfigure(quint32 serial)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->initialized)
        d->ack_configure(serial);
}

void WlrLayerSurfaceV1::addMask(const QRect &rect)
{
    Q_D(WlrLayerSurfaceV1);

    if (d->window) {
        d->mask += rect;
        d->window->setMask(d->mask);
    }
}

void WlrLayerSurfaceV1::unsetMask()
{
    Q_D(WlrLayerSurfaceV1);

    if (d->window) {
        d->mask = QRegion();
        d->window->setMask(d->mask);
    }
}

void WlrLayerSurfaceV1::initialize()
{
    Q_D(WlrLayerSurfaceV1);

    if (d->initialized)
        return;

    if (!d->shell) {
        qCWarning(lcWaylandClient, "Shell not assigned to WlrLayerSurfaceV1, failed to inizialize");
        return;
    }

    if (!d->window) {
        qCWarning(lcWaylandClient, "Window not assigned to WlrLayerSurfaceV1, failed to initialize");
        return;
    }

    // We must not initialize twice
    d->initialized = true;

    // Create wl_surface
    d->window->create();

    // Create layer surface object
    auto layerSurface = WlrLayerShellV1Private::get(d->shell)->createLayerSurface(
                d->window, d->showOnAllScreens ? nullptr : d->window->screen(), d->layer, d->nameSpace);
    d->init(layerSurface);

    // Set layer surface properties
    if (d->size.isValid())
        d->set_size(d->size.width(), d->size.height());
    d->set_anchor(static_cast<uint32_t>(d->anchors));
    d->set_exclusive_zone(d->exclusiveZone);
    d->set_margin(d->margins.top(), d->margins.right(), d->margins.bottom(), d->margins.left());
    d->set_keyboard_interactivity(d->keyboardInteractivity ? 1 : 0);
    wl_surface_commit(getWlSurface(d->window));
}
