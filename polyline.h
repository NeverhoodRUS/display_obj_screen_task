#ifndef POLYLINE_H
#define POLYLINE_H
#include <QVector>
#include "appsettings.h"

/**
 * @brief The Polyline class. Класс описывающий сущность объект-линия
 */
class Polyline {

public:
    explicit Polyline(const QVector<QPair<double, double>> &line, const Constants::Zones zone);

    inline void set_points(const QVector<QPair<double, double>> aPoints)    { _points = aPoints; }
    inline const QVector<QPair<double, double>>& get_points()               { return _points; }
    inline uint get_area() const                                            { return _area; }
    /**
     * @brief possibleView Метод вычисляющий находится ли данный объект в указанных границах
     * @param QPair<double, double> aLeftTop левая верхняя координата
     * @param QPair<double, double> aRightBottom правая нижняя координата
     * @return bool
     */
    inline bool possibleView(QPair<double, double> aLeftTop, QPair<double, double> aRightBottom) const;

private:
    QVector<QPair<double, double>> _points;
    /** * @brief _xLeftPoint крайняя левая координата X */
    double _xLeftPoint;
    /** * @brief _xRightPoint крайняя правая координата X */
    double _xRightPoint;
    /** * @brief _yBottomPoint нижняя координата Y */
    double _yBottomPoint;
    /** * @brief _yTopPoint верхняя координата Y */
    double _yTopPoint;
    /** * @brief _area идентификатор зоны, к которой объект принадлежит */
    uint _area;

    /**
     * @brief initExtrimePoints Метод вычисляющий крайние точки объекта
     */
    void initExtrimePoints();
};

inline bool Polyline::possibleView(QPair<double/*X*/, double/*Y*/> aLeftTop, QPair<double/*X*/, double/*Y*/> aRightBottom) const
{
    //Если крайняя левая Х, или крайняя правая Х, или крайняя верхняя Y, или крайняя нижняя Y объекта в границах поля
    return (_xLeftPoint > aLeftTop.first && _xRightPoint < aRightBottom.first)
            || (_yBottomPoint > aRightBottom.second && _yTopPoint < aRightBottom.second);
}

#endif // POLYLINE_H
