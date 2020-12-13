// TheRaytracer.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <cfloat>

#include "rtweekend.h"

#include "color.h"
#include "hitable_list.h"
#include "sphere.h"
#include "Camera.h"
#include "material.h"

color ray_color(const ray& r, const hitable& world, int depth) {
    hit_record rec; 
    if (depth <= 0)
        return color(0, 0, 0); 

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }
    else {
        // Esto pone el fondo. En este caso es un degradado horizontal.
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
        //vec3(0,0,0) = Blanco
        //vec3(0.5, 0.7, 1) = Azul Así que obtenemos un degradado de azul a blanco de arriba a abajo
    }
}

int main()
{
    //Setup image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 200; 
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 10;

    //World
    hitable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
    auto material_glass = make_shared<dielectric>(1.5);


    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_glass));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_glass));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_glass));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));


    //Setup Camera
    camera cam;
  
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
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(file, pixel_color, samples_per_pixel);
          
        }
    }

    std::cout << "\nDone\n";
}

