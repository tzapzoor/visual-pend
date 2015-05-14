
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

#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>

#include <QWidget>
#include <Qpoint>
#include <QVector>
#include <QPointF>

class Helper
{
public:
    Helper(const QSize canvasSize);

public:
    void paint(QPainter *painter, QPaintEvent *event, int drawnRadius,
               QPointF initialPosition ,QPointF position, QPointF velocity,
               QPointF acceleration);
    void cleanPath();

    void enablePathDrawing(bool var);
    void enableVelocityDrawing(bool var);
    void enableAccelerationDrawing(bool var);

    bool isAccelerationDrawingEnabled();

private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen smallPen;
    QPen textPen;
    QPen pathPen;
    QPen velocityPen;
    QPen accelerationPen;
    QSize canvasSize;
    QPointF position;
    QVector<QPointF> m_path;

    bool m_isPathDrawingEnabled;
    bool m_isVelocityDrawingEnabled;
    bool m_isAccelerationDrawingEnabled;

};

#endif // HELPER_H
