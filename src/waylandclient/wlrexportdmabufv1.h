// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_WLREXPORTDMABUFV1_CLIENT_H
#define LIRI_WLREXPORTDMABUFV1_CLIENT_H

#include <QWaylandClientExtension>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

QT_FORWARD_DECLARE_CLASS(QScreen)

class WlrExportDmabufManagerV1Private;
class WlrExportDmabufFrameV1;
class WlrExportDmabufFrameV1Private;

class LIRIWAYLANDCLIENT_EXPORT WlrExportDmabufManagerV1
        : public QWaylandClientExtensionTemplate<WlrExportDmabufManagerV1>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrExportDmabufManagerV1)
public:
    explicit WlrExportDmabufManagerV1();
    ~WlrExportDmabufManagerV1();

    void init(struct ::wl_registry *registry, int id, int version);

    Q_INVOKABLE WlrExportDmabufFrameV1 *captureOutput(bool overlayCursor, QScreen *screen);

    static const wl_interface *interface();

private:
    WlrExportDmabufManagerV1Private *const d_ptr = nullptr;
};

class LIRIWAYLANDCLIENT_EXPORT WlrExportDmabufFrameV1
        : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrExportDmabufFrameV1)
public:
    enum BufferFlag {
        YInvert = 1,
        Interlaced = 2,
        BottomFirst = 4
    };
    Q_ENUM(BufferFlag)
    Q_DECLARE_FLAGS(BufferFlags, BufferFlag)
    Q_FLAG(BufferFlags)

    enum FrameFlag {
        Transient = 1
    };
    Q_ENUM(FrameFlag)
    Q_DECLARE_FLAGS(FrameFlags, FrameFlag)
    Q_FLAG(FrameFlags)

    enum CancelReason {
        Temporary = 0,
        Permanent,
        Resizing
    };
    Q_ENUM(CancelReason)

    explicit WlrExportDmabufFrameV1(QObject *parent = nullptr);
    ~WlrExportDmabufFrameV1();

Q_SIGNALS:
    void frame(const QSize &size, const QPoint &offset,
               BufferFlags bufferFlags, FrameFlags flags,
               quint32 format, quint64 modifier,
               quint32 numObjects);
    void object(quint32 index, quint32 fd, quint32 size,
                quint32 offset, quint32 stride,
                quint32 planeIndex);
    void ready(quint64 tv_sec, quint32 tv_nsec);
    void cancel(CancelReason reason);

private:
    WlrExportDmabufFrameV1Private *const d_ptr = nullptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(WlrExportDmabufFrameV1::BufferFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(WlrExportDmabufFrameV1::FrameFlags)

#endif // LIRI_WLREXPORTDMABUFV1_CLIENT_H
