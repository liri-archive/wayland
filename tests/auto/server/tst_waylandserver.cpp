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

#include <QtTest>
#include <QMargins>
#include <QQuickWindow>

#include <LiriWaylandServer/WaylandLiriColorPicker>
#include <LiriWaylandServer/WaylandWlrExportDmabufV1>
#include <LiriWaylandServer/WaylandWlrForeignToplevelManagementV1>
#include <LiriWaylandServer/WaylandWlrLayerShellV1>
#include <LiriWaylandServer/WaylandWlrOutputManagerV1>
#include <LiriWaylandServer/WaylandWlrScreencopyV1>

#include "mockclient.h"
#include "testcompositor.h"

class TestWaylandServer : public QObject
{
    Q_OBJECT
public:
    TestWaylandServer(QObject *parent = nullptr);

private slots:
    void liriColorPicker();
    void wlrExportDmabuf();
    void wlrForeignToplevel();
    void wlrLayer();
    void wlrOutputManager();
    void wlrScreencopy();
};

TestWaylandServer::TestWaylandServer(QObject *parent)
    : QObject(parent)
{
}

class LiriColorPickerCompositor : public TestCompositor
{
    Q_OBJECT
public:
    LiriColorPickerCompositor() : manager(this) {}

    WaylandLiriColorPickerManager manager;
};

void TestWaylandServer::liriColorPicker()
{
    auto red = QColor(Qt::red);
    auto redValue = static_cast<uint32_t>(red.rgba());

    QQuickWindow *window = new QQuickWindow();
    window->setColor(red);
    window->resize(QSize(1024, 768));
    window->create();

    LiriColorPickerCompositor compositor;
    compositor.prepare();

    QWaylandOutputMode serverMode(QSize(1024, 768), 60000);
    compositor.defaultOutput()->addMode(serverMode, true);
    compositor.defaultOutput()->setCurrentMode(serverMode);
    compositor.defaultOutput()->setWindow(window);

    compositor.create();

    MockClient client;
    QTRY_VERIFY(client.registry->liriColorPickerManager);

    MockLiriColorPicker *picker = nullptr;

    auto *output = client.registry->outputs.first();
    picker = client.registry->liriColorPickerManager->pickAtLocation(output, QPoint(50, 50));
    QVERIFY(picker);
    QTRY_COMPARE(picker->lastPickedValue(), redValue);
}

class WlrExportDmabufCompositor : public TestCompositor
{
    Q_OBJECT
public:
    WlrExportDmabufCompositor()
        : TestCompositor()
        , manager(this)
    {
        connect(&manager, &WaylandWlrExportDmabufManagerV1::outputCaptureRequested, this,
                [this](WaylandWlrExportDmabufFrameV1 *f) {
            frame = f;
        });
    }

    WaylandWlrExportDmabufManagerV1 manager;
    WaylandWlrExportDmabufFrameV1 *frame = nullptr;
};

