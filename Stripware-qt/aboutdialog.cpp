/* Copyright 2021 nukecat
 * This file is part of Stripware
 *
 * Stripware is a free software: you can redistribute it and or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
 *
 * Stripware is distributed in hope that it wil be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Stripware.
 * If not, see http://www.gnu.org/licenses/
 */

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);
    ui->label->setTextFormat(Qt::RichText);
    ui->label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label->setOpenExternalLinks(true);
    ui->versionLabel->setText(QString::fromStdString(std::string("Stripware v")+std::string(APP_VERSION)));
    setWindowFlags(Qt::Tool);
}

aboutDialog::~aboutDialog()
{
    delete ui;
}
