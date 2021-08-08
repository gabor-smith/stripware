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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class aboutDialog;
}

class aboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit aboutDialog(QWidget *parent = nullptr);
    ~aboutDialog();

private:
    Ui::aboutDialog *ui;
};

#endif // ABOUTDIALOG_H
