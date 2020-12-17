// TheRaytracer.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <cfloat>

#include "rtweekend.h"
#include "Camera.h"
#include "color.h"
#include "Sample_Scenes.h"

color background(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.z() + 1.0);
    color back3(0.0, 0.0, 1.0);
    color back1(1.0, 1.0, 1.0); 
    color back2(0.0, 1.0, 0.0); 

    if (t <= 0.5) {
        return (1.0 - 2 * t) * back1 + 2 * t * back2;
    }
    else {
        t = t - 0.5;
        return (1.0 - 2*t) * back2 + 2*t*back3;
    }
        
}

color ray_color(const ray& r, const color& ambient, const hitable& world, int depth, int first_depth) {
    hit_record rec;
    if (depth <= 0)
        return ambient;

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return emitted;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return emitted + attenuation * ray_color(scattered, ambient, world, depth - 1, first_depth);
        return color(0, 0, 0);
    }
    else if (depth == first_depth)
        return background(r);
    else {
        return ambient;
    }
}

int main()
{
    //Setup image
    auto aspect_ratio = 16.0 / 9.0;
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
        samples_per_pixel = 50;
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

    std::cout << "\nDone\n";
}

