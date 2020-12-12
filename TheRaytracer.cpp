// TheRaytracer.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <cfloat>

#include "rtweekend.h"
#include "Camera.h"

color ray_color(const ray& r, const hitable& world) {
    hit_record rec; 
    if (world.hit(r, 0.0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1.0, 1.0, 1.0));
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
    int image_width = 400; 
    int image_height = static_cast<int>(image_width/aspect_ratio);
    int samples_per_pixel = 100;

    //Setup Camera
    camera cam;

    //World
    hitable_list world;
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5));
    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0));


    //Render

    std::ofstream file("image.ppm"); 
    if (!file.is_open()) {
        std::cout << "Error de apertura de archivo\n";
        return 0;
    }

    file << "P3\n" << image_width << " " << image_height << "\n255" << std::endl; 
    
    for (int j = image_height - 1; j >= 0; j--) {
        std::cout << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < image_width; i++) {
            color pixel_color(0.0, 0.0, 0.0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = double(i) / double(image_width - 1.0);
                auto v = double(j) / double(image_height - 1.0);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(file, pixel_color, samples_per_pixel);
          
        }
    }

    std::cout << "\nDone\n";
}

