#pragma once

#include <GLFW/glfw3.h>
#include "space.h"

class GlViz
{
public:
    GlViz(units::base_space, int, int);

    void print(const Planets & planets);

    ~GlViz();

private:
    void plot_planets(const Planets & planets);

    units::base_space world_size;
    GLFWwindow* window;
    float ratio;
    int width, height;
    int x_size, y_size;
};
