/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     gaoxiang <gaoxiang@uniontech.com>
*
* Maintainer: gaoxiang <gaoxiang@uniontech.com>
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

#ifndef UNCOMPRESSPAGE_H
#define UNCOMPRESSPAGE_H

#include "commonstruct.h"

#include <DWidget>

DWIDGET_USE_NAMESPACE

class UnCompressView;
class CustomCommandLinkButton;
class CustomPushButton;

// 解压界面（列表）
class UnCompressPage : public DWidget
{
    Q_OBJECT
public:
    explicit UnCompressPage(QWidget *parent = nullptr);
    ~UnCompressPage() override;

    /**
     * @brief setArchiveName    设置压缩包名称
     * @param strArchiveName    压缩包名称（含路径）
     */
    void setArchiveName(const QString &strArchiveName);

    /**
     * @brief archiveName   获取压缩包名称
     * @return
     */
    QString archiveName();

    /**
     * @brief setDefaultUncompressPath  设置默认解压路径
     * @param strPath   解压路径
     */
    void setDefaultUncompressPath(const QString &strPath);

    /**
     * @brief setArchiveData   设置加载完之后的数据
     * @param stArchiveData 压缩包数据
     */
    void setArchiveData(const ArchiveData &stArchiveData);

private:
    /**
     * @brief initUI    初始化界面
     */
    void initUI();

    /**
     * @brief initConnections   初始化信号槽
     */
    void initConnections();

Q_SIGNALS:
    /**
     * @brief signalUncompress  点击解压按钮
     * @param strTargetPath     解压目标路径
     */
    void signalUncompress(const QString &strTargetPath);

private Q_SLOTS:
    /**
     * @brief slotUncompressClicked 解压按钮点击槽函数
     */
    void slotUncompressClicked();

private:
    UnCompressView *m_pUnCompressView;    // 压缩列表
    CustomCommandLinkButton *m_pUncompressPathBtn;
    CustomPushButton *m_pUnCompressBtn;   // 解压

    QString m_strArchiveName;       // 压缩包名称
    QString m_strUnCompressPath;    // 解压路径
};

#endif // UNCOMPRESSPAGE_H
