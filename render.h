#pragma once
#ifndef RENDER_H

#define RENDER_H

#include <iostream>
#include <fstream>
#include <cfloat>
#include <string>
#include <vector>

#include "rtweekend.h"
#include "hitable_list.h"
#include "sphere.h"
#include "material.h"
#include "moving_sphere.h"
#include "aarect.h"
#include "box.h"
#include "Polygons.h"
#include "Camera.h"
#include "color.h"

using namespace std;

class gr {
public: 
	hitable_list world; 
	ifstream in_file;
	shared_ptr<material> mat = make_shared<metal>(color(0.05, 0.05, 0.7), 0.4);

	gr() {}
	gr(const char* name, shared_ptr<material> mtp) {
		//Este archivo leerá los archivos lua y los interpretará para construir la escena.
		string line;
		in_file.open(name); 
		if (!in_file) {
			cerr << "No se pudo abrir el archivo " << name << endl;
			exit(1);   // call system to stop
		}
		while (getline (in_file, line)) {
			cout << line << endl;
		}

		mat = mtp;
	}

	void nh_box(const char* name, const point3& corner, const double r) {
		world.add(make_shared<box>(name, corner, corner + r, mat));
	}

	void nh_sphere(const char* name, const point3& center, const double r) {
		world.add(make_shared<sphere>(name, center, r, mat));
	}

	void cube(const char* name) {
		world.add(make_shared<box>(name, point3(0, 0, 0), point3(1, 1, 1), mat));
	}

	void mesh(const char* name, const vector<point3>& positions, const vector<vector<int>>& i_v) {
		world.add(make_shared<mesh>(name, positions, i_v, mat)); 
	}

	void light(const char* name, const point3& position, const color& intensity, const vec3& attenuation) {
		world.add(make_shared<light>(name, position, intensity));
	}

	void render(
		const char* node, const char* filename, int w, int h, const point3& eye, const point3& view,
		const point3& up, double fov, const color& ambient, vector<shared_ptr<hitable>> lights) {
		
		//Other Parameters
		int samples_per_pixel = 50;
		const int max_depth = 50;
		auto aspect_ratio = w / h;
		auto aperture = 0.0;

		//Setup Camera
		auto disk_to_focus = (eye - view).lenght();
		camera cam(eye,view, up, fov, aspect_ratio, aperture, disk_to_focus, 0.0, 0.0);

		//Adding lights
		for (const auto& spotlight : lights) {
			world.add(spotlight);
		}

		//Render

		std::ofstream file(filename);
		if (!file.is_open()) {
			std::cout << "Error de apertura de archivo\n";
			exit(1);
		}

		file << "P3\n" << w << " " << h << "\n255\n";

		for (int j = h - 1; j >= 0; --j) {
			std::cout << "\rScanlines remaining: " << j << " " << std::flush;
			for (int i = 0; i < w; ++i) {
				color pixel_color(0.0, 0.0, 0.0);
				for (int s = 0; s < samples_per_pixel; ++s) {
					auto u = (i + random_double()) / (w - 1.0);
					auto v = (j + random_double()) / (h - 1.0);
					ray r = cam.get_ray(u, v);
					vec3 p = r.at(2.0);
					pixel_color += ray_color(r, ambient, world, max_depth, max_depth);
				}
				write_color(file, pixel_color, samples_per_pixel);

			}
		}

		std::cout << "\nDone\n";
	}
};
#endif // !RENDER_H
