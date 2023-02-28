#ifndef POLYGONFILTERMODEL_H
#define POLYGONFILTERMODEL_H
#include "polyline.h"
#include "appsettings.h"

/**
 * @brief The PolygonModel class. Класс для работы с фильтрацией объектов на поле
 */
class PolygonFilterModel
{
public:
    explicit PolygonFilterModel();

    void setData(const QVector<Polyline> &pl)   { _data = pl; }

    //поиск по прямоугольной области экрана
    QVector<Polyline> find_objects(QPair<double, double> aLeftTop, QPair<double, double> aRightBottom);
    /**
     * @brief zoneChanged Действия при изменении зоны: фильтрация объектов по зоне,
     * вычисление размера оптимального пулла для отправки объектов в 1 поток (кол-во объектов/кол-во потоков)
     * @param const Constants::Zones zone
     */
    void zoneChanged(const Constants::Zones zone);

private:
    //общий список объектов
    QVector<Polyline> _data;
    //Список объектов отфильтрованных по зоне
    QVector< Polyline* > _dataFilteredByZone;

    /*Параметры для вычисления оптимального пулла объектов обрабатываемых в 1 потоке*/
    int _threadIdealCount;  //QThread::idealThreadCount()
    int _pullSize;          //размер пулла для отправки в 1 поток
    int _filteredSize;      //кол-во отфильтрованных по зоне объектов
};

#endif // POLYGONFILTERMODEL_H
