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

#include <QtCore/QFile>
#include <QtCore/QProcess>

#include <QtWaylandCompositor/qwaylandcompositor.h>
#include <QtWaylandCompositor/QWaylandPointer>
#include <QtWaylandCompositor/QWaylandSeat>
#include <QtWaylandCompositor/QWaylandSurface>

#include "shellhelper.h"
#include "shellhelper_p.h"
#include "logging_p.h"

#ifndef WL_DISPLAY_ERROR_IMPLEMENTATION
#  define WL_DISPLAY_ERROR_IMPLEMENTATION WL_DISPLAY_ERROR_INVALID_METHOD
#endif

class ProcessRunner : public QObject
{
    Q_OBJECT
public:
    ProcessRunner(QObject *parent = nullptr)
        : QObject(parent)
        , process(new QProcess(this))
    {
        process->setProcessChannelMode(QProcess::ForwardedChannels);

        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        //env.insert(QLatin1String("WAYLAND_DEBUG"), "1");
        env.insert(QStringLiteral("XDG_SESSION_TYPE"), QStringLiteral("wayland"));
        process->setProcessEnvironment(env);

        connect(process, &QProcess::readyReadStandardOutput,
                this, &ProcessRunner::handleReadOutput);
        connect(process, &QProcess::readyReadStandardError,
                this, &ProcessRunner::handleReadError);
        connect(process, &QProcess::started,
                this, &ProcessRunner::processStarted);
        connect(process, &QProcess::errorOccurred,
                this, &ProcessRunner::handleErrorOccurred);
    }

    ~ProcessRunner()
    {
        qCDebug(lcWaylandServer) << "Stopping shell helper...";
        process->terminate();
        if (!process->waitForFinished())
            process->kill();
    }

    bool startProcess()
    {
        return runProgram(QString::asprintf("%s/liri-shell-helper", INSTALL_LIBEXECDIR));
    }

    int retries = 5;
    QProcess *process = nullptr;

Q_SIGNALS:
    void processStarted();

private Q_SLOTS:
    void handleReadOutput()
    {
        qCInfo(lcWaylandServer) << process->readAllStandardOutput();
    }

    void handleReadError()
    {
        qCCritical(lcWaylandServer) << process->readAllStandardError();
    }

    void handleErrorOccurred(QProcess::ProcessError error)
    {
        if (error != QProcess::FailedToStart || error != QProcess::Crashed)
            return;

        if (retries-- > 0) {
            qCWarning(lcWaylandServer, "Failed to start shell helper, %d attempt(s) left",
                      retries);
            startProcess();
            return;
        }

        if (retries == 0)
            qCWarning(lcWaylandServer, "Failed to start shell helper, giving up");
    }

private:
    bool runProgram(const QString &path)
    {
        if (!QFile::exists(path))
            return false;

        process->start(path);

        return true;
    }
};

/*
 * ShellHelperPrivate
 */

ShellHelperPrivate::ShellHelperPrivate(ShellHelper *qq)
    : QtWaylandServer::liri_shell()
    , processRunner(new ProcessRunner)
    , q_ptr(qq)
{
    qq->connect(processRunner, &ProcessRunner::processStarted,
                qq, &ShellHelper::processStarted);
}

ShellHelperPrivate::~ShellHelperPrivate()
{
    processRunner->deleteLater();
}

ShellHelperPrivate *ShellHelperPrivate::get(ShellHelper *shell)
{
    return shell->d_func();
}

void ShellHelperPrivate::liri_shell_bind_resource(Resource *r)
{
    // Make sure only an authorized client can bind
    pid_t pid;
    wl_client_get_credentials(r->client(), &pid, nullptr, nullptr);
    QFile file(QString::asprintf("/proc/%d/cmdline", pid));
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        QString data = stream.readAll();
        QStringList args = data.split(QLatin1Char('\0'));
        file.close();

        if (!args.first().endsWith(QStringLiteral("liri-shell-helper"))) {
            wl_resource_post_error(r->handle, WL_DISPLAY_ERROR_IMPLEMENTATION,
                                   "unauthorized client program");
            return;
        }
    } else {
        wl_resource_post_error(r->handle, WL_DISPLAY_ERROR_IMPLEMENTATION,
                               "unauthorized client program");
        return;
    }

    // Client can bind only once
    if (resource())
        wl_resource_post_error(r->handle,
                               WL_DISPLAY_ERROR_INVALID_OBJECT,
                               "client can bind only once");
}

void ShellHelperPrivate::liri_shell_set_grab_surface(Resource *resource, struct ::wl_resource *wlSurface)
{
    Q_Q(ShellHelper);

    auto surface = QWaylandSurface::fromResource(wlSurface);
    if (surface) {
        grabSurface = surface;
        Q_EMIT q->grabSurfaceAdded(surface);
    } else {
        qCWarning(lcWaylandServer) << "Couldn't find surface from resource";
        wl_resource_post_error(resource->handle, WL_DISPLAY_ERROR_INVALID_OBJECT,
                               "the specified surface is invalid");
    }
}

/*
 * ShellHelper
 */

ShellHelper::ShellHelper()
    : QWaylandCompositorExtensionTemplate<ShellHelper>()
    , d_ptr(new ShellHelperPrivate(this))
{
}

ShellHelper::ShellHelper(QWaylandCompositor *compositor)
    : QWaylandCompositorExtensionTemplate<ShellHelper>(compositor)
    , d_ptr(new ShellHelperPrivate(this))
{
}

ShellHelper::~ShellHelper()
{
    delete d_ptr;
}

void ShellHelper::initialize()
{
    Q_D(ShellHelper);

    QWaylandCompositorExtension::initialize();
    QWaylandCompositor *compositor = static_cast<QWaylandCompositor *>(extensionContainer());
    if (!compositor) {
        qCWarning(lcWaylandServer) << "Failed to find QWaylandCompositor when initializing ShellHelper";
        return;
    }
    d->init(compositor->display(), 1);
}

void ShellHelper::start()
{
    Q_D(ShellHelper);

    d->processRunner->startProcess();
}

void ShellHelper::grabCursor(GrabCursor cursor)
{
    Q_D(ShellHelper);

    if (d->grabSurface) {
        auto resource = d->resourceMap().value(d->grabSurface->waylandClient());
        if (resource)
            d->send_grab_cursor(resource->handle, static_cast<uint32_t>(cursor));
        if (d->grabSurface->views().size() > 0) {
            auto seat = d->grabSurface->compositor()->defaultSeat();
            auto view = d->grabSurface->views().at(0);
            seat->setMouseFocus(view);
            seat->sendMouseMoveEvent(view, QPointF(0, 0), QPointF(0, 0));
        }
    }
}

const struct wl_interface *ShellHelper::interface()
{
    return ShellHelperPrivate::interface();
}

QByteArray ShellHelper::interfaceName()
{
    return ShellHelperPrivate::interfaceName();
}

#include "shellhelper.moc"
