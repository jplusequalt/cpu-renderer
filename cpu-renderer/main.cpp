#include <iostream>
#include <string>

#include "linalg.hpp"
#include "renderer.hpp"
#include "model.hpp"

int main()
{
    Model m{ "./assets/head.obj" };
    Renderer r{ 800, 600 };
    r.renderWireframe(m);
    r.saveImage("head.png");

    return 0;
}