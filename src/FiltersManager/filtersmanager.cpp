#include "filtersmanager.h"

FiltersManager::FiltersManager(QObject *parent)
    : QObject{parent}
{

}

const QStringList FiltersManager::getFiltersNames() const
{
    QStringList names;
    for(int i=0; i<filters.size(); i++)
    {
        names.append(filters.at(i).getName());
    }
    return names;
}

QList<IFilter> *FiltersManager::getFilters()
{
    return &filters;
}

const QVector<std::function<void()>> &FiltersManager::getCurrentFiltersFunctionsList() const
{
    return currentFiltersFunctionsList;
}

void FiltersManager::updateCurrentFilterFunctionsList()
{
    currentFiltersFunctionsList.clear();    // очищаем список
    currentFiltersFunctionsList.resize(filters.size()); // обновляем размер вектора
    for(int i=0; i<filters.size(); i++)
    {
        currentFiltersFunctionsList.append(filters.at(i).getFunction());    // добавляем в список только функции
    }
}
