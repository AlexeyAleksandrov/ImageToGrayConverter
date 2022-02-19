#ifndef FILTER_H
#define FILTER_H

#include <QObject>

class Filter : public QObject
{
    Q_OBJECT
public:
    explicit Filter(QObject *parent = nullptr);

signals:

private:
    QString name;   // название фильтра
    std::function<void()> function; // функция, которую выполнит фильтр

public:
    const QString &getName() const;
    void setName(const QString &newName);
    const std::function<void()> &getFunction() const;
    void setFunction(const std::function<void()> &newFunction);
};

#endif // FILTER_H
