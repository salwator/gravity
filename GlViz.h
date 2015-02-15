#pragma once

#include <GLFW/glfw3.h>
#include "space.h"

class GlViz
{
public:
    GlViz(units::base_space, units::base_space);

    void print(const Planets & planets);

    ~GlViz();

private:
    void plot_planets(const Planets & planets);

    units::base_space world_size_x, world_size_y;
    GLFWwindow* window;
    float ratio;
    int width, height;
};
