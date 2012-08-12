#include "company.h"

Company::Company() :
leader(),
medical(),
members(),
color()
{
}

Company::Company( const std::string& leader_in,
                  const std::string& medical_in,
                  const RGBColor& color_in) :
    leader(leader_in),
    medical(medical_in),
    members(),
    color(color_in)
{
}

const std::string& Company::get_name() const
{
    return name;
}

void Company::set_name(const std::string& name_in)
{
    name = name_in;
}

const std::string& Company::get_leader() const
{
    return leader;
}

void Company::set_leader(const std::string& leader_in)
{
    leader = leader_in;
}

const std::string& Company::get_medical() const
{
    return medical;
}

void Company::set_medical(const std::string& medical_in)
{
    medical = medical_in;
}

std::vector<std::string> Company::get_all_members() const
{
    std::vector<std::string> all_members;
    all_members.push_back(get_leader());
    all_members.push_back(get_medical());
    all_members.insert(all_members.end(), get_members().begin(), get_members().end());
    return all_members;
}

const std::vector<std::string>& Company::get_members() const
{
    return members;
}

std::vector<std::string>& Company::get_members()
{
    return members;
}

const RGBColor& Company::get_color() const
{
    return color;
}

void Company::set_color(const RGBColor& color_in)
{
    color = color_in;
}

void Company::add_member(const std::string& member)
{
    members.push_back(member);
}



YAML::Emitter& operator << (YAML::Emitter& out, const Company& company)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Company" << YAML::Value;
        out << YAML::BeginMap;
        out << YAML::Key << "name" << YAML::Value << company.get_name();
        out << YAML::Key << "color" << YAML::Value << company.get_color();
        out << YAML::Key << "leader" << YAML::Value << company.get_leader();
        out << YAML::Key << "medical" << YAML::Value << company.get_medical();
        out << YAML::Key << "members" << YAML::Value << company.get_members();
        out << YAML::EndMap;
    out << YAML::EndMap;
    return out;
}

void operator >> (const YAML::Node& node, Company& company)
{
    const YAML::Node& company_node = node["Company"];
    for( YAML::Iterator company_iter = company_node.begin();
         company_iter != company_node.end();
         ++company_iter )
    {
        std::string key;
        company_iter.first() >> key;
           
        if(key == "name")
        {
            const YAML::Node& name_node = company_iter.second();
            std::string name;
            name_node >> name;
            company.set_name(name);
        }
        else if(key == "leader")
        {
            const YAML::Node& leader_node = company_iter.second();
            std::string leader;
            leader_node >> leader;
            company.set_leader(leader);
        }
        else if(key == "medical")
        {
            const YAML::Node& medical_node = company_iter.second();
            std::string medical;
            medical_node >> medical;
            company.set_medical(medical);
        }
        else if(key == "color")
        {
            const YAML::Node& color_node = company_iter.second();
            RGBColor color;
            color_node >> color;
            company.set_color(color);
        }
        else if(key == "members")
        {
           
            const YAML::Node& members_node = company_iter.second();
            std::vector<std::string>& members = company.get_members();
            members_node >> members;
        }
        else
        {
            assert(!"Unhandled parameter when reading Group data from YAML");
        }
    }
}

