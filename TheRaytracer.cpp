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
    //Materials
    auto mat1 = make_shared<basic>(color(0.7, 1.0, 0.7), 0.5, 0.7, 0.5);
    auto mat2 = make_shared<basic>(color(0.5, 0.5, 0.5), 0.5, 0.7, 0.5);
    auto mat3 = make_shared<basic>(color(1.0, 0.6, 0.1), 0.5, 0.7, 0.5);
    auto mat4 = make_shared<basic>(color(0.7, 0.6, 1.0), 0.5, 0.4, 0.8);

    Ball.nh_sphere("s1", point3(0, 0, -400), 100, mat1);
    Ball.nh_sphere("s2", point3(200, 50, -100), 150, mat1);
    Ball.nh_sphere("s3", point3(0, -1200, -500), 1000, mat2 );
    Ball.nh_box("b1", point3(-200, -125, 0), 100, mat4 );
    Ball.nh_sphere("s4", point3(-100, 25, -300), 50, mat3);
    Ball.nh_sphere("s5", point3(0, 100, -250), 25, mat1);

    //Ball.light("white_light", point3(-100, 150, 400), color(0.9, 0.9, 0.9), vec3(1, 0, 0));
    //Ball.light("orange_light", point3(400, 100, 150), color(0.7, 0.0, 0.7), vec3(1, 0, 0));
    //Ball.loadMesh("Dodecahedron", "smstdodeca.obj", mat3);
    Mesh_Struct OBJMesh = loadOBJ("smstdodeca.obj");
    auto Dode = make_shared<malla>(OBJMesh.position, OBJMesh.index_vertices, mat3, "Dodeca");
    Dode = make_shared<translate>(Dode, point3(200, 200, 200));
    Ball.add("Dodeca", Dode);


    auto light = make_shared<hitable_list>(make_shared<spotlight>(point3(2,2,2), color(4, 4,4), vec3(1, 0, 0)));
    light->add(make_shared<spotlight>(point3(400, 100, 150), color(0.7, 0.0, 0.7), vec3(1, 0, 0)));

    Ball.render("Scene", "nonhier.ppm", 256, 256, point3(0, 0, 800), point3(0, 0, -1), vec3(0, 1, 0), 50.0, color(0.3, 0.3, 0.3), light);
    



}

