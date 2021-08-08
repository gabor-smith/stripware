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
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

static bool startMininimized;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Stripware");
    QApplication::setApplicationVersion(APP_VERSION);

    startMininimized = false;

    QCommandLineParser parser;
    parser.setApplicationDescription("\nStripware - Copyright (c) 2021 nukecat\r\n"
                                     "RGB LED strip controller software\r\n\n"
                                     "This program is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\nany later version.\r\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\nhttp://www.gnu.org/licenses/");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption portOption({"p","port"}, "Use this serial port", "path", "/dev/ttyUSB0");
    parser.addOption(portOption);

    QCommandLineOption startMininimizedOption({"m","min","minimized"}, "Start minimized to the tray");
    parser.addOption(startMininimizedOption);

    parser.process(app);

    QString sPort;
    sPort = parser.value(portOption);

    if(parser.isSet(startMininimizedOption)){
        startMininimized = true;
    }else{
        startMininimized = false;
    }

    MainWindow w;
    w.initPort(sPort);
    w.setFixedSize(504, 404);

    if(!startMininimized){
        w.show();
    }

    return app.exec();
}
