/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft SettingsDialog. (https://github.com/fizzyade/SettingsDialog)
 *
 * A cross-platform settings dialog
 *
 * Created by Adrian Carpenter on 20/12/2020.
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

#include "SeparatorWidget.h"

#include <QPainter>

constexpr auto lineMargin = 25;
constexpr auto lineColour = qRgb(0x60, 0x60, 0x60);

void Nedrysoft::SettingsDialog::SeparatorWidget::paintEvent(QPaintEvent *event) {
    QPoint centrePoint = rect().center();

    QPainter painter(this);

    painter.setPen(QColor(lineColour));

    painter.drawLine(rect().left()+lineMargin, centrePoint.y(), rect().right()-lineMargin, centrePoint.y());
}