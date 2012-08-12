#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <vector>
#include <map>
#include "person.h"
#include "group.h"
#include "company.h"
#include "yaml-cpp/yaml.h"

class Document
{
public:
    Document();
    std::vector<Group>::iterator find_group(const std::string& name);
    
    bool add_group(const std::string& name);
    bool remove_group(const std::string& name);

    bool add_person_to_group(const std::string group_name, const Person& person);
    bool remove_person_from_group(const std::string group_name, const Person& person);

    bool add_person_to_unassigned(const Person& person);
    bool remove_person_from_unassigned(const Person& person);
    
    const std::vector<Person>& get_unassigned_people() const;
    std::vector<Person>& get_unassigned_people();
    
    const std::vector<Group>& get_groups() const;
    std::vector<Group>& get_groups();

    const std::vector<Company>& get_companies() const;
    std::vector<Company>& get_companies();

    void read_from_file(const std::string& filename);
    void write_to_file(const std::string& filename);
    
private:
    void clear();

    friend YAML::Emitter& operator << (YAML::Emitter& out, const Document& document);
    friend void operator >> (const YAML::Node& node, Document& document);
    std::vector<Person> unassigned_people;
    std::vector<Group> groups;
    std::vector<Company> companies;
};

YAML::Emitter& operator << (YAML::Emitter& out, const Document& document);
void operator >> (const YAML::Node& node, Document& document);

#endif //DOCUMENT_H
