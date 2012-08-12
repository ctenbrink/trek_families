
#include "document.h"
#include "yaml-cpp/yaml.h"
#include <utility>
#include <fstream>
#include <algorithm>
#include "group.h"
#include "company.h"

Document::Document() :
    unassigned_people(),
    groups(),
    companies()
{
}

class FindGroupPredicate
{
public:
    explicit FindGroupPredicate(const std::string& group_to_find_name_in) : group_to_find_name(group_to_find_name_in){}
    bool operator()(const Group& test_group)
    {
        return test_group.get_name() == group_to_find_name;
    }
private:
    std::string group_to_find_name;
};

std::vector<Group>::iterator Document::find_group(const std::string& name)
{
    FindGroupPredicate pred(name);
    return std::find_if(groups.begin(), groups.end(), pred);
}

bool Document::add_group(const std::string& group_name)
{

    std::vector<Group>::const_iterator iter = find_group(group_name);
    if(iter != groups.end())
        return false;
    
    groups.push_back(Group(group_name));
    return true;
}

bool Document::remove_group(const std::string& group_name)
{
    std::vector<Group>::iterator iter = find_group(group_name);
    if(iter == groups.end())
        return false;
    
    groups.erase(iter, groups.end());
    return true;
}

namespace
{
    bool add_person_to_vector(std::vector<Person>& the_vector, const Person& person)
    {
        the_vector.push_back(person);
        return true;
    }
   
    bool remove_person_from_vector(std::vector<Person>& the_vector, const Person& person)
    {
        the_vector.erase(std::remove(the_vector.begin(), the_vector.end(), person), the_vector.end());
        return true;
    }
}

bool Document::add_person_to_group(const std::string group_name, const Person& person)
{
    std::vector<Group>::iterator iter = find_group(group_name);
    if(iter == groups.end())
        return false;
    return add_person_to_vector(iter->get_members(), person);
}

bool Document::remove_person_from_group(const std::string group_name, const Person& person)
{
    std::vector<Group>::iterator iter = find_group(group_name);
    if(iter == groups.end())
        return false;
    return remove_person_from_vector(iter->get_members(), person);
}

bool Document::add_person_to_unassigned(const Person& person)
{
    return add_person_to_vector(unassigned_people, person);
}

bool Document::remove_person_from_unassigned(const Person& person)
{
    return remove_person_from_vector(unassigned_people, person);
}

const std::vector<Person>& Document::get_unassigned_people() const
{
    return unassigned_people;
}

std::vector<Person>& Document::get_unassigned_people()
{
    return unassigned_people;
}

const std::vector<Group>& Document::get_groups() const
{
    return groups;
}

std::vector<Group>& Document::get_groups()
{
    return groups;
}

const std::vector<Company>& Document::get_companies() const
{
    return companies;
}

std::vector<Company>& Document::get_companies()
{
    return companies;
}

void Document::clear()
{
    unassigned_people.clear();
    groups.clear();
    companies.clear();
}

void Document::write_to_file(const std::string& filename)
{
    YAML::Emitter emitter;
    emitter << *this;

    std::ofstream fout(filename.c_str());
    assert(fout.is_open());
    if(fout.is_open())
        fout << emitter.c_str();
}

void Document::read_from_file(const std::string& filename)
{
    std::ifstream fin(filename.c_str());
    assert(fin.is_open());
    if(fin.is_open())
    {
        clear();

        YAML::Parser parser(fin);
        YAML::Node node;
        parser.GetNextDocument(node);

        node >> *this;
    }
}

YAML::Emitter& operator << (YAML::Emitter& out, const Document& document)
{
    out << YAML::BeginMap;
      out << YAML::Key << "Document";
      out << YAML::Value;
      out << YAML::BeginMap;
        out << YAML::Key << "unassigned_people";
        out << YAML::Value << document.get_unassigned_people();    
        out << YAML::Key << "groups"; 
        out << YAML::Value << document.get_groups();
        out << YAML::Key << "companies"; 
        out << YAML::Value << document.get_companies();
      out << YAML::EndMap;
    out << YAML::EndMap;
    return out;
}

void operator >> (const YAML::Node& node, Document& document)
{
    const YAML::Node& document_node = node["Document"];
    for( YAML::Iterator document_iter = document_node.begin();
                        document_iter != document_node.end();
                        ++document_iter )
    {
        std::string key;
        document_iter.first() >> key;

        if(key == "unassigned_people")
        {
            const YAML::Node& unassigned_node = document_iter.second();
            for(YAML::Iterator unassigned_iter = unassigned_node.begin();
                               unassigned_iter != unassigned_node.end();
                               ++unassigned_iter)
            {   
                Person person;
                *unassigned_iter >> person;
                document.add_person_to_unassigned(person);
            }
        }
        else if(key == "groups")
        {
            const YAML::Node& groups_node = document_iter.second();
            for(YAML::Iterator groups_iter = groups_node.begin();
                               groups_iter != groups_node.end();
                               ++groups_iter)
            {   
                Group group;
                *groups_iter >> group;
                document.get_groups().push_back(group);
            }
        }
        else if(key == "companies")
        {
            const YAML::Node& companies_node = document_iter.second();
            for(YAML::Iterator companies_iter = companies_node.begin();
                               companies_iter != companies_node.end();
                               ++companies_iter)
            {   
                Company company;
                *companies_iter >> company;
                document.get_companies().push_back(company);
            }
        }
        else
        {
            assert(!"Unhandled parameter when reading Document data from YAML");
        }
    }
}
