// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MOCKWLREXPORTDMABUFV1_H
#define MOCKWLREXPORTDMABUFV1_H

#include <QPoint>
#include <QSize>
#include <QVector>

#include "qwayland-wlr-export-dmabuf-unstable-v1.h"

class MockWlrExportDmabufManagerV1 : public QtWayland::zwlr_export_dmabuf_manager_v1
{
public:
    MockWlrExportDmabufManagerV1(struct ::wl_registry *registry, uint32_t name, uint32_t version);
    ~MockWlrExportDmabufManagerV1();
};

class MockWlrExportDmabufFrameV1 : public QtWayland::zwlr_export_dmabuf_frame_v1
{
public:
    MockWlrExportDmabufFrameV1(struct ::zwlr_export_dmabuf_frame_v1 *object);
    ~MockWlrExportDmabufFrameV1();

    typedef struct {
        quint32 fd = 0;
        quint32 size = 0;
        quint32 offset = 0;
        quint32 stride = 0;
        quint32 planeIndex = 0;
    } Object;

    struct {
        QSize size;
        QPoint offset;
        quint32 bufferFlags = 0;
        quint32 flags = 0;
        quint32 format = 0;
        quint64 modifier = 0;
        quint32 numObjects = 0;
        QVector<Object *> objects;
    } frame;

    struct {
        quint64 tv_sec = 0;
        quint32 tv_nsec = 0;
    } time;

    bool ready = false;

    struct {
        bool canceled = false;
        cancel_reason reason = cancel_reason_temporary;
    } cancellation;

protected:
    void zwlr_export_dmabuf_frame_v1_frame(uint32_t width, uint32_t height, uint32_t offset_x, uint32_t offset_y, uint32_t buffer_flags, uint32_t flags, uint32_t format, uint32_t mod_high, uint32_t mod_low, uint32_t num_objects)  override;
    void zwlr_export_dmabuf_frame_v1_object(uint32_t index, int32_t fd, uint32_t size, uint32_t offset, uint32_t stride, uint32_t plane_index)  override;
    void zwlr_export_dmabuf_frame_v1_ready(uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec)  override;
    void zwlr_export_dmabuf_frame_v1_cancel(uint32_t reason) override;
};

#endif // MOCKWLREXPORTDMABUFV1_H
