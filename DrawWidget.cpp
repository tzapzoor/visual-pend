
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

#include "DrawWidget.h"
#include "Helper.h"

#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QLineF>

/* ToDO:
 *
 *  Clean the code in this file!
 */

#define PI 3.14159265358979323846
#define epsilon 0.0001
#define CIRCLE_RADIUS 150 /* the actual circle size */
#define VELOCITY_BASE 30.0
#define VELOCITY_MAX 100.0
#define ACCELERATION_MAX 100.0
#define ACCELERATION_BASE 5.0

DrawWidget::DrawWidget(Helper *helper, QWidget *parent, const int fps):
    QWidget(parent),
    m_helper(helper),
    m_elapsed(0),
    m_fps(fps),
    m_dt(1.0/fps),
    m_g(9.80),
    m_radius(0),
    m_angle(0),
    m_angularVelocity(0),
    m_angularAcceleration(0),
    m_accelerationSize(0),
    m_position(QPointF(0,0)),
    m_initialPosition(QPointF(0,0)),
    m_isBilateral(true),
    m_velocitySize(0),
    m_velocityPos(QPointF(0,0)),
    m_accelerationPos(QPointF(0,0)),
    m_freeFall(false),
    m_velocityX(0),
    m_velocityY(0),
    m_accelerationX(0),
    m_accelerationY(0),
    m_initialAngularVelocity(0)
{
    setMinimumSize(200, 200);
    setAutoFillBackground(false);
}

