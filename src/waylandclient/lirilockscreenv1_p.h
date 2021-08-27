// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_LIRILOCKSCREENV1_P_H
#define LIRI_LIRILOCKSCREENV1_P_H

#include <LiriWaylandClient/LiriLockScreenV1>
#include <LiriWaylandClient/private/qwayland-liri-lockscreen-unstable-v1.h>

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

class LiriLockScreenV1Handle : public QtWayland::zliri_lockscreen_handle_v1
{
public:
    LiriLockScreenV1Handle() = default;
    ~LiriLockScreenV1Handle();

    LiriLockScreenV1 *manager = nullptr;

protected:
    void zliri_lockscreen_handle_v1_locked() override;
};

class LiriLockScreenV1Private : public QtWayland::zliri_lockscreen_v1
{
    Q_DECLARE_PUBLIC(LiriLockScreenV1)
public:
    LiriLockScreenV1Private(LiriLockScreenV1 *self);
    ~LiriLockScreenV1Private();

    LiriLockScreenV1Handle *handle = nullptr;

protected:
    LiriLockScreenV1 *q_ptr = nullptr;
};

#endif // LIRI_LIRILOCKSCREENV1_P_H
