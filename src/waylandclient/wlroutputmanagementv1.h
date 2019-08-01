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

#ifndef LIRI_WLROUTPUTMANAGEMENTV1_CLIENT_H
#define LIRI_WLROUTPUTMANAGEMENTV1_CLIENT_H

#include <QPoint>
#include <QQmlListProperty>
#include <QWaylandClientExtension>
#include <QSize>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

#include <wayland-client.h>

class WlrOutputManagerV1Private;
class WlrOutputHeadV1Private;
class WlrOutputHeadV1;
class WlrOutputModeV1;
class WlrOutputModeV1Private;
class WlrOutputConfigurationV1Private;
class WlrOutputConfigurationHeadV1Private;

class LIRIWAYLANDCLIENT_EXPORT WlrOutputManagerV1 : public QWaylandClientExtensionTemplate<WlrOutputManagerV1>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrOutputManagerV1)
    Q_PROPERTY(QQmlListProperty<WlrOutputHeadV1> heads READ headsList NOTIFY headsChanged)
public:
    explicit WlrOutputManagerV1();
    ~WlrOutputManagerV1();

    void init(wl_registry *registry, int id, int version);

    QVector<WlrOutputHeadV1 *> heads() const;
    QQmlListProperty<WlrOutputHeadV1> headsList();

    Q_INVOKABLE class WlrOutputConfigurationV1 *createConfiguration();
    Q_INVOKABLE void stop();

    static const wl_interface *interface();

Q_SIGNALS:
    void headAdded(WlrOutputHeadV1 *head);
    void headsChanged();

private:
    WlrOutputManagerV1Private *const d_ptr;

    static int headsCount(QQmlListProperty<WlrOutputHeadV1> *prop);
    static WlrOutputHeadV1 *getHead(QQmlListProperty<WlrOutputHeadV1> *prop, int index);
};

class LIRIWAYLANDCLIENT_EXPORT WlrOutputHeadV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrOutputHeadV1)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QSize physicalSize READ physicalSize NOTIFY physicalSizeChanged)
    Q_PROPERTY(bool enabled READ isEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QPoint position READ position NOTIFY positionChanged)
    Q_PROPERTY(Transform transform READ transform NOTIFY transformChanged)
    Q_PROPERTY(qreal scale READ scale NOTIFY scaleChanged)
    Q_PROPERTY(QQmlListProperty<WlrOutputModeV1> modes READ modesList NOTIFY modesChanged)
public:
    enum Transform {
        TransformNormal = 0,
        Transform90,
        Transform180,
        Transform270,
        TransformFlipped,
        TransformFlipped90,
        TransformFlipped180,
        TransformFlipped270
    };
    Q_ENUM(Transform)

    explicit WlrOutputHeadV1(QObject *parent = nullptr);
    ~WlrOutputHeadV1();

    QString name() const;
    QString description() const;
    QSize physicalSize() const;
    bool isEnabled() const;
    QPoint position() const;
    Transform transform() const;
    qreal scale() const;

    QVector<WlrOutputModeV1 *> modes() const;
    QQmlListProperty<WlrOutputModeV1> modesList();
    WlrOutputModeV1 *currentMode() const;
    WlrOutputModeV1 *preferredMode() const;

Q_SIGNALS:
    void nameChanged();
    void descriptionChanged();
    void physicalSizeChanged();
    void enabledChanged();
    void positionChanged();
    void transformChanged();
    void scaleChanged();
    void modeAdded(WlrOutputModeV1 *mode);
    void modesChanged();
    void currentModeChanged(WlrOutputModeV1 *currentMode);
    void preferredModeChanged(WlrOutputModeV1 *preferredMode);

private:
    WlrOutputHeadV1Private *const d_ptr;

    friend class WlrOutputManagerV1Private;
};

class LIRIWAYLANDCLIENT_EXPORT WlrOutputModeV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrOutputModeV1)
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
    Q_PROPERTY(qint32 refresh READ refresh NOTIFY refreshChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
public:
    explicit WlrOutputModeV1(QObject *parent = nullptr);
    ~WlrOutputModeV1();

    QSize size() const;
    qint32 refresh() const;
    QString name() const;

Q_SIGNALS:
    void sizeChanged();
    void refreshChanged();
    void nameChanged();

private:
    WlrOutputModeV1Private *const d_ptr;

    friend class WlrOutputHeadV1Private;
};

class LIRIWAYLANDCLIENT_EXPORT WlrOutputConfigurationV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrOutputConfigurationV1)
    Q_PROPERTY(quint32 serial READ serial CONSTANT)
public:
    ~WlrOutputConfigurationV1();

    quint32 serial() const;

    Q_INVOKABLE class WlrOutputConfigurationHeadV1 *enableHead(WlrOutputHeadV1 *head);
    Q_INVOKABLE void disableHead(WlrOutputHeadV1 *head);

    Q_INVOKABLE void apply();
    Q_INVOKABLE void test();
    Q_INVOKABLE void destroy();

Q_SIGNALS:
    void succeeded();
    void failed();
    void cancelled();

private:
    WlrOutputConfigurationV1Private *const d_ptr;

    explicit WlrOutputConfigurationV1(quint32 serial, QObject *parent = nullptr);

    friend class WlrOutputManagerV1;
};

class LIRIWAYLANDCLIENT_EXPORT WlrOutputConfigurationHeadV1 : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WlrOutputConfigurationHeadV1)
    Q_PROPERTY(WlrOutputModeV1 *mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(WlrOutputModeV1 *customMode READ customMode WRITE setCustomMode NOTIFY customModeChanged)
    Q_PROPERTY(QPoint position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(WlrOutputHeadV1::Transform transform READ transform WRITE setTransform NOTIFY transformChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
public:
    ~WlrOutputConfigurationHeadV1();

    WlrOutputModeV1 *mode() const;
    void setMode(WlrOutputModeV1 *mode);

    WlrOutputModeV1 *customMode() const;
    void setCustomMode(WlrOutputModeV1 *mode);

    QPoint position() const;
    void setPosition(const QPoint &position);

    WlrOutputHeadV1::Transform transform() const;
    void setTransform(WlrOutputHeadV1::Transform transform);

    qreal scale() const;
    void setScale(qreal scale);

Q_SIGNALS:
    void modeChanged();
    void customModeChanged();
    void positionChanged();
    void transformChanged();
    void scaleChanged();

private:
    WlrOutputConfigurationHeadV1Private *const d_ptr;

    explicit WlrOutputConfigurationHeadV1(QObject *parent = nullptr);

    friend class WlrOutputConfigurationV1;
};

#endif // LIRI_WLROUTPUTMANAGEMENTV1_CLIENT_H