void DrawWidget::animate()
{
    m_elapsed = (m_elapsed + qobject_cast<QTimer*>(sender())->interval());

    m_angle = limitAngle(m_angle);

    if(m_isBilateral)
    {
        m_angle += m_angularVelocity*m_dt; /* amortizarea timpului virtual */
        m_angularAcceleration = (-1)*m_g*qSin(m_angle)/m_radius;
        m_angularVelocity += m_angularAcceleration*m_dt;
        m_position = QPointF(CIRCLE_RADIUS*qSin(m_angle),CIRCLE_RADIUS*qCos(m_angle));

        m_velocitySize = fabs(m_radius*m_angularVelocity);
        qreal velocitySize;
        if(m_angularVelocity>0)
            velocitySize =  qMin(m_angularVelocity*m_radius*VELOCITY_BASE, VELOCITY_MAX);
        else
            velocitySize =  qMax(m_angularVelocity*m_radius*VELOCITY_BASE, (-1)*VELOCITY_MAX);
        int drawnVelocityRadius = (int)sqrt(pow(velocitySize,2)+pow(CIRCLE_RADIUS,2));
        qreal BetaAngle = qAtan((qreal)velocitySize/CIRCLE_RADIUS) + m_angle;
        m_velocityPos = QPointF(drawnVelocityRadius*qSin(BetaAngle),drawnVelocityRadius*qCos(BetaAngle));

        qreal accelerationY = -CIRCLE_RADIUS*qCos(m_angle)*pow(m_angularVelocity,2)-CIRCLE_RADIUS*qSin(m_angle)*m_angularAcceleration;
        qreal accelerationX = -CIRCLE_RADIUS*qSin(m_angle)*pow(m_angularVelocity,2)+CIRCLE_RADIUS*qCos(m_angle)*m_angularAcceleration;
        qreal AlphaAngle = currentAngle(QPointF((accelerationX-m_position.x()),(accelerationY-m_position.y())));
        m_accelerationSize = sqrt(pow(accelerationX*m_radius/(qreal)(CIRCLE_RADIUS),2)+pow(accelerationY*m_radius/(qreal)(CIRCLE_RADIUS),2));
        qreal drawnAccelerationSize;
        if(ACCELERATION_BASE*m_accelerationSize > ACCELERATION_MAX)
            drawnAccelerationSize = ACCELERATION_MAX;
        else
            drawnAccelerationSize = m_accelerationSize*ACCELERATION_BASE;
        m_accelerationPos = QPointF(drawnAccelerationSize*qSin(AlphaAngle)+m_position.x(),drawnAccelerationSize*qCos(AlphaAngle)+m_position.y());

        emit angularVelocityChanged(QString::number(truncate(m_angularVelocity))+" m/s");
        emit angularAccelerationChanged(QString::number(truncate(m_angularAcceleration))+" m/s²");
    }
    else
    {
        if(!m_freeFall)
        {
            m_angle += m_angularVelocity*m_dt;
            m_angularAcceleration = (-1)*m_g*qSin(m_angle)/m_radius;
            m_angularVelocity += m_angularAcceleration*m_dt;
            m_position = QPointF(CIRCLE_RADIUS*qSin(m_angle),CIRCLE_RADIUS*qCos(m_angle));

            qreal cN = m_radius*pow(m_angularVelocity,2)+m_g*qCos(m_angle);

            m_velocityY = -m_radius*qSin(m_angle)*m_angularVelocity;
            m_velocityX =  m_radius*qCos(m_angle)*m_angularVelocity;

            m_velocitySize = fabs(m_radius*m_angularVelocity);
            qreal velocitySize;
            if(m_angularVelocity>0)
                velocitySize =  qMin(m_angularVelocity*m_radius*VELOCITY_BASE, VELOCITY_MAX);
            else
                velocitySize =  qMax(m_angularVelocity*m_radius*VELOCITY_BASE, (-1)*VELOCITY_MAX);
            int drawnVelocityRadius = (int)sqrt(pow(velocitySize,2)+pow(CIRCLE_RADIUS,2));
            qreal BetaAngle = qAtan((qreal)velocitySize/CIRCLE_RADIUS) + m_angle;
            m_velocityPos = QPointF(drawnVelocityRadius*qSin(BetaAngle),drawnVelocityRadius*qCos(BetaAngle));

            m_accelerationX = -m_radius*qSin(m_angle)*pow(m_angularVelocity,2)+m_radius*qCos(m_angle)*m_angularAcceleration;
            m_accelerationY = -m_radius*qCos(m_angle)*pow(m_angularVelocity,2)-m_radius*qSin(m_angle)*m_angularAcceleration;

            qreal accelerationY = -CIRCLE_RADIUS*qCos(m_angle)*pow(m_angularVelocity,2)-CIRCLE_RADIUS*qSin(m_angle)*m_angularAcceleration;
            qreal accelerationX = -CIRCLE_RADIUS*qSin(m_angle)*pow(m_angularVelocity,2)+CIRCLE_RADIUS*qCos(m_angle)*m_angularAcceleration;
            qreal AlphaAngle = currentAngle(QPointF((accelerationX-m_position.x()),(accelerationY-m_position.y())));
            m_accelerationSize = sqrt(pow(accelerationX*m_radius/(qreal)(CIRCLE_RADIUS),2)+pow(accelerationY*m_radius/(qreal)(CIRCLE_RADIUS),2));
            qreal drawnAccelerationSize;
            if(ACCELERATION_BASE*m_accelerationSize >ACCELERATION_MAX)
                drawnAccelerationSize = ACCELERATION_MAX;
            else
                drawnAccelerationSize = m_accelerationSize*ACCELERATION_BASE;
            m_accelerationPos = QPointF(drawnAccelerationSize*qSin(AlphaAngle)+m_position.x(),drawnAccelerationSize*qCos(AlphaAngle)+m_position.y());

            if(cN<0)
            {
                m_freeFall=true;
                m_velocityX *=((qreal)CIRCLE_RADIUS/m_radius);
                m_velocityY *=((qreal)CIRCLE_RADIUS/m_radius);
                m_accelerationSize = m_g;


                emit angularVelocityChanged("Not applicable.");
                emit angularAccelerationChanged("Not applicable.");
                return;
            }

            emit angularVelocityChanged(QString::number(truncate(m_angularVelocity))+" m/s");
            emit angularAccelerationChanged(QString::number(truncate(m_angularAcceleration))+" m/s²");
        }
        else
        {
            qreal tempAcceleration = m_g*((qreal)CIRCLE_RADIUS/m_radius);
            qreal tempVelocity = m_velocityY+tempAcceleration*m_dt;
            QPointF tempPosition = QPointF(m_position.x()+m_velocityX*m_dt, m_position.y()+tempVelocity*m_dt);

            qreal dist = sqrt(pow(tempPosition.x(),2)+pow(tempPosition.y(),2));

            if(dist>CIRCLE_RADIUS)
            {
                m_freeFall=false;
                // the angular velocity when hiting the circle after free fall
                m_angularVelocity = (m_velocityX*qCos(m_angle)-m_velocityY*qSin(m_angle))/CIRCLE_RADIUS;
                m_angularAcceleration = (-1)*m_g*qSin(m_angle)/m_radius;
                m_angularVelocity+=m_angularAcceleration*m_dt;
                m_angle+=m_angularVelocity*m_dt;
                m_position = QPointF(CIRCLE_RADIUS*qSin(m_angle),CIRCLE_RADIUS*qCos(m_angle));
            }
            else
            {
                if(dist<CIRCLE_RADIUS)
                    m_angle = currentAngle(m_position);

                m_accelerationY = tempAcceleration;

                m_velocityY = tempVelocity;
                m_velocitySize = sqrt(pow(m_velocityX*((qreal)m_radius/CIRCLE_RADIUS),2)+pow(m_velocityY*((qreal)m_radius/CIRCLE_RADIUS),2));

                qreal drawnVelocitySize;

                if(VELOCITY_BASE*m_accelerationSize > VELOCITY_MAX)
                    drawnVelocitySize = VELOCITY_MAX;
                else
                    drawnVelocitySize = m_velocitySize*VELOCITY_BASE;

                qreal alpha = currentAngle(QPointF(m_velocityX,m_velocityY));

                m_position = tempPosition;

                m_accelerationPos = QPointF(m_position.x(),m_position.y()+m_g*ACCELERATION_BASE);

                m_velocityPos = QPointF(drawnVelocitySize*qSin(alpha)+m_position.x(), drawnVelocitySize*qCos(alpha)+m_position.y());

            }
        }
    }

    emit angleChanged(QString::number(truncate(qRadiansToDegrees(m_angle)))+"°");
    emit accelerationChanged(QString::number(truncate(m_accelerationSize))+" m/s²");
    emit velocityChanged(QString::number(truncate(m_velocitySize))+" m/s");

    update();

    qreal output = truncate(qRadiansToDegrees(limitAngle(m_angle)));
    angle_data.append(output);

}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    m_helper->paint(&painter, event, CIRCLE_RADIUS,
                    m_initialPosition, m_position, m_velocityPos, m_accelerationPos);
    painter.end();
}

