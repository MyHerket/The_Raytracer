// TheRaytracer.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <cfloat>
#include "hitable_list.h"

vec3 color(const ray& r, hitable* world) {
    hit_record rec; 
    if (world->hit(r, 0.0, FLT_MAX, rec)) {
        return 0.5 * vec3(rec.normal.x() + 1.0, rec.normal.y() + 1.0, rec.normal.z() + 1.0);
    }
    else {
        // Esto pone el fondo. En este caso es un degradado horizontal.
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
        //vec3(0,0,0) = Blanco
        //vec3(0.5, 0.7, 1) = Azul Así que obtenemos un degradado de azul a blanco de arriba a abajo
    }
}

int main()
{
    int nx = 200; 
    int ny = 100; 
    std::ofstream file("image.ppm"); 
    if (!file.is_open()) {
        std::cout << "Error de apertura de archivo\n";
        return 0;
    }

    file << "P3\n" << nx << " " << ny << "\n255" << std::endl; 
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    
    hitable* list[2]; 
    list[0] = new sphere(vec3(-0.7, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1.5), 100); 
    hitable* world = new hitable_list(list, 2);


    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            vec3 dir = lower_left_corner + (u * horizontal) + (v * vertical);

            ray r(origin, dir);
            vec3 col = color(r, world);

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            vec3 pix_col(ir, ig, ib);
            file <<pix_col << std::endl;
        }
    }
}
