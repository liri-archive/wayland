// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_WLREXPORTDMABUFV1_P_CLIENT_H
#define LIRI_WLREXPORTDMABUFV1_P_CLIENT_H

#include <LiriWaylandClient/WlrExportDmabufV1>
#include <LiriWaylandClient/private/qwayland-wlr-export-dmabuf-unstable-v1.h>

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

class LIRIWAYLANDCLIENT_EXPORT WlrExportDmabufManagerV1Private
        : public QtWayland::zwlr_export_dmabuf_manager_v1
{
public:
    WlrExportDmabufManagerV1Private() = default;
    ~WlrExportDmabufManagerV1Private();
};

class LIRIWAYLANDCLIENT_EXPORT WlrExportDmabufFrameV1Private
        : public QtWayland::zwlr_export_dmabuf_frame_v1
{
    Q_DECLARE_PUBLIC(WlrExportDmabufFrameV1)
public:
    explicit WlrExportDmabufFrameV1Private(WlrExportDmabufFrameV1 *self);
    ~WlrExportDmabufFrameV1Private();

    static WlrExportDmabufFrameV1Private *get(WlrExportDmabufFrameV1 *self) { return self->d_func(); }

protected:
    WlrExportDmabufFrameV1 *q_ptr = nullptr;

    void zwlr_export_dmabuf_frame_v1_frame(uint32_t width, uint32_t height,
                                           uint32_t offset_x, uint32_t offset_y,
                                           uint32_t buffer_flags, uint32_t flags,
                                           uint32_t format,
                                           uint32_t mod_high, uint32_t mod_low,
                                           uint32_t num_objects) override;
    void zwlr_export_dmabuf_frame_v1_object(uint32_t index, int32_t fd, uint32_t size,
                                            uint32_t offset, uint32_t stride,
                                            uint32_t plane_index) override;
    void zwlr_export_dmabuf_frame_v1_ready(uint32_t tv_sec_hi, uint32_t tv_sec_lo,
                                           uint32_t tv_nsec) override;
    void zwlr_export_dmabuf_frame_v1_cancel(uint32_t reason) override;
};

#endif // LIRI_WLREXPORTDMABUFV1_P_CLIENT_H
