
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

#include "Helper.h"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QtCore/qmath.h>
#include <QDebug>
#include <QtAlgorithms>
#include <QRectF>

Helper::Helper(const QSize canvasSize):
    canvasSize(canvasSize),
    m_isPathDrawingEnabled(true),
    m_isVelocityDrawingEnabled(false),
    m_isAccelerationDrawingEnabled(false)
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    background = QBrush(QColor(252, 255, 255, 255));
    circleBrush = QBrush(QColor(122, 204, 0, 255));
    circlePen = QPen(Qt::black);
    circlePen.setWidth(2);
    smallPen = QPen(Qt::black);
    smallPen.setWidth(1);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);

    pathPen = QPen(QColor(204,0,100,255));
    pathPen.setWidth(3);

    velocityPen = QPen(Qt::blue);
    velocityPen.setWidth(2);

    accelerationPen = QPen(Qt::red);
    accelerationPen.setWidth(2);
}

void Helper::paint(QPainter *painter, QPaintEvent *event,
                   int drawnRadius, QPointF initialPosition, QPointF position,
                   QPointF velocity, QPointF acceleration)
{

    Q_UNUSED(initialPosition);
    if(m_isPathDrawingEnabled && !m_path.contains(position))
    {
        m_path.append(position);
    }

    painter->fillRect(event->rect(), background);
    painter->translate((qreal)canvasSize.width()/4+5, (qreal)canvasSize.height()/2-25);

    painter->setPen(circlePen);
    QPoint center(0,0);

    /* circle */
    painter->drawEllipse(center, drawnRadius+10, drawnRadius+10);
    /* wire */
    painter->setPen(smallPen);
    painter->drawLine(center, position);

    /* path drawing */
    if(m_isPathDrawingEnabled)
    {
        painter->setPen(pathPen);
        painter->drawPoints(m_path.data(),m_path.size());
    }

    if(m_isVelocityDrawingEnabled)
    {
        painter->setPen(velocityPen);
        painter->drawLine(position,velocity);
    }

    if(m_isAccelerationDrawingEnabled)
    {
        painter->setPen(accelerationPen);
        painter->drawLine(position, acceleration);
    }

    painter->setPen(smallPen);
    painter->setBrush(circleBrush);
    painter->drawEllipse(position, 10, 10);

}

void Helper::cleanPath()
{
    m_path.clear();
}

void Helper::enablePathDrawing(bool var)
{
    m_isPathDrawingEnabled = var;
}

void Helper::enableVelocityDrawing(bool var)
{
    m_isVelocityDrawingEnabled = var;
}

void Helper::enableAccelerationDrawing(bool var)
{
    m_isAccelerationDrawingEnabled = var;
}

bool Helper::isAccelerationDrawingEnabled()
{
    return m_isAccelerationDrawingEnabled;
}
