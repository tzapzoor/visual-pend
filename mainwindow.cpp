
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QtCore/qmath.h>

#include "AboutDialog.h"

#define fps 60

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    menuBar(new QMenuBar),
    simulationTime( new QTime() ),
    velocityDrawing(false),
    accelerationDrawing(false)
{
    ui->setupUi(this);

    QIcon icon;
    icon.addFile(":/media/icon.ico");
    setWindowIcon(icon);

    menuBar->addAction("About");
    ui->gridLayout->setMenuBar(menuBar);

    helper = new Helper(this->size());

    QVBoxLayout *layout = new QVBoxLayout;
    native = new DrawWidget(helper, ui->frame, fps);
    layout->addWidget(native);
    ui->frame->setLayout(layout);

    timer = new QTimer(this);

    ui->m_stopButton->setDisabled(true);

    connect(menuBar, SIGNAL(triggered(QAction*)), this, SLOT(handleMenuAction(QAction*)));

    connect(timer, SIGNAL(timeout()), native, SLOT(animate()));
    connect(timer, SIGNAL(timeout()), this, SLOT(setTotalTime()));

    connect(ui->m_startButton, SIGNAL(pressed()), this, SLOT(startTimer()));
    connect(ui->m_stopButton, SIGNAL(pressed()), this, SLOT(stopTimer()));
    connect(ui->m_angleSpinBox, SIGNAL(valueChanged(double)), native, SLOT(setAngle(double)));
    connect(ui->m_resetButton, SIGNAL(pressed()), this, SLOT(resetSimulation()));
    connect(ui->m_bilateralRadioButton, SIGNAL(toggled(bool)), native, SLOT(setBilateral(bool)));
    connect(ui->m_bilateralRadioButton, SIGNAL(toggled(bool)), this, SLOT(resetSimulation()));
    connect(ui->m_radiusSpinBox, SIGNAL(valueChanged(double)), native, SLOT(setRadius(double)));
    connect(ui->m_radiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(resetSimulation()));
    connect(ui->m_pathCheckBox, SIGNAL(toggled(bool)), native, SLOT(enablePathDrawing(bool)));
    connect(ui->m_angularVelocitySpinBox, SIGNAL(valueChanged(double)), native, SLOT(setAngularVelocity(double)));
    connect(ui->m_angularVelocitySpinBox, SIGNAL(valueChanged(double)), this, SLOT(resetSimulation()));

    connect(ui->m_velocityCheckBox, SIGNAL(toggled(bool)),  this, SLOT(setVelocityDrawing(bool)));
    connect(ui->m_accelerationCheckBox, SIGNAL(toggled(bool)), this, SLOT(setAccelerationDrawing(bool)));

    connect(ui->m_gravitationalAcceleration, SIGNAL(valueChanged(double)), native, SLOT(setGravitationalAcceleration(double)));
    connect(ui->m_gravitationalAcceleration, SIGNAL(valueChanged(double)), this, SLOT(resetSimulation()));
    connect(ui->m_angleSpinBox, SIGNAL(valueChanged(QString)), ui->m_angleOutput, SLOT(setText(QString)));

    connect(native, SIGNAL(stopAnimation()), this, SLOT(stopTimer()));
    connect(native, SIGNAL(angularVelocityChanged(QString)), ui->m_angularVelocityOutput, SLOT(setText(QString)));
    connect(native, SIGNAL(angleChanged(QString)), ui->m_angleOutput, SLOT(setText(QString)));
    connect(native, SIGNAL(angularAccelerationChanged(QString)), ui->m_angularAccelerationOutput, SLOT(setText(QString)));
    connect(native, SIGNAL(accelerationChanged(QString)), ui->m_accelerationOutput, SLOT(setText(QString)));
    connect(native, SIGNAL(velocityChanged(QString)), ui->m_velocityOutput, SLOT(setText(QString)));
    connect(native, SIGNAL(accelerationDrawing(bool)), ui->m_accelerationCheckBox, SLOT(setChecked(bool)));
    connect(native,SIGNAL(velocityDrawing(bool)), ui->m_velocityCheckBox, SLOT(setChecked(bool)));

    resetSimulation();
}

void MainWindow::startTimer()
{
    timer->start(1000/fps);
    native->enableVelocityDrawing(velocityDrawing);
    native->enableAccelerationDrawing(accelerationDrawing);
    simulationTime->restart();
    disableInput(true);
}

void MainWindow::stopTimer()
{
    timer->stop();
    if(velocityDrawing)
        native->enableVelocityDrawing(false);
    if(accelerationDrawing)
        native->enableAccelerationDrawing(false);
    disableInput(false);
}

void MainWindow::resetSimulation()
{
    stopTimer();
    native->setAngle(ui->m_angleSpinBox->value());
    native->setRadius(ui->m_radiusSpinBox->value());
    native->setAngularVelocity(ui->m_angularVelocitySpinBox->value());
    ui->m_simulationTime->setText("00:00:00");
    ui->m_angleOutput->setText(ui->m_angleSpinBox->text());
}

MainWindow::~MainWindow()
{
    delete simulationTime;
    delete helper;
    delete timer;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape)
        this->close();
}

void MainWindow::disableInput(bool var)
{
    ui->m_angleSpinBox->setDisabled(var);
    ui->m_startButton->setDisabled(var);
    ui->m_stopButton->setDisabled(!var);
    ui->m_bilateralRadioButton->setDisabled(var);
    ui->m_unilateralRadioButton->setDisabled(var);
    ui->m_radiusSpinBox->setDisabled(var);
    ui->m_angularVelocitySpinBox->setDisabled(var);
    ui->m_gravitationalAcceleration->setDisabled(var);
}

void MainWindow::setTotalTime()
{
    int elapsed = simulationTime->elapsed();
    int hours = elapsed/(36*(int)pow(10,5));
    int minutes = elapsed/(6*(int)pow(10,4));
    int seconds = elapsed/(int)pow(10,3);
    QString H,M,S;

    if(hours/10==0) H+="0";
    H+=QString::number(hours);

    if((minutes-60*hours)/10==0) M+="0";
    M+=QString::number(minutes-60*hours);

    if((seconds-60*minutes)/10==0) S+="0";
    S+=QString::number(seconds-60*minutes);

    ui->m_simulationTime->setText(H+":"+M+":"+S);
}

void MainWindow::handleMenuAction(QAction *a)
{
    if(a->text()=="About")
    {
        AboutDialog *dialog = new AboutDialog;
        dialog->show();
        return;
    }
    if(a->text()=="Help")
    {

        return;
    }
}

void MainWindow::setVelocityDrawing(bool var)
{
    velocityDrawing = var;
    if(timer->isActive())
        native->enableVelocityDrawing(var);
}

void MainWindow::setAccelerationDrawing(bool var)
{
    accelerationDrawing = var;
    if(timer->isActive())
        native->enableAccelerationDrawing(var);
}
