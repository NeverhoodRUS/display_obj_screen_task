#include "polygonfiltermodel.h"
#include <QThread>
#include <QFutureSynchronizer>
#include <QtConcurrent/QtConcurrent>

PolygonFilterModel::PolygonFilterModel() :
    _threadIdealCount(QThread::idealThreadCount()),
    _pullSize(0),
    _filteredSize(0)
{
}

QVector<Polyline> PolygonFilterModel::find_objects(QPair<double, double> aLeftTop, QPair<double, double> aRightBottom)
{
    //Сложность алгоритма O(n)
    QFutureSynchronizer<void> *syncWatcher = new QFutureSynchronizer<void>;
    QVector<Polyline> result;
    QVector<Polyline*> vecForThread;

    for(int i(1); i <= _threadIdealCount; ++i)
    {
        const int step = (_pullSize/_threadIdealCount) * i - _pullSize/_threadIdealCount;
        vecForThread = _dataFilteredByZone.mid(step, _pullSize/_threadIdealCount - 1);
        syncWatcher->addFuture(QtConcurrent::run([&vecForThread, &aLeftTop, &aRightBottom, &result]()
        {
            for(int j(0); j < vecForThread.size(); ++j)
            {
                if(vecForThread[j]->possibleView(aLeftTop, aRightBottom))
                    result.append(*vecForThread[j]);
            }
        }));
        vecForThread.clear();
    }

    syncWatcher->waitForFinished();
    return result;
}

void PolygonFilterModel::zoneChanged(const Constants::Zones zone)
{
    for(Polyline &pl : _data)
    {
        if(pl.get_area() == zone)
            _dataFilteredByZone.append(&pl);
    }
    _filteredSize = _dataFilteredByZone.size();
    //вычисление оптимального пулла объектов обрабатываемых в 1 потоке
    _pullSize = _filteredSize % _threadIdealCount ? _filteredSize/_threadIdealCount + 1 : _filteredSize % _threadIdealCount;
}
