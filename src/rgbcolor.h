#ifndef RGBCOLOR_H
#define RGBCOLOR_H

#include "yaml-cpp/yaml.h"

class RGBColor
{
public:
    RGBColor();
    RGBColor(int red, int green, int blue);

    void set_red(int red);
    int get_red() const;
    void set_green(int green);
    int get_green() const;
    void set_blue(int blue);
    int get_blue() const;

private:
    int red;
    int green;
    int blue;
};

YAML::Emitter& operator << (YAML::Emitter& out, const RGBColor& color);
void operator >> (const YAML::Node& node, RGBColor& color);


#endif //RGBCOLOR_H

