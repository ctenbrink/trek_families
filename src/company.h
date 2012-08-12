#ifndef COMPANY_H
#define COMPANY_H

#include <string>
#include <vector>
#include "rgbcolor.h"

class Company
{
public:
    Company();
    Company( const std::string& leader_in, 
             const std::string& medical_in,
             const RGBColor& color_in);
    
    const std::string& get_name() const;
    void set_name(const std::string& name_in);

    const std::string& get_leader() const;
    void set_leader(const std::string& leader_in);

    const std::string& get_medical() const;
    void set_medical(const std::string& medical_in);

    const std::vector<std::string>& get_members() const;
    std::vector<std::string>& get_members();
    std::vector<std::string> get_all_members() const;

    const RGBColor& get_color() const;
    void set_color(const RGBColor& color_in);

    void add_member(const std::string& member);
    
private:
    std::string name;
    std::string leader;
    std::string medical;
    std::vector<std::string> members;
    RGBColor color;
};

YAML::Emitter& operator << (YAML::Emitter& out, const Company& company);
void operator >> (const YAML::Node& node, Company& company);

#endif //COMPANY_H

