/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft SettingsDialog. (https://github.com/nedrysoft/SettingsDialog)
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

#include <QDebug>
#include "SettingsDialog/SettingsDialog.h"

#include "SettingsDialog/ISettingsPage.h"
#include "SeparatorWidget.h"

#if defined(Q_OS_MACOS)
#include "TransparentWidget.h"
#endif

#if defined(Q_OS_MACOS)
#include <QGraphicsOpacityEffect>
#include <QMacToolBar>
#include <QPropertyAnimation>
#include <QtMac>
#else
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#endif

#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QTreeWidget>
#include <QVBoxLayout>

#if defined(Q_OS_MACOS)
#include "MacHelper.h"

#include <memory>

using namespace std::chrono_literals;

constexpr auto transisionDuration = 100ms;
constexpr auto toolbarItemWidth = 64;
constexpr auto alphaTransparent = 0;
constexpr auto alphaOpaque = 1;
constexpr auto defaultMinimumWidth = 300;
#else
constexpr auto categoryFontAdjustment = 6;
constexpr auto settingsIconSize = 32;
constexpr auto settingsDialogScaleFactor = 0.5;
constexpr auto categoryLeftMargin = 4;
constexpr auto categoryBottomMargin = 9;
constexpr auto detailsLeftMargin = 9;
#endif

Nedrysoft::SettingsDialog::SettingsDialog::SettingsDialog(const QList<Nedrysoft::SettingsDialog::ISettingsPage *> &pages, QWidget *parent) :
        QWidget(nullptr),
        m_currentPage(nullptr) {

    Q_UNUSED(parent)

#if defined(Q_OS_MACOS)
    m_toolBar = new QMacToolBar(this);

    m_animationGroup = nullptr;
#else
    resize((QSizeF(parent->frameSize())*settingsDialogScaleFactor).toSize());

    m_mainLayout = new QHBoxLayout;

    m_treeWidget = new QTreeWidget(this);

    m_treeWidget->setIndentation(0);

    m_treeWidget->setIconSize(QSize(settingsIconSize, settingsIconSize));

    m_treeWidget->setHeaderHidden(true);

    m_treeWidget->setSelectionBehavior(QTreeWidget::SelectRows);

    m_treeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_stackedWidget = new QStackedWidget;

    m_stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_stackedWidget->layout()->setMargin(0);

    m_mainLayout->addWidget(m_treeWidget);

    m_categoryLabel = new QLabel;

    m_categoryLabel->setContentsMargins(categoryLeftMargin,0,0,categoryBottomMargin);

    m_categoryLabel->setFont(QFont(m_categoryLabel->font().family(), m_categoryLabel->font().pointSize()+categoryFontAdjustment));
    m_categoryLabel->setAlignment(Qt::AlignLeft);

    m_detailLayout = new QVBoxLayout;

    m_detailLayout->setContentsMargins(detailsLeftMargin,0,0,0);

    m_detailLayout->addWidget(m_categoryLabel);

    m_detailLayout->addWidget(m_stackedWidget);

    m_detailLayout->setAlignment(m_categoryLabel, Qt::AlignLeft);

    m_mainLayout->addLayout(m_detailLayout);

    m_mainLayout->setSpacing(0);

    m_layout = new QVBoxLayout;

    m_layout->addLayout(m_mainLayout);

    m_controlsLayout = new QHBoxLayout;

    m_controlsLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Minimum));

    m_okButton = new QPushButton(tr("OK"));
    m_cancelButton = new QPushButton(tr("Cancel"));
    m_applyButton = new QPushButton(tr("Apply)"));

    connect(m_okButton, &QPushButton::clicked, [=](bool /*checked*/) {
        bool settingsValid = true;

        for(auto page : m_pages) {
            if (!page->m_pageSettings->canAcceptSettings()) {
                settingsValid = false;
                break;
            }
        }

        if (!settingsValid) {
            //TODO go to page with error
        } else {
            for(auto page : m_pages) {
                page->m_pageSettings->acceptSettings();
            }

            close();
        }
    });

    connect(m_applyButton, &QPushButton::clicked, [=](bool /*checked*/) {
        bool settingsValid = true;

        for(auto page : m_pages) {
            if (!page->m_pageSettings->canAcceptSettings()) {
                settingsValid = false;
                break;
            }
        }

        if (!settingsValid) {
            //TODO go to page with error
        } else {
            for(auto page : m_pages) {
                page->m_pageSettings->acceptSettings();
            }
        }
    });

    connect(m_cancelButton, &QPushButton::clicked, [=](bool /*checked*/) {
       close();
    });

    m_controlsLayout->addWidget(m_okButton);
    m_controlsLayout->addWidget(m_cancelButton);
    m_controlsLayout->addWidget(m_applyButton);

    m_layout->addLayout(m_controlsLayout);

    setLayout(m_layout);
