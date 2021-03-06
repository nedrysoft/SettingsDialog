/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft SettingsDialog. (https://github.com/nedrysoft/SettingsDialog)
 *
 * A cross-platform settings dialog
 *
 * Created by Adrian Carpenter on 08/12/2020.
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

#ifndef NEDRYSOFT_ISETTINGSPAGE_H
#define NEDRYSOFT_ISETTINGSPAGE_H

#include <ComponentSystem/IInterface.h>
#include "SettingsDialogSpec.h"

namespace Nedrysoft::SettingsDialog {
    /**
     * @brief       The Settings Page class defines a settings page.
     */
    class SETTINGS_DIALOG_DLLSPEC ISettingsPage :
            public Nedrysoft::ComponentSystem::IInterface {

        private:
            Q_OBJECT

            Q_INTERFACES(Nedrysoft::ComponentSystem::IInterface)

        public:
            /**
             * @brief       The section name that this settings page should appear in, first level grouping.
             *
             * @returns     a string containing the name.
             */
            virtual auto section() -> QString = 0;

            /**
             * @brief       The category name that this settings page should appear in, second level grouping.
             *
             * @returns     a string containing the name.
             */
            virtual auto category() -> QString = 0;

            /**
             * @brief       The descriptive label for this settings page.
             *
             * @returns     a string containing the name.
             */
            virtual auto description() -> QString = 0;

            /**
             * @brief       The icon for this settings page.
             *
             * @param[in[   isDarkMode set to true to retries the dark mode icon; otherwise false.
             *
             * @returns     a QIcon
             */
            virtual auto icon(bool isDarkMode) -> QIcon = 0;

            /**
             * @brief       Creates a new instance of the page widget.
             *
             * @returns     the new widget instance.
             */
            virtual auto createWidget() -> QWidget * = 0;

            /**
             * @brief       Checks if the settings can be applied.
             *
             * @returns     true if the settings can be applied (i.e valid); otherwise false.
             */
            virtual auto canAcceptSettings() -> bool = 0;

            /**
             * @brief       Applies the current settings.
             */
            virtual auto acceptSettings() -> void = 0;
    };
}

Q_DECLARE_INTERFACE(Nedrysoft::SettingsDialog::ISettingsPage, "com.nedrysoft.settingsdialog.ISettingsPage/1.0.0")

#endif // NEDRYSOFT_ISETTINGSPAGE_H
