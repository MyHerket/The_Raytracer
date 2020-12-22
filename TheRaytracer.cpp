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

    scene Sample_Scene;
    //auto Lights = make_shared<hitable_list>();
    //Lights->add(make_shared<sphere>(point3(10, 1, 0), 0.5, 0));
    auto lights = make_shared<hitable_list>();
    lights->add(make_shared<sphere>(point3(0, 1,  5), 0.25, shared_ptr<material>()));
    lights->add(make_shared<sphere>(point3(0, 1, -5), 0.25, shared_ptr<material>()));
    lights->add(make_shared<sphere>(point3(10, 1, 0), 0.25, shared_ptr<material>()));


    auto pink = make_shared<lambertian>(color(0.9, 128 / 255, 192 / 255));
    auto metal_blue = make_shared<metal>(color(0.05, 0.05, 0.7), 1.0);
    auto glass = make_shared<dielectric>(1.3);
    Sample_Scene.cube("Cubo", metal_blue);
    Sample_Scene.nh_sphere("Piso", point3(0, -1001, 0), 1000.0, pink);
    Sample_Scene.light("Lamp1", point3(0, 1, 5), color(10, 10, 10), vec3(1, 0, 0));
    Sample_Scene.light("Lamp2", point3(0, 1, -5), color(5, 5, 5),    vec3(1,0,0));
    Sample_Scene.light("Lamp3", point3(10, 1, 0), color(10, 10, 10), vec3(1,0,0));
    Sample_Scene.move("Cubo", vec3(1.0, 1.0, 1.0));
    Sample_Scene.rotate("Cubo", vec3(45, 0, 30));

    
    //Pentagono 
    /*Mesh_Struct Penta; 

    Penta.position.push_back(point3(0.0, 0.0, 2.0)); 
    Penta.position.push_back(point3(0.0, 0.0, -2.0)); 
    Penta.position.push_back(point3(0.0, 3.0, 5.0)); 
    Penta.position.push_back(point3(0.0, 3.0, -5.0)); 
    Penta.position.push_back(point3(0.0, 5.0, 0.0)); 

    std::vector<int> ind;
    ind.push_back(0);
    ind.push_back(2);
    ind.push_back(4);
    ind.push_back(3);
    ind.push_back(1);

    Penta.index_vertices.push_back(ind);
    Sample_Scene.mesh("Pent", Penta.position, Penta.index_vertices, metal_blue);*/

    //Sample_Scene.loadMesh("cubo", "cube2.obj", pink);


    Sample_Scene.render("Something", "image2.ppm", 200, 100, point3(26, 3, 6), point3(0, 0, 0), vec3(0, 1, 0), 20.0, color(0.1, 0.1, 0.1), lights);
    



}