void TestWaylandServer::wlrExportDmabuf()
{
    WlrExportDmabufCompositor compositor;
    compositor.prepare();
    compositor.create();

    QWaylandOutputMode serverMode(QSize(1024, 768), 60000);
    compositor.defaultOutput()->addMode(serverMode, true);
    compositor.defaultOutput()->setCurrentMode(serverMode);

    QSignalSpy capturedSpy(&compositor.manager, SIGNAL(outputCaptureRequested(WaylandWlrExportDmabufFrameV1*)));

    MockClient client;
    QTRY_VERIFY(client.registry->wlrExportDmabuf);

    auto *output = client.registry->outputs.first()->object();
    auto *frameObject = client.registry->wlrExportDmabuf->capture_output(1, output);
    QTRY_VERIFY(frameObject);

    auto *frame = new MockWlrExportDmabufFrameV1(frameObject);
    QTRY_VERIFY(frame->isInitialized());

    QTRY_VERIFY(compositor.frame);
    QCOMPARE(capturedSpy.count(), 1);

    compositor.flushClients();

    compositor.frame->frame(
                QSize(100, 100), QPoint(50, 50),
                WaylandWlrExportDmabufFrameV1::YInvert,
                WaylandWlrExportDmabufFrameV1::Transient,
                42, 69, 1);
    compositor.flushClients();
    QTRY_VERIFY(frame->frame.size.isValid());
    QCOMPARE(frame->frame.size, QSize(100, 100));
    QCOMPARE(frame->frame.offset, QPoint(50, 50));
    QCOMPARE(frame->frame.bufferFlags, WaylandWlrExportDmabufFrameV1::YInvert);
    QCOMPARE(frame->frame.flags, MockWlrExportDmabufFrameV1::flags_transient);
    QCOMPARE(frame->frame.format, 42);
    QCOMPARE(frame->frame.modifier, 69);
    QCOMPARE(frame->frame.numObjects, 1);
    QCOMPARE(frame->frame.objects.capacity(), 1);

    compositor.frame->object(0, 0, 1024, 0, 100 * 100 * 24, 0);
    compositor.flushClients();
    QTRY_COMPARE(frame->frame.objects.size(), 1);
    QVERIFY(frame->frame.objects.first()->fd > 0);
    QCOMPARE(frame->frame.objects.first()->size, 1024);
    QCOMPARE(frame->frame.objects.first()->offset, 0);
    QCOMPARE(frame->frame.objects.first()->stride, 100 * 100 * 24);
    QCOMPARE(frame->frame.objects.first()->planeIndex, 0);

    compositor.frame->ready(13, 37);
    compositor.flushClients();
    QTRY_COMPARE(frame->time.tv_sec, 13);
    QCOMPARE(frame->time.tv_nsec, 37);
    QCOMPARE(frame->ready, true);

    compositor.frame->cancel(WaylandWlrExportDmabufFrameV1::Resizing);
    compositor.flushClients();
    QTRY_COMPARE(frame->cancellation.canceled, true);
    QCOMPARE(frame->cancellation.reason, MockWlrExportDmabufFrameV1::cancel_reason_resizing);
}

class WlrForeignToplevelCompositor : public TestCompositor
{
    Q_OBJECT
public:
    WlrForeignToplevelCompositor() : manager(this) {}
    WaylandWlrForeignToplevelManagerV1 manager;
};

void TestWaylandServer::wlrForeignToplevel()
{
    WlrForeignToplevelCompositor compositor;
    compositor.prepare();
    compositor.create();

    MockClient client;
    QTRY_VERIFY(client.registry->wlrForeignToplevel);

    QSignalSpy addedSpy(&compositor.manager, SIGNAL(handleAdded(WaylandWlrForeignToplevelHandleV1*)));

    auto *handle = new WaylandWlrForeignToplevelHandleV1();
    handle->setCompositor(&compositor);
    handle->setManager(&compositor.manager);
    handle->setTitle(QStringLiteral("Window title"));
    handle->setAppId(QStringLiteral("io.liri.App"));
    handle->setMinimized(true);
    handle->setMaximized(true);
    handle->setFullscreen(true);
    handle->setActivated(true);
    handle->initialize();

    compositor.flushClients();

    QCOMPARE(addedSpy.count(), 1);

    QTRY_COMPARE(client.registry->wlrForeignToplevel->handles.size(), 1);

    auto *clientHandle = client.registry->wlrForeignToplevel->handles.last();

    QTRY_COMPARE(clientHandle->effective.title, handle->title());
    QTRY_COMPARE(clientHandle->effective.appId, handle->appId());
    QTRY_COMPARE(clientHandle->effective.minimized, handle->isMinimized());
    QTRY_COMPARE(clientHandle->effective.maximized, handle->isMaximized());
    QTRY_COMPARE(clientHandle->effective.fullscreen, handle->isFullscreen());
    QTRY_COMPARE(clientHandle->effective.activated, handle->isActivated());

    handle->setTitle(QStringLiteral("New window title"));
    compositor.flushClients();
    QTRY_COMPARE(clientHandle->effective.title, handle->title());

    handle->setAppId(QStringLiteral("io.liri.App2"));
    compositor.flushClients();
    QTRY_COMPARE(clientHandle->effective.appId, handle->appId());

    handle->setMinimized(false);
    compositor.flushClients();
    QTRY_COMPARE(clientHandle->effective.minimized, handle->isMinimized());

    handle->setMaximized(false);
    compositor.flushClients();
    QTRY_COMPARE(clientHandle->effective.maximized, handle->isMaximized());

    handle->setFullscreen(false);
    compositor.flushClients();
    QTRY_COMPARE(clientHandle->effective.fullscreen, handle->isFullscreen());

    handle->setActivated(false);
    compositor.flushClients();
    QTRY_COMPARE(clientHandle->effective.activated, handle->isActivated());
}

