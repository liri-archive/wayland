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

#ifndef LIRI_XDGSHELL_CLIENT_H
#define LIRI_XDGSHELL_CLIENT_H

#include <QObject>
#include <QWaylandClientExtension>

#include <LiriWaylandClient/liriwaylandclientglobal.h>

QT_FORWARD_DECLARE_CLASS(QWindow)

class XdgWmBasePrivate;
class XdgPositioner;
class XdgPositionerPrivate;
class XdgSurface;
class XdgSurfacePrivate;
class XdgToplevelPrivate;
class XdgPopupPrivate;

class LIRIWAYLANDCLIENT_EXPORT XdgWmBase
        : public QWaylandClientExtensionTemplate<XdgWmBase>
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(XdgWmBase)
public:
    XdgWmBase();
    ~XdgWmBase();

    void init(struct ::wl_registry *registry, int id, int version);

    static const wl_interface *interface();

private:
    XdgWmBasePrivate *const d_ptr;
};

class LIRIWAYLANDCLIENT_EXPORT XdgPositioner : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(XdgPositioner)
    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)
    Q_PROPERTY(XdgWmBase *xdgWmBase READ xdgWmBase WRITE setXdgWmBase NOTIFY xdgWmBaseChanged)
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QRect anchorRect READ anchorRect WRITE setAnchorRect NOTIFY anchorRectChanged)
    Q_PROPERTY(Anchor anchor READ anchor WRITE setAnchor NOTIFY anchorChanged)
    Q_PROPERTY(Gravity gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(ConstraintAdjustments constraintAdjustments READ constraintAdjustments WRITE setConstraintAdjustments NOTIFY constraintAdjustmentsChanged)
public:
    enum Anchor {
        NoAnchor = 0,
        TopAnchor,
        BottomAnchor,
        LeftAnchor,
        RightAnchor,
        TopLeftAnchor,
        BottomLeftAnchor,
        TopRightAnchor,
        BottomRightAnchor
    };
    Q_ENUM(Anchor)

    enum Gravity {
        NoGravity = 0,
        TopGravity,
        BottomGravity,
        LeftGravity,
        RightGravity,
        TopLeftGravity,
        BottomLeftGravity,
        TopRightGravity,
        BottomRightGravity
    };
    Q_ENUM(Gravity)

    enum ConstraintAdjustment {
        NoConstraintAdjustment = 0,
        ConstraintAdjustmentSlideX = 1,
        ConstraintAdjustmentSlideY = 2,
        ConstraintAdjustmentFlipX = 4,
        ConstraintAdjustmentFlipY = 8,
        ConstraintAdjustmentResizeX = 16,
        ConstraintAdjustmentResizeY = 32
    };
    Q_ENUM(ConstraintAdjustment)
    Q_DECLARE_FLAGS(ConstraintAdjustments, ConstraintAdjustment)

    explicit XdgPositioner(QObject *parent = nullptr);
    ~XdgPositioner();

    bool isInitialized() const;
    bool isValid() const;

    XdgWmBase *xdgWmBase() const;
    void setXdgWmBase(XdgWmBase *xdgWmBase);

    QSize size() const;
    void setSize(const QSize &size);

    QRect anchorRect() const;
    void setAnchorRect(const QRect &rect);

    Anchor anchor() const;
    void setAnchor(Anchor anchor);

    Gravity gravity() const;
    void setGravity(Gravity gravity);

    ConstraintAdjustments constraintAdjustments() const;
    void setConstraintAdjustments(ConstraintAdjustments constraintAdjustment);

    QPoint offset() const;
    void setOffset(const QPoint &offset);

public Q_SLOTS:
    void initialize();

Q_SIGNALS:
    void validChanged();
    void xdgWmBaseChanged();
    void sizeChanged();
    void anchorRectChanged();
    void anchorChanged();
    void gravityChanged();
    void constraintAdjustmentsChanged();
    void offsetChanged();

private:
    XdgPositionerPrivate *const d_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(XdgPositioner::ConstraintAdjustments)

class LIRIWAYLANDCLIENT_EXPORT XdgSurface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(XdgWmBase *xdgWmBase READ xdgWmBase WRITE setXdgWmBase NOTIFY xdgWmBaseChanged)
    Q_PROPERTY(QWindow *window READ window WRITE setWindow NOTIFY windowChanged)
    Q_DECLARE_PRIVATE(XdgSurface)
public:
    explicit XdgSurface(QObject *parent = nullptr);
    ~XdgSurface();

    bool isInitialized() const;

    XdgWmBase *xdgWmBase() const;
    void setXdgWmBase(XdgWmBase *xdgWmBase);

    QWindow *window() const;
    void setWindow(QWindow *window);

public Q_SLOTS:
    void initialize();

Q_SIGNALS:
    void xdgWmBaseChanged();
    void windowChanged();

private:
    XdgSurfacePrivate *const d_ptr;
};

class LIRIWAYLANDCLIENT_EXPORT XdgPopup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(XdgSurface *xdgSurfaceParent READ xdgSurfaceParent WRITE setXdgSurfaceParent NOTIFY xdgSurfaceParentChanged)
    Q_PROPERTY(XdgSurface *xdgSurface READ xdgSurface WRITE setXdgSurface NOTIFY xdgSurfaceChanged)
    Q_PROPERTY(XdgPositioner *xdgPositioner READ xdgPositioner WRITE setXdgPositioner NOTIFY xdgPositionerChanged)
    Q_DECLARE_PRIVATE(XdgPopup)
public:
    explicit XdgPopup(QObject *parent = nullptr);
    ~XdgPopup();

    bool isInitialized() const;

    XdgSurface *xdgSurfaceParent() const;
    void setXdgSurfaceParent(XdgSurface *xdgSurfaceParent);

    XdgSurface *xdgSurface() const;
    void setXdgSurface(XdgSurface *xdgSurface);

    XdgPositioner *xdgPositioner() const;
    void setXdgPositioner(XdgPositioner *xdgPositioner);

    Q_INVOKABLE void grab();

public Q_SLOTS:
    void initialize();

Q_SIGNALS:
    void xdgSurfaceParentChanged();
    void xdgSurfaceChanged();
    void xdgPositionerChanged();
    void configured(const QRect &rect);
    void done();

private:
    XdgPopupPrivate *const d_ptr;
};

#endif // LIRI_XDGSHELL_CLIENT_H