#endif

    int listWidth = 0;

    for (auto page: pages) {
#if defined(Q_OS_MACOS)
        auto settingsPage = addPage(page);

        m_pages[settingsPage->m_toolBarItem] = settingsPage;
#else
        auto settingsPage = addPage(page);

        m_pages.append(settingsPage);
#endif
    }

    for (auto currentIndex=0;currentIndex<m_treeWidget->topLevelItemCount();currentIndex++) {
        auto item = m_treeWidget->topLevelItem(currentIndex);

        auto fontMetrics = QFontMetrics(m_treeWidget->font());

        auto width = fontMetrics.boundingRect(item->text(0)).width();

        if (width>listWidth) {
            listWidth = width;
        }
    }

    m_treeWidget->setMinimumWidth(listWidth+(settingsIconSize*2));
    m_treeWidget->setMaximumWidth(listWidth+(settingsIconSize*2));

#if defined(Q_OS_MACOS)
    m_toolBar->attachToWindow(nativeWindowHandle());

    Nedrysoft::MacHelper::enablePreferencesToolbar(this);

    QSize size(defaultMinimumWidth, 0);

    for(auto page : m_pages) {
        size = QSize(qMax(page->m_widget->sizeHint().width(), size.width()), qMax(page->m_widget->sizeHint().height(), size.height()));
    }

    m_toolbarHeight = frameGeometry().size().height()-geometry().size().height();
    m_maximumWidth = size.width();

    auto maximumHeight = 0;

    if (m_pages.first()) {
        m_currentPage = m_pages.first();

        m_currentPage->m_widget->setOpacity(1);

        setMinimumSize(QSize(m_maximumWidth, m_currentPage->m_widget->sizeHint().height()));
        setMaximumSize(QSize(m_maximumWidth, m_currentPage->m_widget->sizeHint().height()));

        if (m_currentPage->m_widget->sizeHint().height()>maximumHeight) {
            maximumHeight = m_currentPage->m_widget->sizeHint().height();
        }

        this->setWindowTitle(m_currentPage->m_name);
    }

    // this uses qApp->desktop()->rect() which is the union of all screen geometries in the case of multiple
    // screens.

    setGeometry(QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    QSize(m_maximumWidth, maximumHeight),
                    qApp->desktop()->rect() ));
#endif
}

auto Nedrysoft::SettingsDialog::SettingsDialog::sizeHint() -> QSize {
    if (m_currentPage) {
        return m_currentPage->m_widget->sizeHint();
    }

    return QWidget::sizeHint();
}

Nedrysoft::SettingsDialog::SettingsDialog::~SettingsDialog() {
#if defined(Q_OS_MACOS)
    delete m_toolBar;
#else
    delete m_layout;
    delete m_treeWidget;
    delete m_categoryLabel;
    delete m_stackedWidget;
#endif
}

auto Nedrysoft::SettingsDialog::SettingsDialog::okToClose() -> bool {
    auto acceptable = true;

#if !defined(Q_OS_MACOS)
    for(auto page : m_pages) {
        if (!page->m_pageSettings->canAcceptSettings()) {
            acceptable = false;

            break;
        }
    }
#endif
    return acceptable;
}

auto Nedrysoft::SettingsDialog::SettingsDialog::resizeEvent(QResizeEvent *event) -> void {
    for(auto page : m_pages) {
        if (page->m_widget) {
#if defined(Q_OS_MACOS)
            page->m_widget->resize((event->size()));
#else
            // resizing needs to account for the category label & margins

            auto margins = m_layout->contentsMargins();

            auto adjustment = margins.bottom()+m_categoryLabel->height()+m_layout->spacing();

            page->m_widget->resize(m_stackedWidget->size()-QSize(margins.right(), adjustment));
#endif
        }
    }
}

auto Nedrysoft::SettingsDialog::SettingsDialog::nativeWindowHandle() -> QWindow * {
    //
    // @note the call to winId() is required as it sets up windowHandle() to return the correct value,
    //       failing to call this will result in windowHandle not returning the correct value.

    window()->winId();

    return window()->windowHandle();
}

