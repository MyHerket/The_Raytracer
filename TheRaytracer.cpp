// TheRaytracer.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "ray.h"

bool hit_sphere(const vec3& center, float radius, const ray& r) {
    //Función que me permite saber si golpee una esfera con centro c y radio radius. 
    vec3 oc = r.origin() - center; 
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction()); 
    float c = dot(oc, oc) - radius * radius; 
    float discriminant = b * b - 4 * a * c; 

    if (discriminant < 0) return -1.0;
    else return (-b - sqrt(discriminant)) / (2.0 * a);
}

vec3 color(const ray& r) {
    float t = hit_sphere(vec3(0.0, 0.0, -1.0), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0.0, 0.0, -1.0));
        if (r.direction() == vec3(0.0, 0.0, -1.0)) return vec3(0.0, 0.5, 0.5);
        //Es necesario agregar esto que es para cuando el normal apunta exactamente al origen del raytracer
        return 0.5 * vec3(N.x() + 1.0, N.y() + 1.0, N.z() + 1.0); 
    }

    // Esto pone el fondo. En este caso es un degradado horizontal.
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    //vec3(0,0,0) = Blanco
    //vec3(0.5, 0.7, 1) = Azul Así que obtenemos un degradado de azul a blanco de arriba a abajo
}

int main()
{
    int nx = 20; 
    int ny = 10; 
    std::cout << "P3\n" << nx << " " << ny << "\n255\n"; 
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            vec3 dir = lower_left_corner + (u * horizontal) + (v * vertical);
            ray r(origin, dir);
            vec3 col = color(r);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            std::cout << ir << " " << ig << " " << ib <<  "\n"; 
        }
    }
}
