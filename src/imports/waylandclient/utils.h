// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_WAYLANDCLIENT_QML_UTILS_H
#define LIRI_WAYLANDCLIENT_QML_UTILS_H

#define LIRI_DECLARE_CONTAINER_CLASS(className) \
    class className##Container : public className \
    { \
        Q_OBJECT \
        Q_PROPERTY(QQmlListProperty<QObject> data READ data DESIGNABLE false) \
        Q_CLASSINFO("DefaultProperty", "data") \
    public: \
        QQmlListProperty<QObject> data() \
        { \
            return QQmlListProperty<QObject>(this, &m_objects); \
        } \
    private: \
        QList<QObject *> m_objects; \
    };

#endif // LIRI_WAYLANDCLIENT_QML_UTILS_H
