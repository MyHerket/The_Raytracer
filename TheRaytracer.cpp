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
    auto lights = make_shared<hitable_list>();
    lights->add(make_shared<spotlight>(point3(0, 1, 5), color(10, 10, 10), vec3(1, 0, 0)));
    lights->add(make_shared<spotlight>(point3(0, 1, -5), color(10, 10, 10), vec3(1, 0, 0)));
    lights->add(make_shared<spotlight>(point3(10, 1, 0), color(10, 10, 10), vec3(1, 0, 0)));


    auto pink = make_shared<lambertian>(color(0.9, 128 / 255, 192 / 255));
    auto metal_blue = make_shared<Phong_material>(color(0.05, 0.05, 0.7), 1.0, 0.02);
    auto glass = make_shared<dielectric>(1.3);
    Sample_Scene.cube("Cubo", metal_blue);
    Sample_Scene.nh_sphere("Piso", point3(0, -1001, 0), 1000.0, pink);
    Sample_Scene.light("Lamp1", point3(0, 1, 5), color(10, 10, 10),  vec3(1,0,0));
    Sample_Scene.light("Lamp2", point3(0, 1, -5), color(10, 10, 10), vec3(1,0,0));
    Sample_Scene.light("Lamp3", point3(10, 1, 0), color(10, 10, 10), vec3(1,0,0));
   

    
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

    auto pertext = make_shared<noise_texture>(6);

    Sample_Scene.mesh("Pent", Penta.position, Penta.index_vertices, make_shared<lambertian>(pertext));

    auto chess = make_shared<checker_texture>(vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
    Sample_Scene.loadMesh("Cubo", "cube2.obj", make_shared<metal>(chess, 0.7));*/
    Sample_Scene.move("Cubo", vec3(1.0, 1.0, 1.0));
    Sample_Scene.rotate("Cubo", vec3(45, 0, 30));
    Sample_Scene.scaling("Cubo", 2.0);

    //Sample_Scene.nh_sphere("Esfera_vidrio", point3(2, 2, 2), 3, glass);

    Sample_Scene.render("Something", "image3.ppm", 200, 100, point3(26, 3, 6), point3(0, 0, 0), vec3(0, 1, 0), 20.0, color(0, 0, 0), lights);
    



}

