
// VisualPend - Mathematical pendulum simulator
//
//Copyright (c) 2015 Constantin Mihalahce <mihalache.c94@gmai.com>
//Copyright (c) 2105 Tudor-Marian Surdoiu <studormarian@yahoo.co.uk>
//Copyright (c) 2015 Stefania-Cristiana Colbu <cristinacolbu@yahoo.com>
//
//  This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QKeyEvent>
#include "Helper.h"
#include "DrawWidget.h"
#include <QTime>
#include <QMenuBar>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMenuBar *menuBar;
    Helper *helper;
    QTimer *timer;
    DrawWidget *native;
    QTime *simulationTime;

    bool velocityDrawing;
    bool accelerationDrawing;


private:
    void disableInput(bool var);

private slots:
    void startTimer();
    void stopTimer();
    void resetSimulation();
    void setTotalTime();

    void setVelocityDrawing(bool var);
    void setAccelerationDrawing(bool var);

    void handleMenuAction(QAction *a);

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
