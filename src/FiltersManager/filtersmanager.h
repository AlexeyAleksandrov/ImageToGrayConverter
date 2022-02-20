#ifndef FILTERSMANAGER_H
#define FILTERSMANAGER_H

#include <QObject>
#include "ifilter.h"

class FiltersManager : public QObject
{
    Q_OBJECT
public:
    explicit FiltersManager(QObject *parent = nullptr);

    const QStringList getFiltersNames() const;  // получить названия фильтров
    QList<IFilter> *getFilters();

    const QVector<std::function<void()>> &getCurrentFiltersFunctionsList() const; // получить список функций, для применения фильтра

    void updateCurrentFilterFunctionsList();    // пересобрать список функций, для применения текущего набора фильтров

signals:

private:
    QList<IFilter> filters;   // список фильтров, которые надо применить
    QVector<std::function<void()>> currentFiltersFunctionsList;   // список функций, применяемых для текукщего набора фильтров

};

#endif // FILTERSMANAGER_H
