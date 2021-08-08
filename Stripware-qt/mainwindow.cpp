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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QDialog>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QThread>
#include <QColor>
#include <QColorDialog>
#include <QLineEdit>
#include <QTimer>

int fadeAmount = 1;
int brightness = 255;

int rInc;
int gInc;
int bInc;

bool isReset = 0;
bool isPlaying = 0;
bool inc = 1;

QTimer *rgbTimer = new QTimer();
QColor rgbColor;
QString SERIALPORT;

MainWindow::MainWindow(QWidget *parent) ://Constructor
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->centralwidget);

    colorDialog = new QColorDialog(this);

    ser = new QSerialPort;

    auto exitAction = new QAction(tr("&Exit"), this);
    QObject::connect(exitAction, &QAction::triggered, [this](){
        close();
    });

    auto unhideAction = new QAction(tr("&Unhide"), this);
    QObject::connect(unhideAction, &QAction::triggered, [this](){
       show();
       activateWindow();
    });

    auto trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(unhideAction);
    trayIconMenu->addAction(exitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip("Stripware");
    trayIcon->setIcon(QIcon(":/swicon.png"));
    trayIcon->setVisible(true);
    trayIcon->show();

    QObject::connect(trayIcon, &QSystemTrayIcon::activated, [this](auto reason){
        if(reason == QSystemTrayIcon::Trigger){
            if(isVisible()){
                hide();
            }else{
                show();
                activateWindow();
            }
        }
    });

    QObject::connect(rgbTimer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::rgb));

    ui->comboBox->addItem("RGB Fade");

    ui->statusBar->addPermanentWidget(ui->comboBox_serialPorts,0);
    ui->statusBar->layout()->setContentsMargins(0, 0, 5, 0);

    foreach (const QSerialPortInfo &ThisPort, QSerialPortInfo::availablePorts())
    {
        ui->comboBox_serialPorts->addItem(ThisPort.portName());
    }
}

void MainWindow::initPort(QString value){
    SERIALPORT = value;
    qDebug() << "Opening port:" << SERIALPORT;
    // open and configure the serialport
    ser->setPortName(SERIALPORT);
    ser->open(QSerialPort::WriteOnly);
    ser->setBaudRate(QSerialPort::Baud9600);
    ser->setDataBits(QSerialPort::Data8);
    ser->setParity(QSerialPort::NoParity);
    ser->setStopBits(QSerialPort::OneStop);
    ser->setFlowControl(QSerialPort::NoFlowControl);
    if(ser->isOpen()){
        qDebug() << "Serial port " + SERIALPORT + " has been opened!";
        ui->statusBar->showMessage("Connected to: " + SERIALPORT);
    }else{
        qDebug() << "Serial port " + SERIALPORT + " cannot be opened!";
        ui->statusBar->showMessage("Unable to connected to: " + SERIALPORT);
    }
    refresh();
}

void MainWindow::resetController()
{
    // Initialize the controller board for software control
    ser->write("x");
    ser->write("c");
}

MainWindow::~MainWindow()//Destructor
{
    if(ser->isOpen()){
        ser->close();
    }
    delete ui;
}

void MainWindow::on_red_slider_valueChanged(int value)
{
    ui->red_value_label->setText(QString("<span style=\" font-size:18pt; font-weight:600;\">%1</span>").arg(value));
    writeSerial(QString("r%1").arg(value));
    rgbColor.setRed(value);
}

void MainWindow::on_green_slider_valueChanged(int value)
{
    ui->green_value_label->setText(QString("<span style=\" font-size:18pt; font-weight:600;\">%1</span>").arg(value));
    writeSerial(QString("g%1").arg(value));
    rgbColor.setGreen(value);
}

void MainWindow::on_blue_slider_valueChanged(int value)
{
    ui->blue_value_label->setText(QString("<span style=\" font-size:18pt; font-weight:600;\">%1</span>").arg(value));
    writeSerial(QString("b%1").arg(value));
    rgbColor.setBlue(value);
}

