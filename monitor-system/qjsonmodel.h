#ifndef QJSONMODEL_H
#define QJSONMODEL_H

#include <QAbstractItemModel>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QIcon>

class QJsonModel;
class QJsonItem;

class QJsonTreeItem
{
public:
    QJsonTreeItem(QJsonTreeItem * parent = 0);
    ~QJsonTreeItem();
    void appendChild(QJsonTreeItem * item);
    QJsonTreeItem *child(int row);
    QJsonTreeItem *parent();
    int childCount() const;
    int row() const;
    void setKey(const QString& key);
    void setValue(const QString& value);
    void setType(const QJsonValue::Type& type);
    QString key() const;
    QString value() const;
    QJsonValue::Type type() const;


    static QJsonTreeItem* load(const QJsonValue& value, QJsonTreeItem * parent = 0);

private:
    QString mKey;
    QString mValue;
    QJsonValue::Type mType;
    QList<QJsonTreeItem*> mChilds;
    QJsonTreeItem * mParent;


};

//---------------------------------------------------

class QJsonModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QJsonModel(QObject *parent = 0);
    bool load(const QString& fileName);
    bool load(QIODevice * device);
    bool loadJson(const QByteArray& json);
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

private:
    QJsonTreeItem * mRootItem;
    QJsonDocument mDocument;
    QStringList mHeaders;


};

#endif // QJSONMODEL_H
