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
#include "moving_sphere.h"
#include "aarect.h"
#include "box.h"

hitable_list random_scene() {
    hitable_list world; 
    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    auto ground_material = make_shared<lambertian>(checker); 
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -1; b < 11; b++) {
            double choose_mat = random_double(); 
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).lenght() > 0.9) {
                shared_ptr<material> sphere_material; 

                if (choose_mat < 0.8) {
                    //difuse
                    auto albedo = color::random() * color::random(); 
                    sphere_material = make_shared<lambertian>(albedo); 
                    auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material)); 
                }
                else if (choose_mat < 0.95) {
                    //metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5); 
                    sphere_material = make_shared<metal>(albedo, fuzz); 
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    //glass
                    sphere_material = make_shared<dielectric>(1.5); 
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5); 
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hitable_list two_spheres() {
    hitable_list objects; 
    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}

hitable_list two_perlin_spheres() {
    hitable_list objects;
    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;

}

hitable_list simple_light() {
    hitable_list objects;
    auto pertext = make_shared<noise_texture>(4); 
    auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    //objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
    objects.add(make_shared<xz_rect>(0, 5, 1, 5, 6, difflight)); 
    //objects.add(make_shared<yz_rect>(0, 3, 0, 3, -10, difflight));

    shared_ptr<hitable> box1 = make_shared<box>(point3(0, 0, 0), point3(3, 3, 3), red);
    box1 = make_shared<rotate_x>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(2, 0, -2));
    objects.add(box1);

    return objects;
}

hitable_list cornell_box() {
    hitable_list objects; 

    auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = make_shared<diffuse_light>(color(7,7,7));

    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 0, light));

    //objects.add(make_shared<yz_rect>(0, 555, 0,  555,  555, green));
    //objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    //objects.add(make_shared<xz_rect>(0, 555, 0,  555, 0, white));
    //objects.add(make_shared<xz_rect>(0, 555, 0,  555,  555, white));
    //objects.add(make_shared<xy_rect>(0, 555, 0,  555,  555, white));

    /*shared_ptr<hitable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hitable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    objects.add(box2);*/

    return objects;
}

color ray_color(const ray& r, const color& background, const hitable& world, int depth, bool& back) {
    hit_record rec; 
    if (depth <= 0)
        return color(0, 0, 0); 

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return emitted;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return emitted + attenuation * ray_color(scattered, background, world, depth - 1, back);
        return color(0, 0, 0);
    }
    else {
        //para poner el fondo de un color solido entonces usar
        return background;
        // Esto pone el fondo. En este caso es un degradado horizontal.
        //vec3 unit_direction = unit_vector(r.direction());
        //auto t = 0.5 * (unit_direction.y() + 1.0);
        //return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0, 0, 0);
        //back = true;
        //vec3(0,0,0) = Blanco
        //vec3(0.5, 0.7, 1) = Azul Así que obtenemos un degradado de azul a blanco de arriba a abajo
    }
}

int main()
{
    //Setup image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400; 
    int image_height = static_cast<int>(image_width/aspect_ratio);
    int samples_per_pixel = 50;
    const int max_depth = 50;

    //World
    hitable_list world;
    point3 lookfrom; 
    point3 lookat;
    double vfov = 40.0;
    auto aperture = 0.0;
    color background(0, 0, 0);


    //Set_Scene
    switch (0) {
    case 1: 
        world = random_scene(); 
        background = color(0.7, 0.8, 1.0);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        aperture = 0.1;
        break; 
    case 2:
        world = two_spheres();
        background = color(0.7, 0.8, 1.0);
        lookfrom = point3(13, 2, 3); 
        lookat = point3(0, 0, 0); 
        vfov = 20.0; 
        break;
    case 3: 
        world = two_perlin_spheres();
        background = color(0.7, 0.8, 1.0);
        lookfrom = point3(13, 2, 3); 
        lookat = point3(0, 0, 0); 
        vfov = 20.0; 
        break;
    case 4: 
        background = color(0, 0, 0);
        break;
    default:
    case 5: 
        world = simple_light(); 
        samples_per_pixel = 200; 
        background = color(0, 0, 0);
        lookfrom = point3(26, 3, 6); 
        lookat = point3(0, 2, 0); 
        vfov = 20.0;
        break;
    case 6: 
        world = cornell_box(); 
        aspect_ratio = 1.0; 
        image_width = 600; 
        image_height = static_cast<int>(image_width / aspect_ratio);
        samples_per_pixel = 200; 
        background = color(0, 0, 0); 
        lookfrom = point3(278, 278, -800); 
        lookat = point3(278, 278, 0); 
        vfov = 40.0;
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
    
    bool back = false;
    for (int j = image_height - 1; j >= 0; --j) {
        std::cout << "\rScanlines remaining: " << j << " " << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0.0, 0.0, 0.0);
            for (int s = 0; s < samples_per_pixel && back == false; ++s) {
                auto u = (i + random_double()) / (image_width - 1.0);
                auto v = (j + random_double()) / (image_height - 1.0);
                ray r = cam.get_ray(u, v);
                vec3 p = r.at(2.0);
                pixel_color += ray_color(r, background, world, max_depth, back);
                if (back == true)
                    pixel_color = samples_per_pixel * ray_color(r, background, world, max_depth, back);
            }
            write_color(file, pixel_color, samples_per_pixel, back);
            back = false;
          
        }
    }

    std::cout << "\nDone\n";
}

