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

#ifndef NEDRYSOFT_TRANSPARENTWIDGET_H
#define NEDRYSOFT_TRANSPARENTWIDGET_H

#include <QVBoxLayout>
#include <QWidget>

class QGraphicsOpacityEffect;

namespace Nedrysoft::SettingsDialog {
    /**
     * @brief               The TransparentWidget class.
     *
     * @details             A QWidget which supports transparency.
     */
    class TransparentWidget :
            public QWidget {

        private:
            Q_OBJECT

        public:
            /**
             * @brief       Constructs a new TransparentWidget instance.
             *
             * @param[in]   opacity the initial transparency level, 0 is completely transparent, 1 is completely opaque.
             * @param[in]   parent the the parent of the widget.
             */
            TransparentWidget(double opacity=1, QWidget *parent=nullptr);

            /**
             * @brief       Returns the transparency graphics effect.
             *
             * @returns     the transparency effect so that it can be used by property animation.
             */
            auto transparencyEffect() -> QGraphicsOpacityEffect *;

            /**
             * @brief       Sets the opacity value.
             *
             * @param[in]   value is the opacity level, 0 is completely transparent, 1 is completely opaque.
             */
            auto setOpacity(double value) -> void;

            /**
             * @brief       Returns the size hint for the widget.
             *
             * @note        This returns the child widgets size.
             *
             * @returns     the size.
             */
            auto sizeHint() -> const QSize;

            /**
             * @brief       Adds a widget to the layout.
             *
             * @param[in]   childWidget the widget to add to the layout.
             */
            auto addWidget(QWidget *childWidget) -> void;

            /**
             * @brief       Returns the number of widgets in the layout.
             *
             * @returns     the number of widgets.
             */
            auto count() -> int;

        private:
            QGraphicsOpacityEffect *m_transparencyEffect;               //! the transparency graphics effect set to this widget
            QVBoxLayout *m_layout;                                      //! layout used on the transparent window
            QWidget *m_childWidget;                                     //! the child widget being made transparent
            QSize m_childSize;
    };
}

#endif // NEDRYSOFT_TRANSPARENTWIDGET_H
