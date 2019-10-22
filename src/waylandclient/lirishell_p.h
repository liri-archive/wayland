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

#ifndef LIRI_LIRISHELL_P_CLIENT_H
#define LIRI_LIRISHELL_P_CLIENT_H

#include <LiriWaylandClient/LiriShell>
#include <LiriWaylandClient/private/qwayland-liri-shell.h>

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

class LiriShellPrivate : public QtWayland::liri_shell
{
    Q_DECLARE_PUBLIC(LiriShell)
public:
    LiriShellPrivate(LiriShell *qq);

    void setCursorShape(QWindow *window, LiriShell::GrabCursor cursor);

    QWindow *grabWindow = nullptr;

protected:
    LiriShell *q_ptr;

private:
    void liri_shell_grab_cursor(uint32_t cursor) override;
    void liri_shell_quit() override;
};

#endif // LIRI_LIRISHELL_P_CLIENT_H
