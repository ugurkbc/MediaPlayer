#include "utils.h"
#include <QUrl>
#include <mutex>
#include <QDir>
#include <QStandardPaths>

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

QString Utils::recordsFolderPath() {
    QString base = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    if (base.isEmpty())
        base = ".";

    QDir dir(base);
    if (!dir.exists("records"))
        dir.mkpath("records");

    return dir.filePath("records");
}

QString Utils::generateFileName()
{
    QString videosDir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    if (videosDir.isEmpty())
        videosDir = "records";

    QDir dir(videosDir);
    if (!dir.exists("records"))
        dir.mkpath("records");

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");

    return dir.filePath("records/" + timestamp + ".mp4");
}

QString Utils::fileDialogConverter(const QString &url)
{
    QUrl qUrl(url);
    QString filePath = qUrl.toLocalFile();
    return filePath;
}
