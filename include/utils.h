#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QObject>

class Utils : public QObject
{
    Q_OBJECT

public:
    static Utils * getInstance();

    Utils(const Utils&) = delete;
    Utils& operator=(const Utils&) = delete;

    Q_INVOKABLE  QString fileDialogConverter(const QString &url);
    Q_INVOKABLE  QString generateFileName();

private:
    Utils();
};

#endif // UTILS_H
