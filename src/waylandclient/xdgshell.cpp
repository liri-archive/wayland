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

#include "logging_p.h"
#include "utils_p.h"
#include "xdgshell_p.h"

/*
 * XdgWmBasePrivate
 */

XdgWmBasePrivate::XdgWmBasePrivate(XdgWmBase *self)
    : q_ptr(self)
{
}

void XdgWmBasePrivate::xdg_wm_base_ping(uint32_t serial)
{
    pong(serial);
}

/*
 * XdgWmBase
 */

XdgWmBase::XdgWmBase()
    : QWaylandClientExtensionTemplate<XdgWmBase>(1)
    , d_ptr(new XdgWmBasePrivate(this))
{
}

XdgWmBase::~XdgWmBase()
{
    delete d_ptr;
}

void XdgWmBase::init(struct ::wl_registry *registry, int id, int version)
{
    Q_D(XdgWmBase);
    d->init(registry, id, version);
}

const wl_interface *XdgWmBase::interface()
{
    return XdgWmBasePrivate::interface();
}

/*
 * XdgPositionerPrivate
 */

XdgPositionerPrivate::~XdgPositionerPrivate()
{
    destroy();
}

/*
 * XdgPositioner
 */

XdgPositioner::XdgPositioner(QObject *parent)
    : QObject(parent)
    , d_ptr(new XdgPositionerPrivate())
{
}

XdgPositioner::~XdgPositioner()
{
    delete d_ptr;
}

bool XdgPositioner::isInitialized() const
{
    Q_D(const XdgPositioner);
    return d->initialized;
}

bool XdgPositioner::isValid() const
{
    Q_D(const XdgPositioner);
    return !d->size.isEmpty() && d->anchorRect.isValid();
}

XdgWmBase *XdgPositioner::xdgWmBase() const
{
    Q_D(const XdgPositioner);
    return d->xdgWmBase;
}

void XdgPositioner::setXdgWmBase(XdgWmBase *xdgWmBase)
{
    Q_D(XdgPositioner);

    if (d->xdgWmBase == xdgWmBase)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgPositioner::xdgWmBase after initialization");
        return;
    }

    d->xdgWmBase = xdgWmBase;
    emit xdgWmBaseChanged();
}

QSize XdgPositioner::size() const
{
    Q_D(const XdgPositioner);
    return d->size;
}

void XdgPositioner::setSize(const QSize &size)
{
    Q_D(XdgPositioner);

    if (d->size == size)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgPositioner::size after initialization");
        return;
    }

    d->size = size;
    emit sizeChanged();
}

QRect XdgPositioner::anchorRect() const
{
    Q_D(const XdgPositioner);
    return d->anchorRect;
}

void XdgPositioner::setAnchorRect(const QRect &rect)
{
    Q_D(XdgPositioner);

    if (d->anchorRect == rect)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgPositioner::anchorRect after initialization");
        return;
    }

    d->anchorRect = rect;
    emit anchorRectChanged();
}

XdgPositioner::Anchor XdgPositioner::anchor() const
{
    Q_D(const XdgPositioner);
    return d->anchor;
}

void XdgPositioner::setAnchor(XdgPositioner::Anchor anchor)
{
    Q_D(XdgPositioner);

    if (d->anchor == anchor)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgPositioner::anchor after initialization");
        return;
    }

    d->anchor = anchor;
    emit anchorChanged();
}

XdgPositioner::Gravity XdgPositioner::gravity() const
{
    Q_D(const XdgPositioner);
    return d->gravity;
}

void XdgPositioner::setGravity(XdgPositioner::Gravity gravity)
{
    Q_D(XdgPositioner);

    if (d->gravity == gravity)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgPositioner::gravity after initialization");
        return;
    }

    d->gravity = gravity;
    emit gravityChanged();
}

XdgPositioner::ConstraintAdjustments XdgPositioner::constraintAdjustments() const
{
    Q_D(const XdgPositioner);
    return d->constraintAdjustments;
}

void XdgPositioner::setConstraintAdjustments(ConstraintAdjustments constraintAdjustments)
{
    Q_D(XdgPositioner);

    if (d->constraintAdjustments == constraintAdjustments)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgPositioner::constraintAdjustments after initialization");
        return;
    }

    d->constraintAdjustments = constraintAdjustments;
    emit constraintAdjustmentsChanged();
}

QPoint XdgPositioner::offset() const
{
    Q_D(const XdgPositioner);
    return d->offset;
}

void XdgPositioner::setOffset(const QPoint &offset)
{
    Q_D(XdgPositioner);

    if (d->offset == offset)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgPositioner::offset after initialization");
        return;
    }

    d->offset = offset;
    emit offsetChanged();
}

void XdgPositioner::initialize()
{
    Q_D(XdgPositioner);

    if (d->initialized)
        return;

    d->initialized = true;

    d->init(XdgWmBasePrivate::get(d->xdgWmBase)->create_positioner());
    if (!d->size.isEmpty())
        d->set_size(d->size.width(), d->size.height());
    if (!d->anchorRect.isEmpty())
        d->set_anchor_rect(d->anchorRect.x(), d->anchorRect.y(), d->anchorRect.width(), d->anchorRect.height());
    d->set_anchor(static_cast<uint32_t>(d->anchor));
    d->set_gravity(static_cast<uint32_t>(d->gravity));
    d->set_constraint_adjustment(static_cast<uint32_t>(d->constraintAdjustments));
    d->set_offset(d->offset.x(), d->offset.y());
}

/*
 * XdgSurfacePrivate
 */

XdgSurfacePrivate::~XdgSurfacePrivate()
{
    destroy();
}

void XdgSurfacePrivate::xdg_surface_configure(uint32_t serial)
{
    ack_configure(serial);
}

/*
 * XdgSurface
 */

