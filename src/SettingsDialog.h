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

#ifndef NEDRYSOFT_SETTINGSDIALOG_H
#define NEDRYSOFT_SETTINGSDIALOG_H

#include <QtGlobal>

#include "SettingsDialogSpec.h"

#include <QIcon>
#include <QList>
#include <QMap>
#include <QString>
#include <QWidget>

class QHBoxLayout;
class QLabel;
class QParallelAnimationGroup;
class QPushButton;
class QStackedWidget;
class QTreeWidget;
class QVBoxLayout;

namespace Nedrysoft { namespace ThemeSupport {
    class ThemeSupport;
}}

namespace Nedrysoft { namespace SettingsDialog {
    class TransparentWidget;
    class ISettingsPage;
    class MacToolbar;
    class MacToolbarItem;

    /**
     * @brief       The SettingsPage class describes an individual page of the application settings
     */
    class SettingsPage {
        public:
            SettingsPage() :
#if defined(Q_OS_MACOS)
                //m_toolBarItem(nullptr),
                m_pageSettings(QList<ISettingsPage *>()),
#else
                m_pageSettings(nullptr),
#endif
                m_widget(nullptr) {

                }

        public:
            QString m_name;                             //! display name of the settings category page
            QString m_description;                      //! description of the settings category page
#if defined(Q_OS_MACOS)
            TransparentWidget *m_widget;                //! the widget that contains the settings for this category
            Nedrysoft::SettingsDialog::MacToolbarItem *m_toolbarItem;              //! toolbar item
            QList<ISettingsPage *> m_pageSettings;      //! pointer to the page interfaces
#else
            QWidget *m_widget;                          //! the widget that contains the settings for this category
            ISettingsPage *m_pageSettings;              //! pointer to the page interface
#endif
            QIcon m_icon;                               //! the icon of the page
    };

    /**
    * @brief        The SettingsDialog class provides a common themed settings dialog for Windows and Linux
    *               and a correctly styled dialog for macOS.
    *
    * @details      Describes an individual page of the application settings
    */
    class SETTINGS_DIALOG_DLLSPEC SettingsDialog :
            public QWidget {

        private:
            Q_OBJECT

            //Q_DISABLE_COPY(SettingsDialog)
            //Q_DISABLE_MOVE(SettingsDialog)

        public:
            /**
             * @brief       Constructs a new SettingsDialog instance which is a child of the parent.
             *
             * @param[in]   pages the pages to be displayed.
             * @param[in]   parent is the the owner of the child.
             */
            explicit SettingsDialog(const QList<ISettingsPage *> &pages, QWidget *parent=nullptr);

            /**
             * @brief       Destroys the SettingsDialog.
             */
            ~SettingsDialog() override;

            /**
             * @brief       This signal is emitted when the window is closed by the user.
             */
            Q_SIGNAL void closed();

        protected:
            /**
             * @brief       Reimplements: QWidget::closeEvent(QCloseEvent *event).
             *
             * @param[in]   event the event information.
             */
            auto closeEvent(QCloseEvent *event) -> void override;

            /**
             * @brief       Returns the recommended size for the widget.
             *
             * @returns     the size of the widget.
             */
            auto sizeHint() -> QSize;

        private:
            /**
             * @brief       Returns the QWindow handle from native widget.
             *
             * @returns     The QWindow handle of the native window.
             */
            auto nativeWindowHandle() -> QWindow *;

            /**
             * @brief       Checks if the settings dialog can be closed.
             *
             * @returns     true if closable; otherwise false.
             */
            auto okToClose() -> bool;

            /**
             * @brief       Accept all settings that have been modified.
             *
             * @returns     true if settings were accepted; otherwise false.
             */
            auto acceptSettings() -> bool;

        protected:
            /**
             * @brief       Reimplements: QWidget::resizeEvent(QResizeEvent *event).
             *
             * @param[in]   event the event information.
             */
            auto resizeEvent(QResizeEvent *event) -> void override;

            /**
             * @brief       Adds a setting page to the settings dialog.
             *
             * @params[in]  page is a ISettingsPage instance.
             *
             * @returns     the settings page structure
             */
            auto addPage(ISettingsPage *page) -> SettingsPage *;

        private:
#if defined(Q_OS_MACOS)
            Nedrysoft::SettingsDialog::MacToolbar *m_toolbar;   //! A native macOS toolbar (unified style)
            QMap<Nedrysoft::SettingsDialog::MacToolbarItem *, SettingsPage *> m_pages;     //! The list of settings widgets
            int m_toolbarHeight;                                //! the height of the unified toolbar
            int m_maximumWidth;                                 //! the maximal width of all the settings page.
            QParallelAnimationGroup *m_animationGroup;          //! the currently active animation
#else
            QVBoxLayout *m_layout;                              //! details layout + main layout + control layout
            QVBoxLayout *m_detailLayout;                        //! detail layout
            QHBoxLayout *m_mainLayout;                          //! detail layout + tree widget
            QHBoxLayout *m_controlsLayout;                      //! apply/ok/cancel layout
            QTreeWidget *m_treeWidget;                          //! tree widget for categories
            QStackedWidget *m_stackedWidget;                    //! stacked widget for page content
            QLabel *m_categoryLabel;                            //! category label
            QPushButton *m_okButton;                            //! ok button, saves and dismisses the dialog (if changes valid)
            QPushButton *m_cancelButton;                        //! cancel button, forgets any changes made since last apply
            QPushButton *m_applyButton;                         //! apply button, saves changes but keeps dialog open
            QList<SettingsPage *> m_pages;
#endif
            SettingsPage *m_currentPage;                        //! current widget
            Nedrysoft::ThemeSupport::ThemeSupport *m_themeSupport;     //! theme support
    };
}}

#endif // NEDRYSOFT_SETTINGSDIALOG_H
