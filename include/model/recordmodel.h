#ifndef RECORDMODEL_H
#define RECORDMODEL_H

#include <QAbstractListModel>
#include <QFileInfoList>
#include <QObject>

class RecordModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString folder READ folder WRITE setFolder NOTIFY folderChanged)

public:
    enum Roles {
        FileNameRole = Qt::UserRole + 1,
        FilePathRole,
        FileSizeRole,
        FileModifiedRole
    };
    Q_ENUM(Roles)

    explicit RecordModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString folder() const { return mFolder; }
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void removeAt(int row);

public slots:
    void setFolder(const QString& f);

signals:
    void folderChanged();

private:
    QString mFolder;
    QFileInfoList mFiles;
    void loadSync();
};

#endif // RECORDMODEL_H