XdgSurface::XdgSurface(QObject *parent)
    : QObject(parent)
    , d_ptr(new XdgSurfacePrivate())
{
}

XdgSurface::~XdgSurface()
{
    delete d_ptr;
}

bool XdgSurface::isInitialized() const
{
    Q_D(const XdgSurface);
    return d->initialized;
}

XdgWmBase *XdgSurface::xdgWmBase() const
{
    Q_D(const XdgSurface);
    return d->xdgWmBase;
}

void XdgSurface::setXdgWmBase(XdgWmBase *xdgWmBase)
{
    Q_D(XdgSurface);

    if (d->xdgWmBase == xdgWmBase)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgSurface::xdgWmBase after initialization");
        return;
    }

    d->xdgWmBase = xdgWmBase;
    emit xdgWmBaseChanged();
}

QWindow *XdgSurface::window() const
{
    Q_D(const XdgSurface);
    return d->window;
}

void XdgSurface::setWindow(QWindow *window)
{
    Q_D(XdgSurface);

    if (d->window == window)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgSurface::window after initialization");
        return;
    }

    d->window = window;
    d->window->create();
    d->surface = getWlSurface(window);
    emit windowChanged();
}

void XdgSurface::initialize()
{
    Q_D(XdgSurface);

    if (d->initialized)
        return;

    if (!d->xdgWmBase) {
        qCWarning(lcXdgShell, "Unable to initialize XdgSurface: xdgWmBase was not assigned");
        return;
    }

    if (!d->window) {
        qCWarning(lcXdgShell, "Unable to initialize XdgSurface: window was not assigned");
        return;
    }

    d->initialized = true;

    auto *xdgWmBasePriv = XdgWmBasePrivate::get(d->xdgWmBase);
    d->init(xdgWmBasePriv->get_xdg_surface(d->surface));
}

/*
 * XdgPopupPrivate
 */

XdgPopupPrivate::XdgPopupPrivate(XdgPopup *self)
    : QtWayland::xdg_popup()
    , q_ptr(self)
{
}

XdgPopupPrivate::~XdgPopupPrivate()
{
    destroy();
}

void XdgPopupPrivate::xdg_popup_configure(int32_t x, int32_t y, int32_t width, int32_t height)
{
    Q_Q(XdgPopup);
    emit q->configured(QRect(x, y, width, height));
}

void XdgPopupPrivate::xdg_popup_popup_done()
{
    Q_Q(XdgPopup);
    emit q->done();
}

/*
 * XdgPopup
 */

XdgPopup::XdgPopup(QObject *parent)
    : QObject(parent)
    , d_ptr(new XdgPopupPrivate(this))
{
}

XdgPopup::~XdgPopup()
{
    delete d_ptr;
}

bool XdgPopup::isInitialized() const
{
    Q_D(const XdgPopup);
    return d->initialized;
}

XdgSurface *XdgPopup::xdgSurfaceParent() const
{
    Q_D(const XdgPopup);
    return d->xdgSurfaceParent;
}

void XdgPopup::setXdgSurfaceParent(XdgSurface *xdgSurfaceParent)
{
    Q_D(XdgPopup);

    if (d->xdgSurfaceParent == xdgSurfaceParent)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgPopup::xdgSurfaceParent after initialization");
        return;
    }

    d->xdgSurfaceParent = xdgSurfaceParent;
    emit xdgSurfaceParentChanged();
}

XdgSurface *XdgPopup::xdgSurface() const
{
    Q_D(const XdgPopup);
    return d->xdgSurface;
}

void XdgPopup::setXdgSurface(XdgSurface *xdgSurface)
{
    Q_D(XdgPopup);

    if (d->xdgSurface == xdgSurface)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgPopup::xdgSurface after initialization");
        return;
    }

    d->xdgSurface = xdgSurface;
    emit xdgSurfaceChanged();
}

XdgPositioner *XdgPopup::xdgPositioner() const
{
    Q_D(const XdgPopup);
    return d->xdgPositioner;
}

void XdgPopup::setXdgPositioner(XdgPositioner *xdgPositioner)
{
    Q_D(XdgPopup);

    if (d->xdgPositioner == xdgPositioner)
        return;

    if (d->initialized) {
        qCWarning(lcXdgShell, "Cannot change XdgPopup::xdgPositioner after initialization");
        return;
    }

    d->xdgPositioner = xdgPositioner;
    emit xdgPositionerChanged();
}

void XdgPopup::grab()
{
    Q_D(XdgPopup);
    d->grab(getWlSeat(), 0);
}

void XdgPopup::initialize()
{
    Q_D(XdgPopup);

    if (d->initialized)
        return;

    if (!d->xdgSurface) {
        for (auto *p = parent(); p != nullptr; p = p->parent()) {
            if (auto *s = qobject_cast<XdgSurface *>(p)) {
                d->xdgSurface = s;
                emit xdgSurfaceChanged();
                break;
            }
        }
    }

    if (!d->xdgSurface) {
        qCWarning(lcXdgShell, "Unable to initialize XdgPopup: xdgSurface was not assigned");
        return;
    }

    if (!d->xdgPositioner) {
        qCWarning(lcXdgShell, "Unable to initialize XdgPopup: xdgPositioner was not assigned");
        return;
    }

    d->initialized = true;

    auto *xdgSurfacePriv  = XdgSurfacePrivate::get(d->xdgSurface);
    auto *xdgSurfaceParent = d->xdgSurfaceParent ? XdgSurfacePrivate::get(d->xdgSurfaceParent)->object() : nullptr;
    auto *xdgPositioner = XdgPositionerPrivate::get(d->xdgPositioner)->object();
    d->init(xdgSurfacePriv->get_popup(xdgSurfaceParent, xdgPositioner));
}
