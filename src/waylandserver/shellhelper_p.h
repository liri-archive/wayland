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

#ifndef LIRI_LIRISHELL_P_H
#define LIRI_LIRISHELL_P_H

#include <LiriWaylandServer/ShellHelper>
#include <LiriWaylandServer/private/qwayland-server-shell-helper.h>

QT_FORWARD_DECLARE_CLASS(QWaylandSurface)

class ProcessRunner;
class ShellHelper;

class LIRIWAYLANDSERVER_EXPORT ShellHelperPrivate : public QtWaylandServer::liri_shell
{
    Q_DECLARE_PUBLIC(ShellHelper)
public:
    explicit ShellHelperPrivate(ShellHelper *qq);
    ~ShellHelperPrivate();

    static ShellHelperPrivate *get(ShellHelper *shell);

    ProcessRunner *processRunner = nullptr;
    QWaylandSurface *grabSurface = nullptr;

protected:
    ShellHelper *q_ptr;

private:
    void liri_shell_bind_resource(Resource *r) override;
    void liri_shell_set_grab_surface(Resource *resource, struct ::wl_resource *wlSurface) override;
};

#endif // LIRI_LIRISHELL_P_H
