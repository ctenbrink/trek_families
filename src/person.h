#ifndef PERSON_H
#define PERSON_H

#include <string>

namespace YAML
{
    class Emitter;
    class Node;
}

class Person
{
public:
    Person();
    Person( const std::string& first_name_in,
            const std::string& last_name_in,
            bool is_male_in,
            unsigned int age_in,
            double height_in,
            double weight_in,
            const std::string& ward_in,
            bool on_youth_committee_in,
            const std::string& medical_condition_in);

    const std::string& get_first_name() const;
    const std::string& get_last_name() const;
    bool get_is_male() const;
    unsigned int get_age() const;
    double get_height() const;
    double get_weight() const;
    const std::string& get_ward() const;
    bool get_on_youth_committee() const;
    const std::string& get_medical_condition() const;
    
    void set_first_name(const std::string& first_name_in);
    void set_last_name(const std::string& last_name_in);
    void set_is_male( bool is_female_in);
    void set_age(unsigned int age_in);
    void set_height(double height_in);
    void set_weight(double weight_in);
    void set_ward(const std::string& ward_in);
    void set_on_youth_committee(bool on_youth_committee_in);
    void set_medical_condition(const std::string& medical_condition_in);
    
    std::string to_yaml() const;
    static Person from_yaml(const std::string& yaml);
    
    bool operator==(const Person& other) const;
    bool operator<(const Person& other) const;

private:
    std::string first_name;
    std::string last_name;
    bool is_male;
    unsigned int age;
    double height;
    double weight;
    std::string ward;
    bool on_youth_committee;
    std::string medical_condition;
};

YAML::Emitter& operator << (YAML::Emitter& out, const Person&);
void operator >> (const YAML::Node& node, Person& person);


#endif //PERSON_H
