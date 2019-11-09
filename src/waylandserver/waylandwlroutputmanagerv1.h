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

#ifndef LIRI_WAYLANDWLROUTPUTMANAGERV1_H
#define LIRI_WAYLANDWLROUTPUTMANAGERV1_H

#include <QWaylandCompositor>
#include <QWaylandCompositorExtension>
#include <QWaylandOutput>
#include <qwaylandquickchildren.h>
#include <QWaylandResource>

#include <LiriWaylandServer/liriwaylandserverglobal.h>

class QWaylandClient;

class WaylandWlrOutputConfigurationV1;
class WaylandWlrOutputConfigurationV1Private;
class WaylandWlrOutputConfigurationHeadV1;
class WaylandWlrOutputConfigurationHeadV1Private;
class WaylandWlrOutputManagerV1Private;
class WaylandWlrOutputHeadV1;
class WaylandWlrOutputHeadV1Private;
class WaylandWlrOutputModeV1;
class WaylandWlrOutputModeV1Private;

class LIRIWAYLANDSERVER_EXPORT WaylandWlrOutputManagerV1 : public QWaylandCompositorExtensionTemplate<WaylandWlrOutputManagerV1>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrOutputManagerV1)
    Q_PROPERTY(QWaylandCompositor *compositor READ compositor WRITE setCompositor NOTIFY compositorChanged)
public:
    explicit WaylandWlrOutputManagerV1();
    WaylandWlrOutputManagerV1(QWaylandCompositor *compositor);
    ~WaylandWlrOutputManagerV1();

    void initialize() override;

    QWaylandCompositor *compositor() const;
    void setCompositor(QWaylandCompositor *compositor);

    QVector<WaylandWlrOutputHeadV1 *> heads() const;

    Q_INVOKABLE void done(quint32 serial);
    Q_INVOKABLE void finished();

    static const wl_interface *interface();
    static QByteArray interfaceName();

Q_SIGNALS:
    void compositorChanged();
    void headAdded(WaylandWlrOutputHeadV1 *head);
    void configurationRequested(const QWaylandResource &resource);
    void configurationCreated(WaylandWlrOutputConfigurationV1 *configuration);
    void clientStopped(QWaylandClient *client);

private:
    WaylandWlrOutputManagerV1Private *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrOutputHeadV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrOutputHeadV1)
    Q_PROPERTY(WaylandWlrOutputManagerV1 *manager READ manager WRITE setManager NOTIFY managerChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QSize physicalSize READ physicalSize WRITE setPhysicalSize NOTIFY physicalSizeChanged)
    Q_PROPERTY(QPoint position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(WaylandWlrOutputModeV1 *currentMode READ currentMode WRITE setCurrentMode NOTIFY currentModeChanged)
    Q_PROPERTY(WaylandWlrOutputModeV1 *preferredMode READ preferredMode WRITE setPreferredMode NOTIFY preferredModeChanged)
    Q_PROPERTY(QWaylandOutput::Transform transform READ transform WRITE setTransform NOTIFY transformChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
public:
    explicit WaylandWlrOutputHeadV1(QObject *parent = nullptr);
    ~WaylandWlrOutputHeadV1();

    bool isInitialized() const;
    void initialize();

    WaylandWlrOutputManagerV1 *manager() const;
    void setManager(WaylandWlrOutputManagerV1 *manager);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &description);

    QSize physicalSize() const;
    void setPhysicalSize(const QSize &physicalSize);

    QPoint position() const;
    void setPosition(const QPoint &position);

    QVector<WaylandWlrOutputModeV1 *> modes() const;
    Q_INVOKABLE void addMode(WaylandWlrOutputModeV1 *mode);

    WaylandWlrOutputModeV1 *currentMode() const;
    void setCurrentMode(WaylandWlrOutputModeV1 *mode);

    WaylandWlrOutputModeV1 *preferredMode() const;
    void setPreferredMode(WaylandWlrOutputModeV1 *mode);

    QWaylandOutput::Transform transform() const;
    void setTransform(QWaylandOutput::Transform transform);

    qreal scale() const;
    void setScale(qreal scale);

Q_SIGNALS:
    void managerChanged();
    void enabledChanged();
    void nameChanged();
    void descriptionChanged();
    void physicalSizeChanged();
    void positionChanged();
    void modeAdded(WaylandWlrOutputModeV1 *mode);
    void modesChanged();
    void currentModeChanged();
    void preferredModeChanged();
    void transformChanged();
    void scaleChanged();

private:
    WaylandWlrOutputHeadV1Private *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrOutputModeV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrOutputModeV1)
    Q_WAYLAND_COMPOSITOR_DECLARE_QUICK_CHILDREN(WaylandWlrOutputModeV1)
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(qint32 refresh READ refresh WRITE setRefresh NOTIFY refreshChanged)
public:
    explicit WaylandWlrOutputModeV1(QObject *parent = nullptr);
    ~WaylandWlrOutputModeV1();

    bool isInitialized() const;

    QSize size() const;
    void setSize(const QSize &size);

    qint32 refresh() const;
    void setRefresh(qint32 refresh);

