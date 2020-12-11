// TheRaytracer.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "Vec3_Class.h"

int main()
{
    int nx = 200; 
    int ny = 100; 
    std::cout << "P3\n" << nx << " " << ny << "\n255\n"; 
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = nx - 1; i >= 0; i--) {
            vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n"; 
        }
    }
}
