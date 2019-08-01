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

#include <LiriWaylandServer/WaylandWlrOutputManagerV1>

#include "mockclient.h"
#include "testcompositor.h"

class TestWaylandServer : public QObject
{
    Q_OBJECT
public:
    TestWaylandServer(QObject *parent = nullptr);

private slots:
    void wlrOutputManager();
};

TestWaylandServer::TestWaylandServer(QObject *parent)
    : QObject(parent)
{
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
    QTRY_COMPARE(configClient->succeeded, true);
    configClient->succeeded = false;
    configClient->apply();
    QTRY_COMPARE(configClient->succeeded, true);
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

QTEST_MAIN(TestWaylandServer)

#include "tst_waylandserver.moc"
