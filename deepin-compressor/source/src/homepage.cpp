/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     dongsen <dongsen@deepin.com>
 *
 * Maintainer: dongsen <dongsen@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "homepage.h"
#include "utils.h"
#include <QApplication>
#include <QDir>
#include <DFileDialog>
#include <DPalette>
#include <QShortcut>
DGUI_USE_NAMESPACE

HomePage::HomePage(QWidget *parent)
    : QWidget(parent),
      m_layout(new QVBoxLayout(this)),
      m_iconLabel(new DLabel),
      m_tipsLabel(new DLabel(tr("Drag files or folders to this"))),
      m_splitLine(new DLabel),
      m_chooseBtn(new DCommandLinkButton(tr("Select the file"))),
      m_settings(new QSettings(QDir(Utils::getConfigPath()).filePath("config.conf"),
                               QSettings::IniFormat))
{
    m_unloadPixmap = Utils::renderSVG(":/images/folder-128 .svg", QSize(128, 128));
    m_loadedPixmap = Utils::renderSVG(":/images/folder-128 .svg", QSize(128, 128));

    m_iconLabel->setFixedSize(128, 128);
    m_iconLabel->setPixmap(m_unloadPixmap);

    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if(themeType == DGuiApplicationHelper::LightType)
        m_splitLine->setPixmap(QPixmap(":/images/split_line.svg"));
    else if(themeType == DGuiApplicationHelper::DarkType)
        m_splitLine->setPixmap(QPixmap(":/images/split_line_dark.svg"));
    else
        m_splitLine->setPixmap(QPixmap(":/images/split_line.svg"));

    m_tipsLabel->setFont(DFontSizeManager::instance()->get(DFontSizeManager::T8));
    DPalette pa;
    pa = DApplicationHelper::instance()->palette(m_tipsLabel);
    pa.setBrush(DPalette::WindowText, pa.color(DPalette::TextTips));
    m_tipsLabel->setPalette(pa);
    m_chooseBtn->setFont(DFontSizeManager::instance()->get(DFontSizeManager::T8));

    // initalize the configuration file.
    if (m_settings->value("dir").toString().isEmpty()) {
        m_settings->setValue("dir", "");
    }

    DLabel* buttomlabel = new DLabel();
    buttomlabel->setFixedHeight(50);

    m_layout->addStretch();
    m_layout->addWidget(m_iconLabel, 0, Qt::AlignHCenter);
    m_layout->addSpacing(13);
    m_layout->addWidget(m_tipsLabel, 0, Qt::AlignHCenter);
    m_layout->addSpacing(15);
    m_layout->addWidget(m_splitLine, 0, Qt::AlignHCenter);
    m_layout->addSpacing(15);
    m_layout->addWidget(m_chooseBtn, 0, Qt::AlignHCenter);
    m_layout->addWidget(buttomlabel, 0, Qt::AlignHCenter);
    m_layout->addStretch();

    connect(m_chooseBtn, &DCommandLinkButton::clicked, this, &HomePage::onChooseBtnClicked);

    m_spinner = new DSpinner(this);
    m_spinner->setFixedSize(40, 40);

    m_spinner->move(285, 200);
    m_spinner->hide();

    auto openkey = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this);
    openkey->setContext(Qt::ApplicationShortcut);
    connect(openkey, &QShortcut::activated, this, &HomePage::onChooseBtnClicked);
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
                        this, &HomePage::themeChanged);

    setBackgroundRole(DPalette::Background);
}

HomePage::~HomePage()
{
}

void HomePage::setIconPixmap(bool isLoaded)
{
    if (isLoaded) {
        m_iconLabel->setPixmap(m_loadedPixmap);
    } else {
        m_iconLabel->setPixmap(m_unloadPixmap);
    }
}

void HomePage::spinnerStart()
{
    m_spinner->show();
    m_spinner->start();
    m_chooseBtn->setEnabled(false);
}

void HomePage::spinnerStop()
{
    m_spinner->hide();
    m_spinner->stop();
    m_chooseBtn->setEnabled(true);
}

void HomePage::onChooseBtnClicked()
{
    DFileDialog dialog;
    dialog.setAcceptMode(DFileDialog::AcceptOpen);
    dialog.setFileMode(DFileDialog::ExistingFiles);
    dialog.setAllowMixedSelection(true);
//    dialog.setNameFilter(Utils::suffixList());

    QString historyDir = m_settings->value("dir").toString();
    if (historyDir.isEmpty()) {
        historyDir = QDir::homePath();
    }
    dialog.setDirectory(historyDir);

    const int mode = dialog.exec();

    // save the directory string to config file.
    m_settings->setValue("dir", dialog.directoryUrl().toLocalFile());

    // if click cancel button or close button.
    if (mode != QDialog::Accepted) {
        return;
    }

    emit fileSelected(dialog.selectedFiles());
}

void HomePage::themeChanged()
{
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if(themeType == DGuiApplicationHelper::LightType)
        m_splitLine->setPixmap(QPixmap(":/images/split_line.svg"));
    else if(themeType == DGuiApplicationHelper::DarkType)
        m_splitLine->setPixmap(QPixmap(":/images/split_line_dark.svg"));
    else
        m_splitLine->setPixmap(QPixmap(":/images/split_line.svg"));
}
