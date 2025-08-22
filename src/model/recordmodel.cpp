#include "model/recordmodel.h"
#include <QDir>
#include <QDateTime>
#include <QFile>

RecordModel::RecordModel(QObject* parent): QAbstractListModel(parent)
{
}

int RecordModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : mFiles.size();
}

QVariant RecordModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= mFiles.size())
        return {};

    const QFileInfo& fi = mFiles.at(index.row());
    switch (role) {
    case FileNameRole:     return fi.fileName();
    case FilePathRole:     return fi.absoluteFilePath();
    case FileSizeRole:     return QVariant::fromValue<qulonglong>(fi.size());
    case FileModifiedRole: return fi.lastModified();
    default:               return {};
    }
}

QHash<int, QByteArray> RecordModel::roleNames() const
{
    return {
        { FileNameRole,     "fileName"     },
        { FilePathRole,     "filePath"     },
        { FileSizeRole,     "fileSize"     },
        { FileModifiedRole, "fileModified" }
    };
}

void RecordModel::setFolder(const QString& f)
{
    if (mFolder == f) return;
    mFolder = f;
    emit folderChanged();
    refresh();
}

void RecordModel::refresh()
{
    loadSync();
}

void RecordModel::loadSync()
{
    beginResetModel();
    QDir dir(mFolder.isEmpty() ? "." : mFolder);
    dir.setNameFilters({ "*.mp4", "*.mkv", "*.mov" });
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Time);
    mFiles = dir.entryInfoList();
    endResetModel();
}

void RecordModel::removeAt(int row)
{
    if (row < 0 || row >= mFiles.size()) return;
    const QString path = mFiles.at(row).absoluteFilePath();
    if (QFile::remove(path)) {
        beginRemoveRows(QModelIndex(), row, row);
        mFiles.removeAt(row);
        endRemoveRows();
    }
}
