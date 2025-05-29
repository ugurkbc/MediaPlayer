#include "utils.h"
#include <QUrl>
#include <mutex>

Utils * Utils::getInstance()
{
    static std::mutex mutex;
    static Utils *instance = nullptr;
    std::lock_guard<std::mutex> lock(mutex);

    if(instance == nullptr){
        instance = new Utils();
    }

    return instance;
}

Utils::Utils()
{

}

QString Utils::fileDialogConverter(const QString &url)
{
    QUrl qUrl(url);
    QString filePath = qUrl.toLocalFile();
    return filePath;
}
