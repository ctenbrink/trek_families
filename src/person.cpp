#include "person.h"
#include "yaml-cpp/yaml.h"

Person::Person():
first_name(),
last_name(),
age(0),
height(0.0),
weight(0.0),
ward(),
on_youth_committee(false),
medical_condition()
{
}

Person::Person( const std::string& first_name_in,
                const std::string& last_name_in,
                bool is_male_in,
                unsigned int age_in,
                double height_in,
                double weight_in,
                const std::string& ward_in,
                bool on_youth_committee_in,
                const std::string& medical_condition_in):
    first_name(first_name_in),
    last_name(last_name_in),
    is_male(is_male_in),
    age(age_in),
    height(height_in),
    weight(weight_in),
    ward(ward_in),
    on_youth_committee(on_youth_committee_in),
    medical_condition(medical_condition_in)
{
}

const std::string& Person::get_first_name() const
{
    return first_name;
}

void Person::set_first_name(const std::string& first_name_in)
{
    first_name = first_name_in;
}

const std::string& Person::get_last_name() const
{
    return last_name;
}

void Person::set_last_name(const std::string& last_name_in)
{
    last_name = last_name_in;
}

bool Person::get_is_male() const
{
    return is_male;
}

void Person::set_is_male(bool is_male_in)
{
    is_male = is_male_in;
}

unsigned int Person::get_age() const
{
    return age;
}

void Person::set_age(unsigned int age_in)
{
    age = age_in;
}

double Person::get_height() const
{
    return height;
}

void Person::set_height(double height_in)
{
    height = height_in;
}

double Person::get_weight() const
{
    return weight;
}

void Person::set_weight(double weight_in)
{
    weight = weight_in;
}

const std::string& Person::get_ward() const
{
    return ward;
}

void Person::set_ward(const std::string& ward_in)
{
    ward = ward_in;
}

bool Person::get_on_youth_committee() const
{
    return on_youth_committee;
}

void Person::set_on_youth_committee( bool on_youth_committee_in)
{
    on_youth_committee = on_youth_committee_in;
}

const std::string& Person::get_medical_condition() const
{
    return medical_condition;
}

void Person::set_medical_condition( const std::string& medical_condition_in)
{
    medical_condition = medical_condition_in;
}

bool Person::operator==(const Person& other) const
{
    return( !(*this < other) && !(other < *this));
}

bool Person::operator<(const Person& other) const
{
    
    if(get_last_name().compare(other.get_last_name()) < 0)
        return true;
    if(get_first_name().compare(other.get_first_name()) < 0)
        return true;
    if(get_age() < other.get_age())
        return true;
    if(get_height()<other.get_height())
        return true;
    if(get_weight()<other.get_weight())
        return true;
    if(get_ward().compare(other.get_ward()) < 0)
        return true;
    if(get_is_male() < other.get_is_male())
        return true;
    return false;
}

std::string Person::to_yaml() const
{
    YAML::Emitter yaml_emitter;
    yaml_emitter << *this;
    return std::string(yaml_emitter.c_str());
}

Person Person::from_yaml(const std::string& yaml)
{
    std::stringstream s;
    s << yaml;
    
    YAML::Parser parser(s);
    YAML::Node node;
    parser.GetNextDocument(node);
    
    Person person;
    node >> person;
    
    return person;
}

YAML::Emitter& operator << (YAML::Emitter& out, const Person& person)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Person" << YAML::Value;
        out << YAML::Flow;
        out << YAML::BeginMap;
        out << YAML::Key << "first_name" << YAML::Value << person.get_first_name();
        out << YAML::Key << "last_name" << YAML::Value << person.get_last_name();
        out << YAML::Key << "age" << YAML::Value << person.get_age();
        out << YAML::Key << "height" << YAML::Value << person.get_height();
        out << YAML::Key << "weight" << YAML::Value << person.get_weight();
        out << YAML::Key << "ward" << YAML::Value << person.get_ward();
        out << YAML::Key << "on_youth_committee" << YAML::Value << person.get_on_youth_committee();
        out << YAML::Key << "medical_condition" << YAML::Value << person.get_medical_condition();
        out << YAML::Key << "is_male" << YAML::Value << person.get_is_male();
        out << YAML::EndMap;
    out << YAML::EndMap;
    return out;
}

void operator >> (const YAML::Node& node, Person& person)
{
    const YAML::Node& person_node = node["Person"];
    for( YAML::Iterator person_iter = person_node.begin();
         person_iter != person_node.end();
         ++person_iter )
    {
        std::string key;
        person_iter.first() >> key;
           
        if(key == "first_name")
        {
            const YAML::Node& first_name_node = person_iter.second();
            std::string first_name;
            first_name_node >> first_name;
            person.set_first_name(first_name);
        }
        else if(key == "last_name")
        {
            const YAML::Node& last_name_node = person_iter.second();
            std::string last_name;
            last_name_node >> last_name;
            person.set_last_name(last_name);
        }
        else if(key == "age")
        {
            const YAML::Node& age_node = person_iter.second();
            unsigned int age;
            age_node >> age;
            person.set_age(age);
        }
        else if(key == "height")
        {
            const YAML::Node& height_node = person_iter.second();
            double height;
            height_node >> height;
            person.set_height(height);
        }
        else if(key == "weight")
        {
            const YAML::Node& weight_node = person_iter.second();
            double weight;
            weight_node >> weight;
            person.set_weight(weight);
        }
        else if(key == "ward")
        {
            const YAML::Node& ward_node = person_iter.second();
            std::string ward;
            ward_node >> ward;
            person.set_ward(ward);
        }
        else if(key == "on_youth_committee")
        {
            const YAML::Node& on_youth_committee_node = person_iter.second();
            bool on_youth_committee;
            on_youth_committee_node >> on_youth_committee;
            person.set_on_youth_committee(on_youth_committee);
        }
        else if(key == "medical_condition")
        {
            const YAML::Node& medical_condition_node = person_iter.second();
            std::string medical_condition;
            medical_condition_node >> medical_condition;
            person.set_medical_condition(medical_condition);
        }
        else if(key == "is_male")
        {
            const YAML::Node& is_male_node = person_iter.second();
            bool is_male;
            is_male_node >> is_male;
            person.set_is_male(is_male);
        }
        else
        {
            assert(!"Unhandled parameter when reading Person data from YAML");
        }
    }
}
            
