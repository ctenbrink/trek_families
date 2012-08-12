#ifndef PERSONTABLEMODEL_H
#define PERSONTABLEMODEL_H

#include "PersonModel.h"

class PersonTableModel : public PersonModel
{
    Q_OBJECT

public:
    explicit PersonTableModel(std::vector<Person>& members_in, QObject* = NULL);
    
    //overrides from PersonModel
    virtual const std::vector<Person>& getMembers() const;
    virtual std::vector<Person>& getMembers();
    virtual std::string makeTitle() const;
    
    //overrides from QAbstractItemModel
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    
private:
    std::vector<Person>& members;
};

#endif //PERSONTABLEMODEL_H
