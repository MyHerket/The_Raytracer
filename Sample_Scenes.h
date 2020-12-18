#pragma once
#ifndef SAMPLE_SCENES_H
#define SAMPLE_SCENES_H


#include <iostream>
#include <fstream>
#include <cfloat>

#include "rtweekend.h"
#include "hitable_list.h"
#include "sphere.h"
#include "material.h"
#include "moving_sphere.h"
#include "aarect.h"
#include "box.h"
#include "Polygons.h"

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
    auto pertext = make_shared<noise_texture>(6);
    auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto metal_blue = make_shared<metal>(color(0.05, 0.05, 0.7), 0.4);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    //objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -5, difflight));
    objects.add(make_shared<xz_rect>(0, 5, 1, 5, 6, difflight));
    //objects.add(make_shared<yz_rect>(0, 3, 0, 3, -10, difflight));

    shared_ptr<hitable> box1 = make_shared<box>(point3(0, 0, 0), point3(3, 3, 3), red);
    box1 = make_shared<rotate_x>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(2, 0, -2));
    objects.add(box1);

    shared_ptr<hitable> box2 = make_shared<box>(point3(0, 0, 0), point3(2, 2, 2), metal_blue);
    box1 = make_shared<rotate_y>(box2, -15);
    box1 = make_shared<translate>(box2, vec3(-15, 0, 10));
    objects.add(box2);

    objects.add(make_shared<sphere>(point3(10, 4, 0), 0.45, metal_blue));
    objects.add(make_shared<sphere>(point3(10, 4, 0), 0.5,make_shared<dielectric>(1.3)));

    return objects;
}


hitable_list just_a_sphere() {
    hitable_list objects;
    auto pertext = make_shared<noise_texture>(6);
    auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto pink = make_shared<lambertian>(color(0.9, 128/255, 192/255));
    auto metal_blue = make_shared<metal>(color(0.05, 0.05, 0.7), 0.5);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -6, difflight));
    objects.add(make_shared<xz_rect>(0, 5, 1, 5, 6, difflight));
    
    std::vector<point3> positions; 
    positions.push_back(vec3(3, 0, 3));
    positions.push_back(vec3(3, 0, -3));
    positions.push_back(vec3(3, 3, -5));
    positions.push_back(vec3(3, 5, 0));
    positions.push_back(vec3(3, 3, 5));
    positions.push_back(vec3(4, 6, 2));

    std::vector<std::vector<int>> index_v;
    index_v.push_back(std::vector<int>{0,1, 2, 3, 4});
    index_v.push_back(std::vector<int>{3, 4, 5});


    //objects.add(make_shared<polygon>(positions, vertices, red));
    objects.add(make_shared<malla>(positions, index_v, metal_blue));

    return objects;
}

#endif // !1
