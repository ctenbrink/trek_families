#include "PersonListModel.h"
#include <sstream>
#include "group.h"

PersonListModel::PersonListModel(Group& group_in, QObject* parent /*= NULL*/) :
    PersonModel(parent),
    group(group_in)
{
}

const std::vector<Person>& PersonListModel::getMembers() const
{
    return group.get_members();
}

std::vector<Person>& PersonListModel::getMembers()
{
    return group.get_members();
}

std::string PersonListModel::makeTitle() const
{
    return make_title_from_group(group);
}

int PersonListModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return getMembers().size();
}

int PersonListModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return 1;
}

QVariant PersonListModel::data(const QModelIndex & index, int role /*= Qt::DisplayRole*/) const
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
            std::stringstream s;
            s << person.get_first_name() << " " << person.get_last_name() << " (" << person.get_ward() << ")";
            if(person.get_on_youth_committee())
                s<<" *";
            return QVariant(QString::fromStdString(s.str()));
        }
            break;
            
        case Qt::ToolTipRole:
        {
            std::stringstream s;
            std::string sex = person.get_is_male() ? "M" : "F";
            s<<person.get_first_name()<<" "<<person.get_last_name()<<"\n";
            s<<"Sex: "<<sex<< ", Age: "<<person.get_age()<<", Unit: "<<person.get_ward()<<"\n";
            s<<"Ht: "<<convert_height_to_string(person.get_height())<<", Wt: "<<person.get_weight() << " lbs";
            if(person.get_on_youth_committee())
                s<<"\nYouth Committee";
            if(!person.get_medical_condition().empty())
                s<<"\n"<<person.get_medical_condition();
            
            return QVariant(QString::fromStdString(s.str()));
        }
            break;
    }
    return QVariant();
}

QColor PersonListModel::getColor() const
{
    const RGBColor color = group.get_color();
    QColor qcolor(color.get_red(), color.get_green(), color.get_blue());
    return qcolor;
}

void PersonListModel::setColor(const QColor& qcolor)
{
    RGBColor color(qcolor.red(), qcolor.green(), qcolor.blue());
    group.set_color(color);
}