class WlrLayerCompositor : public TestCompositor
{
    Q_OBJECT
public:
    WlrLayerCompositor()
        : TestCompositor()
        , layerShell(this)
    {
        connect(&layerShell, &WaylandWlrLayerShellV1::layerSurfaceCreated, this, [this](WaylandWlrLayerSurfaceV1 *surface) {
            layerSurface = surface;
        });
    }

    WaylandWlrLayerShellV1 layerShell;
    WaylandWlrLayerSurfaceV1 *layerSurface = nullptr;
};

void TestWaylandServer::wlrLayer()
{
    WlrLayerCompositor compositor;
    compositor.prepare();
    compositor.create();

    MockClient client;
    QTRY_VERIFY(client.registry->wlrLayerShell);

    auto *surface = client.registry->createSurface();
    QTRY_COMPARE(compositor.surfaces.size(), 1);

    auto *wlrSurface = client.registry->wlrLayerShell->createSurface(surface->object(), nullptr,
                                                                     MockWlrLayerShellV1::layer_bottom,
                                                                     QStringLiteral("bg"));
    QTRY_VERIFY(compositor.layerSurface);

    QSignalSpy changedSpy(compositor.layerSurface, SIGNAL(changed()));
    QSignalSpy layerChangedSpy(compositor.layerSurface, SIGNAL(layerChanged()));

    compositor.layerSurface->sendConfigure(100, 100);
    surface->commit();
    QTRY_COMPARE(wlrSurface->size(), QSize(100, 100));

    wlrSurface->set_anchor(MockWlrLayerSurfaceV1::anchor_left | MockWlrLayerSurfaceV1::anchor_right);
    wlrSurface->set_exclusive_zone(42);
    wlrSurface->set_keyboard_interactivity(1);
    wlrSurface->set_margin(5, 5, 5, 5);
    wlrSurface->set_size(50, 50);
    surface->commit();
    QTRY_COMPARE(compositor.layerSurface->layer(), WaylandWlrLayerShellV1::BottomLayer);
    QTRY_COMPARE(compositor.layerSurface->anchors(), WaylandWlrLayerSurfaceV1::LeftAnchor | WaylandWlrLayerSurfaceV1::RightAnchor);
    QTRY_COMPARE(compositor.layerSurface->exclusiveZone(), 42);
    QTRY_COMPARE(compositor.layerSurface->keyboardInteractivity(), true);
    QTRY_COMPARE(compositor.layerSurface->margins(), QMargins(5, 5, 5, 5));
    QTRY_COMPARE(compositor.layerSurface->size(), QSize(50, 50));

    wlrSurface->set_layer(MockWlrLayerShellV1::layer_overlay);
    surface->commit();
    QTRY_COMPARE(changedSpy.count(), 3);
    QTRY_COMPARE(layerChangedSpy.count(), 1);
    QCOMPARE(compositor.layerSurface->layer(), WaylandWlrLayerShellV1::OverlayLayer);

    compositor.layerSurface->close();
    surface->destroy();
}

class WlrOutputManagerCompositor : public TestCompositor
{
    Q_OBJECT
public:
    WlrOutputManagerCompositor()
        : TestCompositor()
        , outputManager(this)
    {
        connect(&outputManager, &WaylandWlrOutputManagerV1::configurationCreated, this, [this](WaylandWlrOutputConfigurationV1 *config) {
            configuration = config;

            connect(configuration, &WaylandWlrOutputConfigurationV1::readyToTest, this, [this] {
                configuration->sendSucceeded();
            });
            connect(configuration, &WaylandWlrOutputConfigurationV1::readyToApply, this, [this] {
                configuration->sendSucceeded();
            });
        });
    }

