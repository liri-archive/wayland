// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_WAYLANDCLIENT_QML_UTILS_H
#define LIRI_WAYLANDCLIENT_QML_UTILS_H

#include <QQmlListProperty>

#define LIRI_DECLARE_CONTAINER_CLASS(className) \
    class className##Container : public className \
    { \
        Q_OBJECT \
        Q_PROPERTY(QQmlListProperty<QObject> data READ data DESIGNABLE false) \
        Q_CLASSINFO("DefaultProperty", "data") \
    public: \
        QQmlListProperty<QObject> data() \
        { \
            return QQmlListProperty<QObject>(this, this, \
                                             &className##Container::appendFunction, \
                                             &className##Container::countFunction, \
                                             &className##Container::atFunction, \
                                             &className##Container::clearFunction); \
        } \
        static int countFunction(QQmlListProperty<QObject> *list) \
        { \
            return static_cast<className##Container *>(list->data)->m_objects.size(); \
        } \
        static QObject *atFunction(QQmlListProperty<QObject> *list, int index) \
        { \
            return static_cast<className##Container *>(list->data)->m_objects.at(index); \
        } \
        static void appendFunction(QQmlListProperty<QObject> *list, QObject *object) \
        { \
            static_cast<className##Container *>(list->data)->m_objects.append(object); \
        } \
        static void clearFunction(QQmlListProperty<QObject> *list) \
        { \
            static_cast<className##Container *>(list->data)->m_objects.clear(); \
        } \
    private: \
        QList<QObject *> m_objects; \
    };

#endif // LIRI_WAYLANDCLIENT_QML_UTILS_H
