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

#ifndef LIRI_WAYLANDWLRSCREENCOPYV1_P_H
#define LIRI_WAYLANDWLRSCREENCOPYV1_P_H

#include <LiriWaylandServer/WaylandWlrScreencopyManagerV1>
#include <LiriWaylandServer/private/qwayland-server-wlr-screencopy-unstable-v1.h>

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

class WaylandWlrScreencopyFrameEventFilter;

class LIRIWAYLANDSERVER_EXPORT WaylandWlrScreencopyManagerV1Private
        : public QtWaylandServer::zwlr_screencopy_manager_v1
{
    Q_DECLARE_PUBLIC(WaylandWlrScreencopyManagerV1)
public:
    explicit WaylandWlrScreencopyManagerV1Private(WaylandWlrScreencopyManagerV1 *self);

protected:
    WaylandWlrScreencopyManagerV1 *q_ptr;

    void zwlr_screencopy_manager_v1_capture_output(Resource *resource,
                                                   uint32_t frame,
                                                   int32_t overlay_cursor,
                                                   struct ::wl_resource *output_res) override;
    void zwlr_screencopy_manager_v1_capture_output_region(Resource *resource,
                                                          uint32_t frame,
                                                          int32_t overlay_cursor,
                                                          struct ::wl_resource *output_res,
                                                          int32_t x, int32_t y,
                                                          int32_t width, int32_t height) override;
    void zwlr_screencopy_manager_v1_destroy(Resource *resource) override;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrScreencopyFrameV1Private
        : public QtWaylandServer::zwlr_screencopy_frame_v1
{
    Q_DECLARE_PUBLIC(WaylandWlrScreencopyFrameV1)
public:
    explicit WaylandWlrScreencopyFrameV1Private(WaylandWlrScreencopyFrameV1 *self);
    ~WaylandWlrScreencopyFrameV1Private();

    void setup();
    void copy(Resource *resource, struct ::wl_resource *buffer_res);

    WaylandWlrScreencopyFrameEventFilter *filterObject = nullptr;
    bool overlayCursor = false;
    QWaylandOutput *output = nullptr;
    QRect rect;
    WaylandWlrScreencopyFrameV1::Flags flags = 0;
    bool withDamage = false;
    QRect damageRect;
    uint32_t stride = 0;
    wl_shm_format requestedBufferFormat = WL_SHM_FORMAT_ABGR8888;
    struct ::wl_shm_buffer *buffer = nullptr;
    quint32 tv_sec_hi = 0, tv_sec_lo = 0;
    bool ready = false;

    static WaylandWlrScreencopyFrameV1Private *get(WaylandWlrScreencopyFrameV1 *frame) { return frame ? frame->d_func() : nullptr; }

protected:
    WaylandWlrScreencopyFrameV1 *q_ptr;

    void zwlr_screencopy_frame_v1_destroy_resource(Resource *resource) override;
    void zwlr_screencopy_frame_v1_copy(Resource *resource,
                                       struct ::wl_resource *buffer_res) override;
    void zwlr_screencopy_frame_v1_destroy(Resource *resource) override;
    void zwlr_screencopy_frame_v1_copy_with_damage(Resource *resource,
                                                   struct ::wl_resource *buffer_res) override;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrScreencopyFrameEventFilter
        : public QObject
{
    Q_OBJECT
public:
    explicit WaylandWlrScreencopyFrameEventFilter(QObject *parent = nullptr);

    WaylandWlrScreencopyFrameV1Private::Resource *resource = nullptr;
    struct ::wl_resource *buffer_res = nullptr;
    WaylandWlrScreencopyFrameV1Private *frame = nullptr;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};


#endif // LIRI_WAYLANDWLRSCREENCOPYV1_P_H
