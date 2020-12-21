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
    //Setup image
    /*auto aspect_ratio = 16.0 / 9.0;
    int image_width = 600; 
    int image_height = static_cast<int>(image_width/aspect_ratio);
    int samples_per_pixel = 50;
    const int max_depth = 50;

    //World
    hitable_list world;
    point3 lookfrom; 
    point3 lookat;
    double vfov = 40.0;
    auto aperture = 0.0;
    color ambient(0, 0, 0);


    //Set_Scene
    switch (0) {
    case 1: 
        world = random_scene(); 
        ambient = color(0.7, 0.8, 1.0);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        aperture = 0.1;
        break; 
    case 2:
        world = two_spheres();
        ambient = color(0.7, 0.8, 1.0);
        lookfrom = point3(13, 2, 3); 
        lookat = point3(0, 0, 0); 
        vfov = 20.0; 
        break;
    case 3: 
        world = two_perlin_spheres();
        ambient = color(0.7, 0.8, 1.0);
        lookfrom = point3(13, 2, 3); 
        lookat = point3(0, 0, 0); 
        vfov = 20.0; 
        break;
    case 4: 
        ambient = color(0, 0, 0);
        break;
    case 5: 
        world = simple_light(); 
        samples_per_pixel = 200; 
        ambient = color(0.3, .3, .3);
        lookfrom = point3(26, 3, 6); 
        lookat = point3(0, 2, 0); 
        vfov = 20.0;
        break;

    default:
    case 6:
        world = just_a_sphere();
        samples_per_pixel = 100;
        ambient = color(0.1, 0.1, 0.1);
        lookfrom = point3(26, 3, 6);
        lookat = point3(0, 2, 0);
        vfov = 20.0;
        break;

    }


    //Setup Camera
    vec3 vup(0, 1, 0);
    auto disk_to_focus = 10.0; //(lookfrom - lookat).lenght();
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, disk_to_focus, 0.0, 1.0);
  
    //Render

    std::ofstream file("image.ppm"); 
    if (!file.is_open()) {
        std::cout << "Error de apertura de archivo\n";
        return 0;
    }

    file << "P3\n" << image_width << " " << image_height << "\n255\n"; 
    
    for (int j = image_height - 1; j >= 0; --j) {
        std::cout << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0.0, 0.0, 0.0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1.0);
                auto v = (j + random_double()) / (image_height - 1.0);
                ray r = cam.get_ray(u, v);
                vec3 p = r.at(2.0);
                pixel_color += ray_color(r, ambient, world, max_depth, max_depth);
            }
            write_color(file, pixel_color, samples_per_pixel);
          
        }
    }

    std::cout << "\nDone\n";*/

    gr Sample_Scene;
    std::vector<shared_ptr<hitable>> Lights; 
    auto pink = make_shared<lambertian>(color(0.9, 128 / 255, 192 / 255));
    auto metal_blue = make_shared<metal>(color(0.05, 0.05, 0.7), 0.5);
    //Sample_Scene.cube("Cubo", metal_blue);
    Sample_Scene.nh_sphere("Piso", point3(0, -1001, 0), 1000.0, pink);
    Lights.push_back(make_shared<spotlight>(point3(0, 1, 5), color(4, 4, 4),     vec3(1,0,0), "Lamp1"));
    Lights.push_back(make_shared<spotlight>(point3(0, 1, -5), color(5, 5, 5),    vec3(1,0,0), "Lamp2"));
    Lights.push_back(make_shared<spotlight>(point3(10, 1, 0), color(10, 10, 10), vec3(1,0,0), "Lamp3"));

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

    Sample_Scene.loadMesh("cubo", "cube2.obj", pink);


    Sample_Scene.render("Something", "image2.ppm", 300, 200, point3(26, 3, 6), point3(0, 2, 0), vec3(0, 1, 0), 20.0, color(0.1, 0.1, 0.1), Lights);



}

