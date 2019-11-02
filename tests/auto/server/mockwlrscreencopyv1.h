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

#ifndef MOCKWLRSCREENCOPYV1_H
#define MOCKWLRSCREENCOPYV1_H

#include <QRect>

#include "qwayland-wayland.h"
#include "qwayland-wlr-screencopy-unstable-v1.h"

class MockWlrScreencopyFrameV1;

class MockWlrScreencopyManagerV1 : public QtWayland::zwlr_screencopy_manager_v1
{
public:
    MockWlrScreencopyManagerV1(struct ::wl_registry *registry, uint32_t name, uint32_t version);

    MockWlrScreencopyFrameV1 *captureOutput(bool overlayCursor, QtWayland::wl_output *output);
    MockWlrScreencopyFrameV1 *captureOutputRegion(bool overlayCursor, QtWayland::wl_output *output, const QRect &rect);

    QtWayland::wl_shm *shm = nullptr;
};

class MockWlrScreencopyFrameV1 : public QtWayland::zwlr_screencopy_frame_v1
{
public:
    MockWlrScreencopyFrameV1(struct ::zwlr_screencopy_frame_v1 *object);
    ~MockWlrScreencopyFrameV1();

    wl_buffer *createShmBuffer(quint32 format, const QSize &size, quint32 stride, uchar **out);

    MockWlrScreencopyManagerV1 *parent = nullptr;
    wl_buffer *shmBuffer = nullptr;
    bool yInverted = false;
    bool ready = false;
    bool failed = false;
    QRect damagedRect;

protected:
    void zwlr_screencopy_frame_v1_buffer(uint32_t format, uint32_t width, uint32_t height, uint32_t stride) override;
    void zwlr_screencopy_frame_v1_flags(uint32_t flags) override;
    void zwlr_screencopy_frame_v1_ready(uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec) override;
    void zwlr_screencopy_frame_v1_failed() override;
    void zwlr_screencopy_frame_v1_damage(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
};

#endif // MOCKWLRSCREENCOPYV1_H
