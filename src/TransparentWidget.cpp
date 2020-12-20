/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft SettingsDialog. (https://github.com/fizzyade/SettingsDialog)
 *
 * A cross-platform settings dialog
 *
 * Created by Adrian Carpenter on 07/12/2020.
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
 */

#include "TransparentWidget.h"

#include <QGraphicsOpacityEffect>
#include <QDebug>
#include <QScrollArea>
/*
Nedrysoft::SettingsDialog::TransparentWidget::TransparentWidget(double opacity, QWidget *parent) :
        QWidget(parent) {

    m_childSize = QSize(
            childWidget->sizeHint().width(),
            qMin(childWidget->sizeHint().height(), childWidget->size().height()) );

    m_transparencyEffect = new QGraphicsOpacityEffect(this);

    m_layout = new QGridLayout;

    m_layout->setMargin(0);

    m_childWidget = childWidget;

    setLayout(m_layout);

    if (childWidget) {
        m_layout->addWidget(childWidget);
    }

    connect(m_transparencyEffect, &QGraphicsOpacityEffect::opacityChanged, [=](qreal opacity) {
        if (opacity) {
            setVisible(true);
        } else {
            setVisible(false);
        }
    });

    m_transparencyEffect->setOpacity(opacity);

    setGraphicsEffect(m_transparencyEffect);

    if (opacity==0) {
        hide();
    }

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
}
*/

Nedrysoft::SettingsDialog::TransparentWidget::TransparentWidget(double opacity, QWidget *parent) :
        QWidget(parent) {

    m_childSize = QSize(0,0);

    m_layout = new QVBoxLayout;

    m_layout->setMargin(12);
    m_layout->setSpacing(4);

    m_childWidget = nullptr;

    setLayout(m_layout);

    m_transparencyEffect = new QGraphicsOpacityEffect(this);

    connect(m_transparencyEffect, &QGraphicsOpacityEffect::opacityChanged, [=](qreal opacity) {
        if (opacity) {
            setVisible(true);
        } else {
            setVisible(false);
        }
    });

    m_transparencyEffect->setOpacity(opacity);

    setGraphicsEffect(m_transparencyEffect);

    if (opacity==0) {
        hide();
    }

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
}

void Nedrysoft::SettingsDialog::TransparentWidget::addWidget(QWidget *childWidget) {
    if (childWidget) {
        m_layout->addWidget(childWidget);

        m_childSize += QSize(
                childWidget->sizeHint().width(),
                qMin(childWidget->sizeHint().height(), childWidget->size().height()) );

        m_childSize.setHeight(m_childSize.height()+m_layout->margin());

        if (m_layout->count()) {
            m_childSize.setHeight(m_childSize.height()+(m_layout->spacing()*(m_layout->count()-1)));
        }
    }
}

int Nedrysoft::SettingsDialog::TransparentWidget::count() {
    return m_layout->count();
}

const QSize Nedrysoft::SettingsDialog::TransparentWidget::sizeHint() {
    return m_childSize;
}

QGraphicsOpacityEffect *Nedrysoft::SettingsDialog::TransparentWidget::transparencyEffect() {
    return m_transparencyEffect;
}

void Nedrysoft::SettingsDialog::TransparentWidget::setOpacity(double value) {
    m_transparencyEffect->setOpacity(value);
}
