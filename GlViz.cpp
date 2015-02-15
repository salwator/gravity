#include <cstdlib>
#include "GlViz.h"

GlViz::GlViz(units::base_space world_size_x,
             units::base_space world_size_y)
        :
        world_size_x(world_size_x),
        world_size_y(world_size_y)
{
    if(not glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(1280, 1024, "PlanetSimulatorViz", NULL, NULL);

    if(not window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    };

    glfwSwapInterval(1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
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
        auto shade = planet.mass / units::M;

        glColor3f(0.1+shade,shade,shade);
        glPointSize(planet.mass/units::M);
        glVertex2f(planet.x/world_size_x,
                   planet.y/world_size_y);
    }

    glEnd();
}