    WaylandWlrOutputManagerV1 outputManager;
    WaylandWlrOutputConfigurationV1 *configuration = nullptr;
};

void TestWaylandServer::wlrOutputManager()
{
    WlrOutputManagerCompositor compositor;
    compositor.prepare();
    compositor.create();

    QWaylandOutputMode serverMode(QSize(1024, 768), 60000);
    compositor.defaultOutput()->addMode(serverMode, true);
    compositor.defaultOutput()->setCurrentMode(serverMode);

    QSignalSpy headAddedSpy(&compositor.outputManager, SIGNAL(headAdded(WaylandWlrOutputHeadV1*)));

    auto *head1 = new WaylandWlrOutputHeadV1;
    head1->setManager(&compositor.outputManager);
    head1->setName(QStringLiteral("HEAD1"));
    head1->setDescription(QStringLiteral("Test output"));
    head1->setPhysicalSize(QSize(600, 900));
    head1->setPosition(QPoint(50, 50));
    head1->setTransform(QWaylandOutput::Transform90);
    head1->setScale(1.5);
    auto *mode1 = new WaylandWlrOutputModeV1;
    mode1->setSize(QSize(1024, 768));
    mode1->setRefresh(60000);
    head1->addMode(mode1);
    head1->setCurrentMode(mode1);
    head1->setPreferredMode(mode1);
    QCOMPARE(headAddedSpy.count(), 1);

    auto *head2 = new WaylandWlrOutputHeadV1;
    head2->setManager(&compositor.outputManager);
    head2->setName(QStringLiteral("HEAD2"));
    head2->setDescription(QStringLiteral("Second test output"));
    head2->setPhysicalSize(QSize(300, 450));
    head2->setPosition(QPoint(50, 50));
    head2->setTransform(QWaylandOutput::Transform90);
    head2->setScale(2);
    auto *mode2 = new WaylandWlrOutputModeV1;
    mode2->setSize(QSize(800, 600));
    mode2->setRefresh(75000);
    head2->addMode(mode2);
    head2->setCurrentMode(mode2);
    head2->setPreferredMode(mode2);
    QCOMPARE(headAddedSpy.count(), 2);

    MockClient client;
    QTRY_VERIFY(client.registry->wlrOutputManager);
    QTRY_COMPARE(client.registry->outputs.size(), 1);
    QTRY_COMPARE(client.registry->wlrOutputManager->actual.heads.size(), 2);

    auto *clientHead1 = client.registry->wlrOutputManager->actual.heads.at(0);
    QTRY_VERIFY(clientHead1->enabled == true);
    QTRY_COMPARE(clientHead1->name, QLatin1String("HEAD1"));
    QTRY_COMPARE(clientHead1->description, QLatin1String("Test output"));
    QTRY_COMPARE(clientHead1->physicalSize, QSize(600, 900));
    QTRY_COMPARE(clientHead1->position, QPoint(50, 50));
    QTRY_COMPARE(clientHead1->transform, MockWlrOutputHeadV1::Transform90);
    QTRY_COMPARE(clientHead1->scale, 1.5);
    QTRY_VERIFY(clientHead1->currentMode);
    QTRY_COMPARE(clientHead1->currentMode->size, QSize(1024, 768));
    QTRY_COMPARE(clientHead1->currentMode->refresh, 60000);
    QTRY_VERIFY(clientHead1->currentMode->preferred == true);

    auto *clientHead2 = client.registry->wlrOutputManager->actual.heads.at(1);
    QTRY_VERIFY(clientHead2->enabled == true);
    QTRY_COMPARE(clientHead2->name, QLatin1String("HEAD2"));
    QTRY_COMPARE(clientHead2->description, QLatin1String("Second test output"));
    QTRY_COMPARE(clientHead2->physicalSize, QSize(300, 450));
    QTRY_COMPARE(clientHead2->position, QPoint(50, 50));
    QTRY_COMPARE(clientHead2->transform, MockWlrOutputHeadV1::Transform90);
    QTRY_COMPARE(clientHead2->scale, 2);
    QTRY_VERIFY(clientHead2->currentMode);
    QTRY_COMPARE(clientHead2->currentMode->size, QSize(800, 600));
    QTRY_COMPARE(clientHead2->currentMode->refresh, 75000);
    QTRY_VERIFY(clientHead2->currentMode->preferred == true);

    auto *configClient = client.registry->wlrOutputManager->createConfiguration();
    QTRY_VERIFY(configClient);
    QTRY_VERIFY(compositor.configuration);
    auto *configHeadClient = configClient->enableHead(clientHead1);
    QTRY_VERIFY(configHeadClient);
    configClient->disableHead(clientHead2);

    configHeadClient->setCustomMode(QSize(111, 222), 89000);
    configHeadClient->setPosition(QPoint(5, 5));
    configHeadClient->setTransform(MockWlrOutputHeadV1::Transform180);
    configHeadClient->setScale(4);

    configClient->test();
    compositor.configuration->sendSucceeded();
    compositor.flushClients();
    // FIXME: investigate
    //QTRY_VERIFY(configClient->succeeded);
    configClient->succeeded = false;

    configClient->apply();
    compositor.configuration->sendSucceeded();
    compositor.flushClients();
    // FIXME: investigate
    //QTRY_VERIFY(configClient->succeeded);
    configClient->succeeded = false;

    QTRY_COMPARE(compositor.configuration->enabledHeads().size(), 1);
    QTRY_COMPARE(compositor.configuration->enabledHeads().at(0)->customModeSize(), QSize(111, 222));
    QTRY_COMPARE(compositor.configuration->enabledHeads().at(0)->customModeRefresh(), 89000);
    QTRY_COMPARE(compositor.configuration->enabledHeads().at(0)->position(), QPoint(5, 5));
    QTRY_COMPARE(compositor.configuration->enabledHeads().at(0)->transform(), QWaylandOutput::Transform180);
    QTRY_COMPARE(compositor.configuration->enabledHeads().at(0)->scale(), 4);

    QTRY_COMPARE(compositor.configuration->disabledHeads().size(), 1);
    QTRY_COMPARE(compositor.configuration->disabledHeads().at(0)->name(), QStringLiteral("HEAD2"));
}

