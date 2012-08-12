#ifndef PERSONMODEL_H
#define PERSONMODEL_H

#include <QAbstractItemModel>
#include "person.h"
#include <vector>

class PersonModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit PersonModel(QObject* parent = NULL);
    
    virtual const std::vector<Person>& getMembers() const = 0;
    virtual std::vector<Person>& getMembers() = 0;
    virtual std::string makeTitle() const = 0;

    //overrides from QAbstractTableModel
    virtual QModelIndex index(int row, 
                              int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual Qt::DropActions supportedDropActions() const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QStringList mimeTypes() const;
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
    virtual bool dropMimeData(const QMimeData *data, 
                              Qt::DropAction action,
                              int row, 
                              int column, 
                              const QModelIndex &parent);
    virtual bool insertRows(int position, int rows, const QModelIndex &parent);
    virtual bool removeRows(int position, int rows, const QModelIndex &parent);
    
signals:
    void title_changed(const std::string& new_title);
};

std::string convert_height_to_string(double inches);

#endif //PERSONMODEL_H