Q_SIGNALS:
    void sizeChanged();
    void refreshChanged();

private:
    WaylandWlrOutputModeV1Private *const d_ptr;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrOutputConfigurationHeadV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrOutputConfigurationHeadV1)
    Q_PROPERTY(WaylandWlrOutputHeadV1 *head READ head CONSTANT)
    Q_PROPERTY(WaylandWlrOutputModeV1 *mode READ mode NOTIFY modeChanged)
    Q_PROPERTY(QSize customModeSize READ customModeSize NOTIFY customModeChanged)
    Q_PROPERTY(qint32 customModeRefresh READ customModeRefresh NOTIFY customModeChanged)
    Q_PROPERTY(QPoint position READ position NOTIFY positionChanged)
    Q_PROPERTY(QWaylandOutput::Transform transform READ transform NOTIFY transformChanged)
    Q_PROPERTY(qreal scale READ scale NOTIFY scaleChanged)
public:
    ~WaylandWlrOutputConfigurationHeadV1();

    WaylandWlrOutputHeadV1 *head() const;
    WaylandWlrOutputModeV1 *mode() const;
    QSize customModeSize() const;
    qint32 customModeRefresh() const;
    QPoint position() const;
    QWaylandOutput::Transform transform() const;
    qreal scale() const;

Q_SIGNALS:
    void modeChanged(WaylandWlrOutputModeV1 *mode);
    void customModeChanged(const QSize &size, qint32 refreshRate);
    void positionChanged(const QPoint &position);
    void transformChanged(QWaylandOutput::Transform transform);
    void scaleChanged(qreal scale);

private:
    WaylandWlrOutputConfigurationHeadV1Private *const d_ptr;

    explicit WaylandWlrOutputConfigurationHeadV1(WaylandWlrOutputHeadV1 *head, QObject *parent = nullptr);

    friend class WaylandWlrOutputConfigurationV1Private;
};

class LIRIWAYLANDSERVER_EXPORT WaylandWlrOutputConfigurationV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WaylandWlrOutputConfigurationV1)
public:
    explicit WaylandWlrOutputConfigurationV1(QObject *parent = nullptr);
    WaylandWlrOutputConfigurationV1(WaylandWlrOutputManagerV1 *manager,
                                    const QWaylandResource &resource);
    ~WaylandWlrOutputConfigurationV1();

    QVector<WaylandWlrOutputConfigurationHeadV1 *> enabledHeads() const;
    QVector<WaylandWlrOutputHeadV1 *> disabledHeads() const;

    Q_INVOKABLE void initialize(WaylandWlrOutputManagerV1 *manager,
                                const QWaylandResource &resource);

    Q_INVOKABLE void sendSucceeded();
    Q_INVOKABLE void sendFailed();
    Q_INVOKABLE void sendCancelled();

    static WaylandWlrOutputConfigurationV1 *fromResource(struct ::wl_resource *resource);

Q_SIGNALS:
    void enabledHeadsChanged();
    void disabledHeadsChanged();
    void headEnabled(WaylandWlrOutputConfigurationHeadV1 *headChanges);
    void headDisabled(WaylandWlrOutputHeadV1 *head);
    void readyToApply();
    void readyToTest();

private:
    WaylandWlrOutputConfigurationV1Private *const d_ptr;
};

#endif // LIRI_WAYLANDWLROUTPUTMANAGERV1_H
