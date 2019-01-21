#pragma once

#include <vector>

#include "geometry.h"
#include "sphere.h"
#include "light.h"
#include "material.h"

struct World
{
    World()
    {
        Material      ivory(1.0, Vec4f(0.6,  0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3),   50.);
        Material      glass(1.5, Vec4f(0.0,  0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8),  125.);
        Material red_rubber(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(0.3, 0.1, 0.1),   10.);
        Material     mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.);

        spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2, ivory));
        spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2,      glass));
        spheres.push_back(Sphere(Vec3f( 1.5, -0.5, -18), 3, red_rubber));
        spheres.push_back(Sphere(Vec3f( 7,    5,   -18), 4,     mirror));

        lights.push_back(Light(Vec3f(-20, 20, 20), 1.5));
        lights.push_back(Light(Vec3f( 30, 50, -25), 1.8));
        lights.push_back(Light(Vec3f( 30, 20,  30), 1.7));
    }

    std::vector<Sphere> spheres;
    std::vector<Light> lights;
    int scale = 15;
    float fov = static_cast<float>(M_PI / 3);
    int camera[3] = { 0, 2, 0 };
};