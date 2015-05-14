
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

#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>

class Helper;

class DrawWidget : public QWidget
{
    Q_OBJECT

public:
    DrawWidget(Helper *helper, QWidget *parent, const int fps);

public slots:
    void animate();

    /* the angle is converted to radians from degrees */
    void setAngle(qreal angle);

    void setBilateral(bool var);
    void setRadius(qreal radius);
    void setAngularVelocity(double v);
    void setGravitationalAcceleration(double var);

    void enablePathDrawing(bool var);
    void enableVelocityDrawing(bool var);
    void enableAccelerationDrawing(bool var);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
    void stopAnimation();
    void resetPath();

    void accelerationDrawing(bool);
    void velocityDrawing(bool);

    void angularVelocityChanged(const QString&);
    void angularAccelerationChanged(const QString&);
    void velocityChanged(const QString&);
    void accelerationChanged(const QString&);
    void angleChanged(const QString&);

private:
    Helper *m_helper;
    int m_elapsed;
    int m_fps;
    qreal m_dt;
    qreal m_g;

    /* pendulum specific */
    qreal m_radius;
    qreal m_angle;
    qreal m_angularVelocity;
    qreal m_angularAcceleration;
    qreal m_accelerationSize;
    QPointF m_position;
    QPointF m_initialPosition;

    bool m_isBilateral;
    qreal m_velocitySize;
    QPointF m_velocityPos;
    QPointF m_accelerationPos;

    bool m_freeFall;

    qreal m_velocityX, m_velocityY;
    qreal m_accelerationX, m_accelerationY;

private:
    qreal limitAngle(qreal angle);
    qreal currentAngle(QPointF position);
    qreal truncate(qreal x);
};

#endif // DRAWWIDGET_H
