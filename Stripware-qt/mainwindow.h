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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMenuBar>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <QThread>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QColorDialog>
#include <aboutdialog.h>

namespace Ui {
    class MainWindow;
}

class QSystemTrayIcon;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    //Constructor
    explicit MainWindow(QWidget *parent = 0);

    void initPort(QString value);

    //Destructor
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *e);
private slots:

    void on_red_slider_valueChanged(int value);

    void on_green_slider_valueChanged(int value);

    void on_blue_slider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);

    void writeSerial(QString);

    void rgb();

    void on_actionExit_triggered();

    void on_actionHide_in_tray_triggered();

    void on_pushButton_4_clicked();

    void refresh();

    int inverse(int arg1);

    void on_actionAbout_triggered();

    void resetController();

    void on_actionReset_triggered();

    void on_comboBox_serialPorts_activated(int index);

private:
    Ui::MainWindow *ui;
    QSerialPort *ser;
    QString arduino_port_name;
    QColorDialog *colorDialog;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // MAINWINDOW_H
