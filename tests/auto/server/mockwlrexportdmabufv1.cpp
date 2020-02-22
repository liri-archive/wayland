// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mockwlrexportdmabufv1.h"

MockWlrExportDmabufManagerV1::MockWlrExportDmabufManagerV1(struct ::wl_registry *registry, uint32_t name, uint32_t version)
    : QtWayland::zwlr_export_dmabuf_manager_v1(registry, name, version)
{
}

MockWlrExportDmabufManagerV1::~MockWlrExportDmabufManagerV1()
{
    destroy();
}

MockWlrExportDmabufFrameV1::MockWlrExportDmabufFrameV1(struct ::zwlr_export_dmabuf_frame_v1 *object)
    : QtWayland::zwlr_export_dmabuf_frame_v1(object)
{
}

MockWlrExportDmabufFrameV1::~MockWlrExportDmabufFrameV1()
{
    qDeleteAll(frame.objects);
    destroy();
}

void MockWlrExportDmabufFrameV1::zwlr_export_dmabuf_frame_v1_frame(uint32_t width, uint32_t height, uint32_t offset_x, uint32_t offset_y, uint32_t buffer_flags, uint32_t flags, uint32_t format, uint32_t mod_high, uint32_t mod_low, uint32_t num_objects)
{
    frame.size = QSize(width, height);
    frame.offset = QPoint(offset_x, offset_y);
    frame.bufferFlags = buffer_flags;
    frame.flags = flags;
    frame.format = format;
    frame.modifier = (quint64(mod_high) << 32) | mod_low;
    frame.numObjects = num_objects;
    frame.objects.reserve(num_objects);
}

void MockWlrExportDmabufFrameV1::zwlr_export_dmabuf_frame_v1_object(uint32_t index, int32_t fd, uint32_t size, uint32_t offset, uint32_t stride, uint32_t plane_index)
{
    Q_UNUSED(index)

    auto *object = new Object;
    object->fd = fd;
    object->size = size;
    object->offset = offset;
    object->stride = stride;
    object->planeIndex = plane_index;
    frame.objects.append(object);
}

void MockWlrExportDmabufFrameV1::zwlr_export_dmabuf_frame_v1_ready(uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec)
{
    time.tv_sec = (quint64(tv_sec_hi) << 32) | tv_sec_lo;
    time.tv_nsec = tv_nsec;
    ready = true;
}

void MockWlrExportDmabufFrameV1::zwlr_export_dmabuf_frame_v1_cancel(uint32_t reason)
{
    cancellation.canceled = true;
    cancellation.reason = static_cast<cancel_reason>(reason);
}
