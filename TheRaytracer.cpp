// TheRaytracer.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <cfloat>

#include "rtweekend.h"
#include "Camera.h"
#include "color.h"
#include "Sample_Scenes.h"
#include "render.h"
int main()
{
    //Materiales y Texturas
    auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    auto pertext = make_shared<noise_texture>(6);
    auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto chess = make_shared<checker_texture>(color(1, 1, 1), color(0.05, 0.05, 0.7));
    auto metal_blue = make_shared<metal>(chess, 0.5);
    auto metal_red = make_shared<metal>(color(0.7, 0.05, 0.05), 0.7);

    //Escena1
    /*scene Sample_Scene;
    auto lights = make_shared<hitable_list>();
    lights->add(make_shared<xy_rect>(3, 5, 1, 3, -6, difflight));
    lights->add(make_shared<xz_rect>(0, 5, 1, 5, 6, difflight));

    
    Sample_Scene.nh_sphere("Ground", point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext));

    Sample_Scene.add("Light1", make_shared<xy_rect>(3, 5, 1, 3, -6, difflight));
    Sample_Scene.add("Light2", make_shared<xz_rect>(0, 5, 1, 5, 6,  difflight));

    std::vector<point3> positions;
    positions.push_back(vec3(3, 0, 3));
    positions.push_back(vec3(3, 0, -3));
    positions.push_back(vec3(3, 3, -5));
    positions.push_back(vec3(3, 5, 0));
    positions.push_back(vec3(3, 3, 5));
    positions.push_back(vec3(4, 6, 2));

    std::vector<std::vector<int>> index_v;
    index_v.push_back(std::vector<int>{0, 1, 2, 3, 4});
    index_v.push_back(std::vector<int>{3, 4, 5});

    Sample_Scene.mesh("Pentagono", positions, index_v, metal_blue);
    Sample_Scene.nh_box("Cubo", point3(10,0,1), 1.0, metal_red);
    Sample_Scene.nh_box("Cubo_cristal", point3(10.1,0.1,1.1), 1.1, make_shared<dielectric>(1.3));

    Sample_Scene.render("Something", "image3.ppm", 600, 300, point3(26, 3, 6), point3(0, 2, 0), vec3(0, 1, 0), 20.0, color(0.1, 0.1, 0.1), lights);*/

    scene Ball;
    Ball.nh_sphere("Ball", point3(0, 0, 0), 1, make_shared<basic>(color(0.7, 1.0, 0.7), 0.5, 0.7, 0.5));
    Ball.light("Lamp1", point3(2, 2, 2), color(4, 4, 4), vec3(1, 0, 0));
    auto light = make_shared<hitable_list>(make_shared<spotlight>(point3(2,2,2), color(4, 4,4), vec3(1, 0, 0)));
    Ball.render("Scene", "Ball.ppm", 400, 200, point3(26, 3, 6), point3(0, 0, 0), vec3(0, 1, 0), 20.0, color(0.1, 0.1, 0.1), light);
    



}

