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

#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

#include "mockwlrscreencopyv1.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

MockWlrScreencopyManagerV1::MockWlrScreencopyManagerV1(struct ::wl_registry *registry, uint32_t name, uint32_t version)
    : QtWayland::zwlr_screencopy_manager_v1(registry, name, version)
{
}

MockWlrScreencopyFrameV1 *MockWlrScreencopyManagerV1::captureOutput(bool overlayCursor, QtWayland::wl_output *output)
{
    auto *frame = new MockWlrScreencopyFrameV1(capture_output(overlayCursor ? 1 : 0, output->object()));
    frame->parent = this;
    return frame;
}

MockWlrScreencopyFrameV1 *MockWlrScreencopyManagerV1::captureOutputRegion(bool overlayCursor, QtWayland::wl_output *output, const QRect &rect)
{
    auto *frame = new MockWlrScreencopyFrameV1(capture_output_region(overlayCursor ? 1 : 0, output->object(),
                                                                     rect.x(), rect.y(), rect.width(), rect.height()));
    frame->parent = this;
    return frame;
}


MockWlrScreencopyFrameV1::MockWlrScreencopyFrameV1(struct ::zwlr_screencopy_frame_v1 *object)
    : QtWayland::zwlr_screencopy_frame_v1(object)
{
}

MockWlrScreencopyFrameV1::~MockWlrScreencopyFrameV1()
{
    if (shmBuffer)
        wl_buffer_destroy(shmBuffer);
}

wl_buffer *MockWlrScreencopyFrameV1::createShmBuffer(quint32 format, const QSize &size, quint32 stride, uchar **out)
{
    int bytesSize = stride * size.height();

    const char name[] = "/liri-waylandclient-screencopy";
    int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0);
    if (fd < 0) {
        qWarning("Failed to open shared memory: %s", strerror(errno));
        return nullptr;
    }
    shm_unlink(name);

    int ret;
    while ((ret = ftruncate(fd, bytesSize)) == EINTR) {
        // no-op
    }
    if (ret < 0) {
        close(fd);
        qWarning("Failed to set shared memory size: %s", strerror(errno));
        return nullptr;
    }

    uchar *data = static_cast<uchar *>(mmap(nullptr, bytesSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (data == (uchar *)MAP_FAILED) {
        close(fd);
        qWarning("Failed to map shared memory: %s", strerror(errno));
        return nullptr;
    }

    auto *pool = wl_shm_create_pool(parent->shm->object(), fd, bytesSize);
    close(fd);
    auto *shmBuffer = wl_shm_pool_create_buffer(pool, 0, size.width(), size.height(), stride, format);
    wl_shm_pool_destroy(pool);

    *out = data;
    return shmBuffer;
}

void MockWlrScreencopyFrameV1::zwlr_screencopy_frame_v1_buffer(uint32_t format, uint32_t width, uint32_t height, uint32_t stride)
{
    uchar *data = nullptr;
    shmBuffer = createShmBuffer(format, QSize(width, height), stride, &data);
    if (!shmBuffer) {
        qWarning("Failed to create buffer");
        failed = true;
        return;
    }

    copy(shmBuffer);
}

void MockWlrScreencopyFrameV1::zwlr_screencopy_frame_v1_flags(uint32_t flags)
{
    yInverted = flags & flags_y_invert;
}

void MockWlrScreencopyFrameV1::zwlr_screencopy_frame_v1_ready(uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec)
{
    Q_UNUSED(tv_sec_hi)
    Q_UNUSED(tv_sec_lo)
    Q_UNUSED(tv_nsec)

    ready = true;
}

void MockWlrScreencopyFrameV1::zwlr_screencopy_frame_v1_failed()
{
    failed = true;
}

void MockWlrScreencopyFrameV1::zwlr_screencopy_frame_v1_damage(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    damagedRect = QRect(x, y, width, height);
}