auto Nedrysoft::SettingsDialog::SettingsDialog::addPage(ISettingsPage *page) -> Nedrysoft::SettingsDialog::SettingsPage * {
#if defined(Q_OS_MACOS)
    TransparentWidget *widgetContainer = nullptr;
    SettingsPage *settingsPage = nullptr;

    for(auto currentPage : m_pages) {
        if (currentPage->m_name==page->section()) {
            widgetContainer = currentPage->m_widget;
            settingsPage = currentPage;

            break;
        }
    }

    if (!widgetContainer) {
        widgetContainer = new TransparentWidget(0, this);
    }

    if (widgetContainer->count()) {
        widgetContainer->addWidget(new SeparatorWidget);
    }

    auto pageWidget = page->createWidget();

    widgetContainer->addWidget(pageWidget);

    if (settingsPage) {
        return settingsPage;
    }

    settingsPage = new SettingsPage;

    settingsPage->m_name = page->section();
    settingsPage->m_widget = widgetContainer;
    settingsPage->m_pageSettings = page;
    settingsPage->m_icon = page->icon();
    settingsPage->m_description = page->description();

    if (pageWidget->layout()) {
        pageWidget->layout()->setSizeConstraint(QLayout::SetMinimumSize);
    }

    settingsPage->m_toolBarItem = m_toolBar->addItem(page->icon(), page->section());

    connect(settingsPage->m_toolBarItem, &QMacToolBarItem::activated, this, [this, settingsPage]() {
        if (!m_currentPage) {
            m_currentPage = settingsPage;
            m_currentPage->m_widget->setOpacity(1);

            resize(m_currentPage->m_widget->sizeHint());

            this->setWindowTitle(settingsPage->m_name);

            return;
        }

        auto currentItem = m_pages[m_currentPage->m_toolBarItem]->m_widget;
        auto nextItem = settingsPage->m_widget;

        if (currentItem==nextItem) {
            return;
        }

        if (m_animationGroup) {
            m_animationGroup->stop();
            m_animationGroup->deleteLater();
        }

        m_animationGroup = new QParallelAnimationGroup;

        auto minSize = QSize(m_maximumWidth, nextItem->sizeHint().height());

        auto propertyNames = {"size", "minimumSize", "maximumSize"};

        for(auto property : propertyNames) {
            auto sizeAnimation = new QPropertyAnimation(this, property);

            sizeAnimation->setDuration(transisionDuration.count());
            sizeAnimation->setStartValue(currentItem->size());
            sizeAnimation->setEndValue(minSize);

            m_animationGroup->addAnimation(sizeAnimation);
        }

        auto outgoingAnimation = new QPropertyAnimation(currentItem->transparencyEffect(), "opacity");

        outgoingAnimation->setDuration(transisionDuration.count());
        outgoingAnimation->setStartValue(currentItem->transparencyEffect()->opacity());
        outgoingAnimation->setEndValue(alphaTransparent);

        m_animationGroup->addAnimation(outgoingAnimation);

        auto incomingAnimation = new QPropertyAnimation(nextItem->transparencyEffect(), "opacity");

        incomingAnimation->setDuration(transisionDuration.count());
        incomingAnimation->setStartValue(nextItem->transparencyEffect()->opacity());
        incomingAnimation->setEndValue(alphaOpaque);

        m_animationGroup->addAnimation(incomingAnimation);

        m_animationGroup->start(QParallelAnimationGroup::DeleteWhenStopped);

        // the current page is set here immediately, so that if the page is changed again before the animation is
        // complete then the new selection will be animated in from the current position in the previous animation

        m_currentPage = settingsPage;

        connect(m_animationGroup, &QParallelAnimationGroup::finished, [this, settingsPage]() {
            m_animationGroup->deleteLater();

            m_animationGroup = nullptr;

            this->setWindowTitle(settingsPage->m_name);
        });
    });

    return settingsPage;
#else
    QTabWidget *tabWidget = nullptr;

    for (int currentItem=0;currentItem<m_treeWidget->topLevelItemCount();currentItem++) {
        QString itemSection = m_treeWidget->topLevelItem(currentItem)->text(0);

        if (page->section()==itemSection) {
            tabWidget = m_treeWidget->topLevelItem(currentItem)->data(0, Qt::UserRole).value<QTabWidget *>();

            break;
        }
    }

    if (!tabWidget) {
        auto treeItem = new QTreeWidgetItem(m_treeWidget);

        tabWidget = new QTabWidget();

        treeItem->setIcon(0, page->icon());
        treeItem->setText(0, page->section());
        treeItem->setData(0, Qt::UserRole, QVariant::fromValue(tabWidget));
        treeItem->setData(0, Qt::ToolTipRole, page->description());

        m_treeWidget->addTopLevelItem(treeItem);

        connect(m_treeWidget, &QTreeWidget::currentItemChanged, [=](QTreeWidgetItem *current, QTreeWidgetItem *previous) {
            Q_UNUSED(previous)

            auto widget = current->data(0, Qt::UserRole).value<QWidget *>();

            if (widget) {
                m_stackedWidget->setCurrentWidget(widget);
                m_categoryLabel->setText(current->text(0));
            }
        });
    }

    auto widget = new QWidget;
    auto widgetLayout = new QVBoxLayout;
    auto pageWidget = page->createWidget();

    widgetLayout->addWidget(pageWidget);
    widgetLayout->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Preferred, QSizePolicy::Expanding));

    widget->setLayout(widgetLayout);

    tabWidget->addTab(widget, page->category());

    m_stackedWidget->addWidget(tabWidget);

    auto settingsPage = new SettingsPage;

    settingsPage->m_name = page->section();
    settingsPage->m_widget = pageWidget;
    settingsPage->m_pageSettings = page;
    settingsPage->m_icon = page->icon();
    settingsPage->m_description = page->description();

    return settingsPage;
#endif
}

auto Nedrysoft::SettingsDialog::SettingsDialog::closeEvent(QCloseEvent *event) -> void {
    if (okToClose()) {
        for (auto page : m_pages) {
            page->m_pageSettings->acceptSettings();
        }

        event->accept();

        Q_EMIT closed();
    } else {
        event->ignore();
    }
}