void DrawWidget::setAngle(double angle)
{
    m_freeFall=false;
    m_angle = (qreal)qDegreesToRadians(angle);
    m_velocityX=m_velocityY= m_accelerationX = m_accelerationY =0;
    m_angularAcceleration=0;
    m_angularVelocity=m_initialAngularVelocity;
    m_initialPosition = m_position = QPointF(CIRCLE_RADIUS*qSin(m_angle),CIRCLE_RADIUS*qCos(m_angle));
    m_helper->cleanPath();
    update();
}

void DrawWidget::setBilateral(bool var)
{
    m_isBilateral = var;
    m_freeFall=false;
    m_helper->cleanPath();
    update();
}

void DrawWidget::setRadius(double radius)
{
    m_radius = (qreal)radius;
    m_angularAcceleration=0;
    m_angularVelocity=m_initialAngularVelocity;
    m_helper->cleanPath();
}

qreal DrawWidget::limitAngle(qreal angle)
{
    /*limitare intre 0;2pi*/
    /*
    if(angle>=2*PI)
        angle-=2*PI;
    if(angle<0)
        angle+=2*PI;
    */
    while(angle>PI)
        angle-=2*PI;

    while(angle<=-PI)
        angle+=2*PI;

    return angle;
}

void DrawWidget::enablePathDrawing(bool var)
{
    m_helper->enablePathDrawing(var);
}

void DrawWidget::setAngularVelocity(double v)
{
    m_angularVelocity = (qreal)v;
    m_initialAngularVelocity = (qreal)v;
}

void DrawWidget::enableVelocityDrawing(bool var)
{   
    m_helper->enableVelocityDrawing(var);
}

void DrawWidget::enableAccelerationDrawing(bool var)
{
    m_helper->enableAccelerationDrawing(var);
}

qreal DrawWidget::currentAngle(QPointF position)
{
    /* cazuri pe cele 4 cadrane */
    if(position.x()>0 && position.y()<=0)
    {
        //cadranul II
        return PI/2+qAtan((-1)*position.y()/position.x());
    }
    else if(position.x()>=0 && position.y()>0)
    {
        //cadranul I
        return qAtan(position.x()/position.y());
    }
    else if(position.x()<0 && position.y()>=0)
    {
        //cadranul IV
        return 3*PI/2+qAtan((-1)*position.y()/position.x());
    }
    else if(position.x()<=0 && position.y()<0)
    {
        //cadranul III
        return PI+qAtan(position.x()/position.y());
    }
    return 0;
}

qreal DrawWidget::truncate(qreal x)
{
    return ((int)(x*100))/100.0;
}

void DrawWidget::setGravitationalAcceleration(double var)
{
    m_g = var;
    m_helper->cleanPath();
}

QVector<qreal> DrawWidget::getData()
{
    return angle_data;
}

void DrawWidget::resetData()
{
    angle_data.clear();
}
