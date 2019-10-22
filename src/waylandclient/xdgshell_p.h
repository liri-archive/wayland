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

#ifndef LIRI_XDGSHELL_CLIENT_P_H
#define LIRI_XDGSHELL_CLIENT_P_H

#include <LiriWaylandClient/XdgShell>
#include <LiriWaylandClient/private/qwayland-xdg-shell.h>

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

class LIRIWAYLANDCLIENT_EXPORT XdgWmBasePrivate : public QtWayland::xdg_wm_base
{
    Q_DECLARE_PUBLIC(XdgWmBase)
public:
    XdgWmBasePrivate(XdgWmBase *self);

    static XdgWmBasePrivate *get(XdgWmBase *xdgWmBase) { return xdgWmBase ? xdgWmBase->d_func() : nullptr; }

protected:
    XdgWmBase *q_ptr;

    void xdg_wm_base_ping(uint32_t serial) override;
};

class LIRIWAYLANDCLIENT_EXPORT XdgPositionerPrivate : public QtWayland::xdg_positioner
{
public:
    XdgPositionerPrivate() = default;
    ~XdgPositionerPrivate();

    bool initialized = false;
    XdgWmBase *xdgWmBase = nullptr;
    QSize size;
    QRect anchorRect;
    XdgPositioner::Anchor anchor = XdgPositioner::NoAnchor;
    XdgPositioner::Gravity gravity = XdgPositioner::NoGravity;
    XdgPositioner::ConstraintAdjustments constraintAdjustments = XdgPositioner::NoConstraintAdjustment;
    QPoint offset;

    static XdgPositionerPrivate *get(XdgPositioner *xdgPositioner) { return xdgPositioner ? xdgPositioner->d_func() : nullptr; }
};

class LIRIWAYLANDCLIENT_EXPORT XdgSurfacePrivate : public QtWayland::xdg_surface
{
public:
    XdgSurfacePrivate() = default;
    ~XdgSurfacePrivate();

    bool initialized = false;
    XdgWmBase *xdgWmBase = nullptr;
    QWindow *window = nullptr;
    struct ::wl_surface *surface = nullptr;

    static XdgSurfacePrivate *get(XdgSurface *surface) { return surface ? surface->d_func() : nullptr; }

protected:
    void xdg_surface_configure(uint32_t serial) override;
};

class LIRIWAYLANDCLIENT_EXPORT XdgPopupPrivate : public QtWayland::xdg_popup
{
    Q_DECLARE_PUBLIC(XdgPopup)
public:
    explicit XdgPopupPrivate(XdgPopup *self);
    ~XdgPopupPrivate();

    static XdgPopupPrivate *get(XdgPopup *xdgPopup) { return xdgPopup ? xdgPopup->d_func() : nullptr; }

    bool initialized = false;
    XdgSurface *xdgSurfaceParent = nullptr;
    XdgSurface *xdgSurface = nullptr;
    XdgPositioner *xdgPositioner = nullptr;

protected:
    XdgPopup *q_ptr;

    void xdg_popup_configure(int32_t x, int32_t y, int32_t width, int32_t height) override;
    void xdg_popup_popup_done() override;
};

#endif // LIRI_XDGSHELL_CLIENT_P_H
