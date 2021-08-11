// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "lirishortcutqml.h"

LiriShortcutQml::LiriShortcutQml()
    : LiriShortcut()
{
}

void LiriShortcutQml::componentComplete()
{
    // Find the shell from the parent, if not specified
    if (!shell()) {
        for (auto *p = parent(); p != nullptr; p = p->parent()) {
            if (auto *s = qobject_cast<LiriShell *>(p)) {
                setShell(s);
                break;
            } else if (auto *s = p->findChild<LiriShell *>()) {
                setShell(s);
                break;
            }
        }
    }

    QMetaObject::invokeMethod(this, "bindShortcut", Qt::QueuedConnection);
}
