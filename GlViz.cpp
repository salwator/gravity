#include <cstdlib>
#include "GlViz.h"
#include <iostream>

GlViz::GlViz(units::base_space world_size, int x_size, int y_size)
        :
        world_size(world_size),
        x_size(x_size),
        y_size(y_size)
{
    if(not glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(x_size, y_size, "PlanetSimulatorViz", NULL, NULL);

    if(not window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    };

    glfwSwapInterval(1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glfwGetFramebufferSize(window, &width, &height);
    ratio = float(width)/float(height);
    glOrtho(0, width, height, 0, 0, 1);
    glEnable(GL_SMOOTH);
}

void GlViz::print(const Planets & planets)
{
    glfwMakeContextCurrent(window);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    plot_planets(planets);
    glfwSwapBuffers(window);
}

GlViz::~GlViz()
{
    glfwTerminate();
}


void GlViz::plot_planets(const Planets & planets)
{

    glBegin(GL_POINTS);

    for(const auto & planet : planets)
    {
        auto shade = 0.5 + 0.5*(planet.mass() / units::M);

        glColor3f(0.1+shade,shade,shade);
        glPointSize(planet.mass()/units::M);
        glVertex2f((planet.position().x/world_size)/ratio,
                    planet.position().y/world_size);
    }

    glEnd();
}

