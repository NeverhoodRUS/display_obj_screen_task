#include "polyline.h"

Polyline::Polyline(const QVector<QPair<double, double>> &line, const Constants::Zones zone) :
    _points(line),
    _area(zone)
{
    //При инициализации объекта необходимо заранее найти крайние точки для более лёгкой обработки в дальнейшем.
    initExtrimePoints();
}

void Polyline::initExtrimePoints()
{
    if(_points.isEmpty())
        return;
    _xLeftPoint     = _points.at(0).first;
    _xRightPoint    = _points.at(0).first;
    _yBottomPoint   = _points.at(0).second;
    _yTopPoint      = _points.at(0).second;

    //Поиск крайних точек объекта
    for(const QPair<double, double> &pair : qAsConst(_points)) {
        pair.first < _xLeftPoint?       : _xLeftPoint   = pair.first;
        pair.first > _xRightPoint?      : _xRightPoint  = pair.first;
        pair.second > _yTopPoint?       : _yTopPoint    = pair.second;
        pair.second < _yBottomPoint?    : _yBottomPoint = pair.second;
    }
}
