#include "PersonModel.h"
#include <QStringList>
#include <QMimeData>
#include <sstream>

const std::string kYamlMimeType("text.yaml");

std::string convert_height_to_string(double inches)
{
    int int_inches = static_cast<int>(inches); //todo: use boost::numeric_cast
    int feet = int_inches / 12;
    int remainder_inches = int_inches % 12;
    std::stringstream s;
    s<< feet << "\' "<< remainder_inches <<"\"";
    return s.str();
    
}

PersonModel::PersonModel(QObject* parent /*= NULL*/) :
    QAbstractItemModel(parent){
}

QModelIndex PersonModel::index(int row, 
                               int column,
                               const QModelIndex &parent /*= QModelIndex()*/) const
{
    return createIndex(row, column);
}

QModelIndex PersonModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

bool PersonModel::setData(const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole*/)
{
    if (index.isValid() && index.row() < getMembers().size() && role == Qt::EditRole) {
        
        Person person = Person::from_yaml( value.toString().toStdString() );
        getMembers()[index.row()] = person;
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::DropActions PersonModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::ItemFlags PersonModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList PersonModel::mimeTypes() const
{
    QStringList types;
    types << kYamlMimeType.c_str();
    return types;
}

QMimeData* PersonModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mimeData = new QMimeData();
    QByteArray encodedData;
    
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    
    foreach (const QModelIndex &index, indexes) {
        if (index.isValid() && index.row() < getMembers().size() && index.column() == 0) {
            const Person& person = getMembers()[index.row()];
            QString text = QString::fromStdString(person.to_yaml());
            stream << text;
        }
    }
    
    mimeData->setData(kYamlMimeType.c_str(), encodedData);
    return mimeData;
}

bool PersonModel::dropMimeData(const QMimeData *data,
                                   Qt::DropAction action, 
                                   int row, 
                                   int column, 
                                   const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;
    
    if (!data->hasFormat(kYamlMimeType.c_str()))
        return false;
    
    int beginRow = rowCount(QModelIndex());
    
    if (row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = parent.row();
    
    QByteArray encodedData = data->data(kYamlMimeType.c_str());
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    std::vector<QString> yaml_vector;
    
    while (!stream.atEnd()) {
        QString text;
        stream >> text;
        yaml_vector.push_back(text);
    }
    
    insertRows(beginRow, yaml_vector.size(), QModelIndex());
    for(std::vector<QString>::const_iterator iter = yaml_vector.begin(); iter != yaml_vector.end(); ++iter) {
        QModelIndex idx = index(beginRow, 0, QModelIndex());
        setData(idx, QVariant(*iter), Qt::EditRole);
        beginRow++;
    }
    
    return true;
}

bool PersonModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    if(position > getMembers().size())
        return false;
    
    beginInsertRows(QModelIndex(), position, position+rows-1);
    
    std::vector<Person>::iterator position_iter = getMembers().begin() + position;
    getMembers().insert(position_iter, rows, Person());
    
    endInsertRows();
    emit title_changed(makeTitle());
    return true;
}

bool PersonModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    if(position + rows > getMembers().size())
        return false;
    
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    
    std::vector<Person>::iterator start_iter = getMembers().begin() + position;
    std::vector<Person>::iterator end_iter = getMembers().begin() + position + rows;
    
    getMembers().erase(start_iter, end_iter);
    
    endRemoveRows();
    emit title_changed(makeTitle());
    return true;
}



