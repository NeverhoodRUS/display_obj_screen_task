#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include "polygonfiltermodel.h"
#include "appsettings.h"

/** * @brief model_ модель производящая фильтрацию объектов*/
PolygonFilterModel *model_ = nullptr;
/** * @brief polyLines_ "список" объектов*/
QVector<Polyline> polyLines_ {};
/** * @brief testFields_ "список" тестовых полей*/
QVector</*leftTop*/QPair<QPair<double, double>,/*rightBottom*/QPair<double, double>> > testFields_ {};

/**
 * @brief initModel Инициализация модели и "списка" объектов
 */
void initModel();

/**
 * @brief initPolyLines Инициализация объектов
 * @param const QString& str
 */
void initPolyLines(const QString &str);

/**
 * @brief initTestFields Инициализация данных тестовых полей
 */
void initTestFields();

/**
* @brief main
* @param argc
* @param argv
* @return int
*/
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    initModel();
    if(!model_)
        return -1;
    initTestFields();
    //Имитация смены области
    model_->zoneChanged(Constants::Zones::ZONE1);
    //Для каждого поля выполняется поиск объектов
    for(const QPair<QPair<double, double>, QPair<double, double>> &field: qAsConst(testFields_))
        model_->find_objects(field.first, field.second);

    return a.exec();
}

void initModel()
{
    //Чтение файла с координатами объектов
    QFile polyLineFile("/home/jk/polylines.txt");
    if(!polyLineFile.open(QIODevice::ReadOnly)) {
        qDebug() << "poly file not opened";
        polyLineFile.close();
        return;
    }
    const QString &str = polyLineFile.readAll();
    if(!str.isEmpty()) {
        initPolyLines(str);
        model_ = new PolygonFilterModel();
        model_->setData(polyLines_);
    }
}

void initPolyLines(const QString &str)
{
    if(str.isEmpty())
        return;
    QVector<QPair<double, double>> objectCoordinates{};
    //Плучение всего списка координат объектов [{11.123, 10.194 : 0.000, 18.269 ...}, ...]
    const QStringList &allCoordinates = str.split('\n');
    for(const QString &coordinatesObj : allCoordinates)
    {
        //Получение координат объекта [{11.123, 10.194 : 0.000, 18.269 ...}]
        const QStringList &coordinatesObjList = coordinatesObj.split(':');
        foreach (const QString &coordinateStr, coordinatesObjList)
        {
            //точка объекта
            QPair<double, double> pair;
            //Получение координаты {11.123, 10.194}
            const QStringList &coordinateList = coordinateStr.split(',');
            for(int i(0); i < coordinateList.size(); ++i)
                //всегда 2 координаты. 0 - x, 1 - y
                i % 2 ? pair.second = coordinateList.at(i).toDouble() : pair.first = coordinateList.at(i).toDouble();
            objectCoordinates.append(pair);
        }
        //генерация псевдо случайной зоны
        const Constants::Zones zone = static_cast<Constants::Zones>(qrand() % ((Constants::Zones::ZONE3 + 1)
                                        - Constants::Zones::DEAFAULT) + Constants::Zones::DEAFAULT);
        polyLines_.append(Polyline(objectCoordinates, zone));
        objectCoordinates.clear();
    }
}

void initTestFields()
{
    QFile fieldsFile("/home/jk/fields.txt");
    if(!fieldsFile.open(QIODevice::ReadOnly)) {
        qDebug() << "field file not opened";
        fieldsFile.close();
        return;
    }
    const QString &str = fieldsFile.readAll();
    //Получение всего списка координат в строковом представлении [{0.000, 100.000 : 100.000, 0.000}, ...]
    const QStringList &allCoordinates = str.split('\n');
    for(const QString &s : allCoordinates)
    {
        //Получение координат поля [{0.000, 100.000}, {100.000, 0.000}]. Всегда 2
        const QStringList &fieldData = s.split(':');
        //Получение левой верхней координаты {0.000, 100.000}. Всегда первый элемент
        const QStringList &pairsALeftTop = fieldData.at(0).split(',');
        QPair <double, double> aLeftTop { pairsALeftTop.at(0).toDouble(), pairsALeftTop.at(1).toDouble()};
        //Получение правой нижней координаты {100.000, 0.000}. Всегда второй элемент
        const QStringList &pairsARightBottom = fieldData.at(1).split(',');
        QPair <double, double> aRightBottom { pairsARightBottom.at(0).toDouble(), pairsARightBottom.at(1).toDouble()};

        testFields_.append(QPair<QPair<double, double>, QPair<double, double>> {aLeftTop, aRightBottom});
    }
}
