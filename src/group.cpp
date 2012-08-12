
#include "group.h"
#include "yaml-cpp/yaml.h"
#include <sstream>

Group::Group() : 
    name(),
    unit(),
    has_medical_training(false),
    members(),
    color()
{
}

Group::Group(const std::string& name_in) : 
    name(name_in),
    unit(),
    has_medical_training(false),
    members(),
    color()
{
}

const std::string& Group::get_name() const
{
    return name;
}

void Group::set_name(const std::string& name_in)
{
    name = name_in;
}
const std::string& Group::get_unit() const
{
    return unit;
}

void Group::set_unit(const std::string& unit_in)
{
    unit = unit_in;
}

bool Group::get_has_medical_training() const
{
    return has_medical_training;
}

void Group::set_has_medical_training(bool has_medical_training_in)
{
    has_medical_training = has_medical_training_in;
}

const RGBColor& Group::get_color() const
{
    return color;
}

void Group::set_color(const RGBColor &color_in)
{
    color = color_in;
}

const std::vector<Person>& Group::get_members() const
{
    return members;
}

std::vector<Person>& Group::get_members()
{
    return members;
}

YAML::Emitter& operator << (YAML::Emitter& out, const Group& group)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Group" << YAML::Value;
        out << YAML::BeginMap;
        out << YAML::Key << "name" << YAML::Value << group.get_name();
        out << YAML::Key << "color" << YAML::Value << group.get_color();
        out << YAML::Key << "unit" << YAML::Value << group.get_unit();
        out << YAML::Key << "has_medical_training" << YAML::Value << group.get_has_medical_training();
        out << YAML::Key << "members" << YAML::Value << group.get_members();
        out << YAML::EndMap;
    out << YAML::EndMap;
    return out;
}

void operator >> (const YAML::Node& node, Group& group)
{
    const YAML::Node& group_node = node["Group"];
    for( YAML::Iterator group_iter = group_node.begin();
         group_iter != group_node.end();
         ++group_iter )
    {
        std::string key;
        group_iter.first() >> key;
           
        if(key == "name")
        {
            const YAML::Node& name_node = group_iter.second();
            std::string name;
            name_node >> name;
            group.set_name(name);
        }
        else if(key == "unit")
        {
            const YAML::Node& unit_node = group_iter.second();
            std::string unit;
            unit_node >> unit;
            group.set_unit(unit);
        }
        else if(key == "has_medical_training")
        {
            const YAML::Node& has_training_node = group_iter.second();
            bool has_training;
            has_training_node >> has_training;
            group.set_has_medical_training(has_training);
        }
        else if(key == "color")
        {
            const YAML::Node& color_node = group_iter.second();
            RGBColor color;
            color_node >> color;
            group.set_color(color);
        }
        else if(key == "members")
        {
           
            const YAML::Node& members_node = group_iter.second();
            std::vector<Person>& members = group.get_members();
            members_node >> members;
        }
        else
        {
            assert(!"Unhandled parameter when reading Group data from YAML");
        }
    }
}

std::string make_title_from_group(const Group& group)
{
 
    size_t num_girls = 0;
    size_t num_boys = 0;
    std::vector<Person>::const_iterator iter;
    for(iter = group.get_members().begin(); iter != group.get_members().end(); ++iter){
        if(iter->get_is_male())
            ++num_boys;
        else
            ++num_girls;
        
    }
    
    std::stringstream s;
    s<< group.get_name() << ":" << num_girls << "," << num_boys << " " << group.get_unit();
    if(group.get_has_medical_training())
        s << " M";
    return s.str();
}