void MainWindow::writeSerial(QString command)
{
    if(!isReset){ //reset the controller for the first time so its expects the rgb values
        isReset = 1;
        resetController();
    }
    if(ser->isWritable()){
        ser->write(command.toStdString().c_str());
    }else{
        qDebug() << "Unable to write to serial port! " << SERIALPORT;
        ui->statusBar->showMessage("Unable to write to port: " + SERIALPORT);
    }
    refresh();
}

void MainWindow::refresh() //Refresh the ui controls
{ 
    ui->colorFrame->setStyleSheet("background-color: "+rgbColor.name()+";"); //Update colorFrame background color
    ui->comboBox_serialPorts->setCurrentText(SERIALPORT);
    ui->red_slider->setValue(rgbColor.red());
    ui->green_slider->setValue(rgbColor.green());
    ui->blue_slider->setValue(rgbColor.blue());
    isPlaying ? ui->pushButton_2->setText("Stop") : ui->pushButton_2->setText("Start");
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox exitMsg;
    exitMsg.setText("Exiting application");
    exitMsg.setInformativeText("Do you want to exit?");
    exitMsg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    exitMsg.setDefaultButton(QMessageBox::No);

    switch(exitMsg.exec()){
        case QMessageBox::Yes:
            QWidget::closeEvent(e);
            e->accept();
            break;
        case QMessageBox::No:
            e->ignore();
            break;
    }
}

void MainWindow::rgb()
{
      if(rgbColor.red() >= brightness){
          rInc = -fadeAmount;
          gInc = fadeAmount;
      }
      if(rgbColor.green() >= brightness){
          gInc=-fadeAmount;
          bInc = fadeAmount;
      }
      if(rgbColor.blue() >= brightness){
          bInc=-fadeAmount;
          rInc = fadeAmount;
      }

      rgbColor.setRed(rgbColor.red() + rInc);
      rgbColor.setGreen(rgbColor.green() + gInc);
      rgbColor.setBlue(rgbColor.blue() + bInc);
      refresh();
}

int MainWindow::inverse(int arg1){ //Used for inverting the effect time value
    return pow(1.0 * arg1, -1) * 1000;
}

void MainWindow::on_pushButton_2_clicked()
{
    if(isPlaying){
        isPlaying = false;
        rgbTimer->stop();
        qDebug() << "Stopped effect";
    }else{
        isPlaying = true;
        int value = ui->comboBox->currentIndex();
        qDebug() << "Starting effect:" << ui->comboBox->itemText(value);

        switch(value){
            case 0:
                rInc = 0;
                gInc = fadeAmount;
                bInc = 0;

                rgbColor.setRgb(0,0,0);
                rgbTimer->start(inverse(ui->spinBox_2->value()));

                qDebug() << "Speed:" << inverse(ui->spinBox_2->value());
                qDebug() << "Fade amount:" << fadeAmount;
                qDebug() << "Brightness:" << brightness;
                break;
            default :
                qDebug() << "Effect missing";
                break;
        }
    }
    refresh();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    fadeAmount = arg1;
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    rgbTimer->setInterval(inverse(arg1));
}

void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    brightness = arg1;
}

void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}

void MainWindow::on_actionHide_in_tray_triggered()
{
    hide();
}

void MainWindow::on_pushButton_4_clicked()
{
    rgbColor = colorDialog->getColor();
    if(rgbColor.isValid()){
        refresh();
        qDebug() << "Color picker returned: " << rgbColor.name();
    }
}

void MainWindow::on_actionAbout_triggered()
{
    aboutDialog aboutDialog;
    aboutDialog.exec();
}

void MainWindow::on_actionReset_triggered() //Manually reset the controller if SHTF
{
    resetController();
}

void MainWindow::on_comboBox_serialPorts_activated(int index)
{
    initPort(ui->comboBox_serialPorts->itemText(index));
}

