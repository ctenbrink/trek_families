#include "rgbcolor.h"
#include "yaml-cpp/yaml.h"

RGBColor::RGBColor() :
    red(255),
    green(255),
    blue(255)
{
}

RGBColor::RGBColor(int red_in, int green_in, int blue_in) :
    red(red_in),
    green(green_in),
    blue(blue_in)
{
}

void RGBColor::set_red(int red_in)
{
    red = red_in;
}

int RGBColor::get_red() const
{
    return red;
}

void RGBColor::set_green(int green_in)
{
    green = green_in;
}

int RGBColor::get_green() const
{
    return green;
}

void RGBColor::set_blue(int blue_in)
{
    blue = blue_in;
}

int RGBColor::get_blue() const
{
    return blue;
}

YAML::Emitter& operator << (YAML::Emitter& out, const RGBColor& color)
{
    out << YAML::BeginMap;
    out << YAML::Key << "RGBColor" << YAML::Value;
        out << YAML::Flow;
        out << YAML::BeginMap;
        out << YAML::Key << "red" << YAML::Value << color.get_red();
        out << YAML::Key << "green" << YAML::Value << color.get_green();
        out << YAML::Key << "blue" << YAML::Value << color.get_blue();
        out << YAML::EndMap;
    out << YAML::EndMap;
    return out;
}

void operator >> (const YAML::Node& node, RGBColor& color)
{
    const YAML::Node& color_node = node["RGBColor"];
    for( YAML::Iterator color_iter = color_node.begin();
         color_iter != color_node.end();
         ++color_iter )
    {
        std::string key;
        color_iter.first() >> key;
           
        if(key == "red")
        {
            const YAML::Node& red_node = color_iter.second();
            int red;
            red_node >> red;
            color.set_red(red);
        }
        else if(key == "green")
        {
            const YAML::Node& green_node = color_iter.second();
            int green;
            green_node >> green;
            color.set_green(green);
        }
        else if(key == "blue")
        {
            const YAML::Node& blue_node = color_iter.second();
            int blue;
            blue_node >> blue;
            color.set_blue(blue);
        }
        else
        {
            assert(!"Unhandled parameter when reading RGBColor data from YAML");
        }
    }
}

  
