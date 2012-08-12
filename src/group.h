#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include <string>
#include "person.h"
#include "rgbcolor.h"
#include "yaml-cpp/yaml.h"

class Group
{
public:
    Group();
    explicit Group(const std::string& name_in);

    const std::string& get_name() const;
    void set_name(const std::string& name_in);
    
    const std::string& get_unit() const;
    void set_unit(const std::string& unit_in);
    
    bool get_has_medical_training() const;
    void set_has_medical_training(bool has_medical_training_in);
    
    const RGBColor& get_color() const;
    void set_color(const RGBColor& color_in);

    const std::vector<Person>& get_members() const;
    std::vector<Person>& get_members();

private:
    std::string name;
    std::string unit;
    bool has_medical_training;
    std::vector<Person> members;
    RGBColor color;
};

YAML::Emitter& operator << (YAML::Emitter& out, const Group& group);
void operator >> (const YAML::Node& node, Group& group);

std::string make_title_from_group(const Group& group);

#endif //GROUP_H
