#include "PersonTableModel.h"
#include "group.h"
#include <sstream>
#include "QStringList"
#include "QMimeData"

enum ColumnType
{
    FirstName = 0,
    LastName,
    Sex,
    Age,
    Height,
    Weight,
    Ward,
    MedicalCondition,
    YouthCommittee,
    NumColumnTypes
};

PersonTableModel::PersonTableModel(std::vector<Person>& members_in, QObject* parent /*=NULL*/) :
    PersonModel(parent),
    members(members_in)
{
}

const std::vector<Person>& PersonTableModel::getMembers() const
{
    return members;
}

std::vector<Person>& PersonTableModel::getMembers()
{
    return members;
}

std::string PersonTableModel::makeTitle() const
{
    std::stringstream s;
    s << "Unassigned (" << getMembers().size() << ")";
    return s.str();
}

int PersonTableModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return getMembers().size();
}

int PersonTableModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return NumColumnTypes;
}

QVariant PersonTableModel::data(const QModelIndex & index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= getMembers().size())
        return QVariant();

    const Person& person = getMembers()[index.row()];
    switch(role)
    {
        case Qt::DisplayRole:
        {
            std::string display_txt;
            switch(index.column())
            {
                case FirstName:
                    display_txt = person.get_first_name();
                    break;
                case LastName:
                    display_txt = person.get_last_name();
                    break;
                case Sex:
                    display_txt = person.get_is_male() ? "M" : "F";
                    break;
                case Age:
                {
                    std::stringstream s;
                    s<<person.get_age();
                    display_txt = s.str();
                }
                    break;
                case Height:
                    display_txt = convert_height_to_string(person.get_height());
                    break;
                case Weight:
                {
                    std::stringstream s;
                    s<<person.get_weight()<<" lbs";
                    display_txt = s.str();
                }
                    break;
                case Ward:
                    display_txt = person.get_ward();
                    break;
                case MedicalCondition:
                    display_txt = person.get_medical_condition();
                    break;
                case YouthCommittee:
                    display_txt = person.get_on_youth_committee() ? "yes" : "" ;
                    break;
                default:
                    display_txt = "unknown";
                    break;
            }
            return QVariant(QString::fromStdString(display_txt));
            break;
        }
    }
    return QVariant();
}

QVariant PersonTableModel::headerData ( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/ ) const
{

    if (orientation != Qt::Horizontal)
        return QVariant();
    
    switch(role)
    {
        case Qt::DisplayRole:
        {
            std::string display_txt;
            switch(section)
            {
                case FirstName:
                    display_txt = "First Name";
                    break;
                case LastName:
                    display_txt = "Last Name";
                    break;
                case Sex:
                    display_txt = "Sex";
                    break;
                case Age:
                     display_txt = "Age";
                    break;
                case Height:
                    display_txt = "Height";
                    break;
                case Weight:
                    display_txt = "Weight";
                    break;
                case Ward:
                    display_txt = "Unit";
                    break;
                case MedicalCondition:
                    display_txt = "Medical";
                    break;
                case YouthCommittee:
                    display_txt = "Youth Comm.";
                    break;
                default:
                    display_txt = "unknown";
                    break;
            }
            return QVariant(QString::fromStdString(display_txt));
            break;
        }
            
    }
    return QVariant();
}

class Sorter
{
public:
    Sorter(int criteria_in, bool ascending_in) : criteria(criteria_in), ascending(ascending_in){};
    bool operator()(const Person& left, const Person& right)
    {
        const Person& a = (ascending ? left : right);
        const Person& b = (ascending ? right : left);
        bool rval = false;
        switch (criteria) {
            case FirstName:
                rval = a.get_first_name() < b.get_first_name();
                break;
            case LastName:
                rval = a.get_last_name() < b.get_last_name();
                break;
            case Sex:
                rval = a.get_is_male() < b.get_is_male();
                break;
            case Age:
                rval = a.get_age() < b.get_age();
                break;
            case Height:
                rval = a.get_height() < b.get_height();
                break;
            case Weight:
                rval = a.get_weight() < b.get_weight();
                break;
            case Ward:
                rval = a.get_ward() < b.get_ward();
                break;
            case MedicalCondition:
                rval = a.get_medical_condition() < b.get_medical_condition();
                break;
            case YouthCommittee:
                rval = a.get_on_youth_committee() < b.get_on_youth_committee();
                break;
            case NumColumnTypes:
            default:
                rval = false;
                break;

        }
        return rval;
    }
    
private:
    int criteria;
    bool ascending;
    
};

void PersonTableModel::sort(int column, Qt::SortOrder order /*= Qt::AscendingOrder*/)
{
    emit layoutAboutToBeChanged();
    
    Sorter sorter(column, order == Qt::AscendingOrder);
    std::sort(getMembers().begin(), getMembers().end(), sorter);
    
    emit layoutChanged();
}


