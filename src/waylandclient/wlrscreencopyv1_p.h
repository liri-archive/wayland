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

#ifndef LIRI_WLRSCREENCOPYV1_P_CLIENT_H
#define LIRI_WLRSCREENCOPYV1_P_CLIENT_H

#include <LiriWaylandClient/WlrScreencopyV1>
#include <LiriWaylandClient/private/qwayland-wlr-screencopy-unstable-v1.h>

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

class LIRIWAYLANDCLIENT_EXPORT WlrScreencopyManagerV1Private
        : public QtWayland::zwlr_screencopy_manager_v1
{
public:
    explicit WlrScreencopyManagerV1Private();
    ~WlrScreencopyManagerV1Private();

    wl_shm *shm = nullptr;

    static void global(void *data, struct ::wl_registry *registry, uint32_t id, const char *interface, uint32_t version);
    static void globalRemove(void *data, struct ::wl_registry *registry, uint32_t id);
};

class LIRIWAYLANDCLIENT_EXPORT WlrScreencopyFrameV1Private
        : public QtWayland::zwlr_screencopy_frame_v1
{
    Q_DECLARE_PUBLIC(WlrScreencopyFrameV1)
public:
    explicit WlrScreencopyFrameV1Private(WlrScreencopyFrameV1 *self);
    ~WlrScreencopyFrameV1Private();

    wl_buffer *createShmBuffer(quint32 format, const QSize &size, quint32 stride, uchar **out);

    WlrScreencopyManagerV1Private *parent = nullptr;
    QScreen *screen = nullptr;
    struct {
        quint32 format = 0;
        QSize size = QSize(0, 0);
        quint32 stride = 0;
        wl_buffer *shmBuffer = nullptr;
        uchar *data = nullptr;
    } buffer;
    bool yInverted = false;

    static WlrScreencopyFrameV1Private *get(WlrScreencopyFrameV1 *frame) { return frame ? frame->d_func() : nullptr; }

protected:
    WlrScreencopyFrameV1 *q_ptr;

    void zwlr_screencopy_frame_v1_buffer(uint32_t format, uint32_t width, uint32_t height, uint32_t stride) override;
    void zwlr_screencopy_frame_v1_flags(uint32_t flags) override;
    void zwlr_screencopy_frame_v1_ready(uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec) override;
    void zwlr_screencopy_frame_v1_failed() override;
    void zwlr_screencopy_frame_v1_damage(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
};

#endif // LIRI_WLRSCREENCOPYV1_P_CLIENT_H
