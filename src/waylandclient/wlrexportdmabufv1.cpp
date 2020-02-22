// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QScreen>

#include "wlrexportdmabufv1_p.h"
#include "utils_p.h"

WlrExportDmabufManagerV1Private::~WlrExportDmabufManagerV1Private()
{
    destroy();
}


WlrExportDmabufManagerV1::WlrExportDmabufManagerV1()
    : QWaylandClientExtensionTemplate<WlrExportDmabufManagerV1>(1)
    , d_ptr(new WlrExportDmabufManagerV1Private)
{
}

WlrExportDmabufManagerV1::~WlrExportDmabufManagerV1()
{
    delete d_ptr;
}

void WlrExportDmabufManagerV1::init(struct ::wl_registry *registry, int id, int version)
{
    Q_D(WlrExportDmabufManagerV1);
    d->init(registry, id, version);
}

WlrExportDmabufFrameV1 *WlrExportDmabufManagerV1::captureOutput(bool overlayCursor, QScreen *screen)
{
    Q_D(WlrExportDmabufManagerV1);

    auto *object = d->capture_output(overlayCursor ? 1 : 0, getWlOutput(screen));
    auto *frame = new WlrExportDmabufFrameV1(this);
    WlrExportDmabufFrameV1Private::get(frame)->init(object);
    return frame;
}

const wl_interface *WlrExportDmabufManagerV1::interface()
{
    return WlrExportDmabufManagerV1Private::interface();
}


WlrExportDmabufFrameV1Private::WlrExportDmabufFrameV1Private(WlrExportDmabufFrameV1 *self)
    : QtWayland::zwlr_export_dmabuf_frame_v1()
    , q_ptr(self)
{
}

WlrExportDmabufFrameV1Private::~WlrExportDmabufFrameV1Private()
{
    destroy();
}

void WlrExportDmabufFrameV1Private::zwlr_export_dmabuf_frame_v1_frame(uint32_t width, uint32_t height,
                                                                      uint32_t offset_x, uint32_t offset_y,
                                                                      uint32_t buffer_flags, uint32_t flags,
                                                                      uint32_t format,
                                                                      uint32_t mod_high, uint32_t mod_low,
                                                                      uint32_t num_objects)
{
    Q_Q(WlrExportDmabufFrameV1);

    quint64 modifier = ((quint64) mod_high << 32) | mod_low;
    Q_EMIT q->frame(QSize(width, height), QPoint(offset_x, offset_y),
                    static_cast<WlrExportDmabufFrameV1::BufferFlags>(buffer_flags),
                    static_cast<WlrExportDmabufFrameV1::FrameFlags>(flags),
                    format, modifier, num_objects);
}

void WlrExportDmabufFrameV1Private::zwlr_export_dmabuf_frame_v1_object(uint32_t index, int32_t fd,
                                                                       uint32_t size, uint32_t offset,
                                                                       uint32_t stride, uint32_t plane_index)
{
    Q_Q(WlrExportDmabufFrameV1);

    Q_EMIT q->object(index, fd, size, offset, stride, plane_index);
}

void WlrExportDmabufFrameV1Private::zwlr_export_dmabuf_frame_v1_ready(uint32_t tv_sec_hi, uint32_t tv_sec_lo,
                                                                      uint32_t tv_nsec)
{
    Q_Q(WlrExportDmabufFrameV1);

    quint64 tv_sec = ((quint64) tv_sec_hi << 32) | tv_sec_lo;
    Q_EMIT q->ready(tv_sec, tv_nsec);
}

void WlrExportDmabufFrameV1Private::zwlr_export_dmabuf_frame_v1_cancel(uint32_t reason)
{
    Q_Q(WlrExportDmabufFrameV1);

    Q_EMIT q->cancel(static_cast<WlrExportDmabufFrameV1::CancelReason>(reason));
}


WlrExportDmabufFrameV1::WlrExportDmabufFrameV1(QObject *parent)
    : QObject(parent)
    , d_ptr(new WlrExportDmabufFrameV1Private(this))
{
}

WlrExportDmabufFrameV1::~WlrExportDmabufFrameV1()
{
    delete d_ptr;
}
