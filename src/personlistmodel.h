#ifndef PERSONLISTMODEL_H
#define PERSONLISTMODEL_H

#include "PersonModel.h"
#include "QColor"

class Group;

class PersonListModel : public PersonModel
{
    Q_OBJECT

public:
    explicit PersonListModel(Group& group, QObject* parent = NULL);
    
    QColor getColor() const;
    void setColor(const QColor& qcolor);
    
    //overrides from PersonModel
    virtual const std::vector<Person>& getMembers() const;
    virtual std::vector<Person>& getMembers();
    virtual std::string makeTitle() const;

    //overrides from QAbstractListModel
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    
private:
    Group& group;
};

#endif //PERSONLISTMODEL_H