class WlrScreencopyCompositor : public TestCompositor
{
public:
    WlrScreencopyCompositor()
        : TestCompositor()
        , manager(this)
    {
        connect(&manager, &WaylandWlrScreencopyManagerV1::captureOutputRequested, this,
                [this](WaylandWlrScreencopyFrameV1 *f) {
            frame = f;
        });
    }

    WaylandWlrScreencopyManagerV1 manager;
    WaylandWlrScreencopyFrameV1 *frame = nullptr;
};

void TestWaylandServer::wlrScreencopy()
{
    WlrScreencopyCompositor compositor;
    compositor.prepare();
    compositor.create();

    QWaylandOutputMode serverMode(QSize(1024, 768), 60000);
    compositor.defaultOutput()->addMode(serverMode, true);
    compositor.defaultOutput()->setCurrentMode(serverMode);

    MockClient client;
    QTRY_VERIFY(client.registry->wlrScreencopyManager);
    auto *outputClient = client.registry->outputs.first();

    client.registry->wlrScreencopyManager->shm = client.registry->shm;

    QSignalSpy frameSpy(&compositor.manager, SIGNAL(captureOutputRequested(WaylandWlrScreencopyFrameV1*)));

    auto *frameClient = client.registry->wlrScreencopyManager->captureOutput(true, outputClient);

    QTRY_COMPARE(frameSpy.count(), 1);

    QVERIFY(compositor.frame);

    compositor.frame->setFlags(WaylandWlrScreencopyFrameV1::YInvert);
    compositor.frame->copy();

    // FIXME: Oddly enough, the server sends flags and ready but our mock client won't receive it
    //QVERIFY(frameClient->yInverted);
    //QVERIFY(frameClient->ready);
    //QVERIFY(!frameClient->failed);
}

QTEST_MAIN(TestWaylandServer)

#include "tst_waylandserver.moc"
