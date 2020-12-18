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


color background(const ray& r) {
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.z() + 1.0);
	color back3(0.0, 0.0, 1.0);
	color back1(0.0, 0.0, 0.0);
	color back2(0.0, 1.0, 0.0);

	if (t <= 0.5) {
		return (1.0 - 2 * t) * back1 + 2 * t * back2;
	}
	else {
		t = t - 0.5;
		return (1.0 - 2 * t) * back2 + 2 * t * back3;
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

class gr {
public: 
	hitable_list world; 
	ifstream in_file;
	shared_ptr<material> def;

	gr() {}
	gr(const char* name, const shared_ptr<material> mtp) {
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
		def = mtp;
	}


	void nh_box(const char* name, const point3& corner, const double r, const shared_ptr<material> mat) {
		world.add(make_shared<box>(corner, corner + r, mat, name));
	}

	void nh_sphere(const char* name, const point3& center, const double r, const shared_ptr<material> mat) {
		world.add(make_shared<sphere>(center, r, mat, name));
	}

	void cube(const char* name, const shared_ptr<material> mat) {
		world.add(make_shared<box>( point3(0, 0, 0), point3(1, 1, 1), mat, name));
	}

	void mesh(const char* name, const vector<point3>& positions, const vector<vector<int>>& i_v, const shared_ptr<material> mat) {
		auto M = make_shared<malla>(positions, i_v, mat, name);
		world.add(M);
	}

	void light(const char* name, const point3& position, const color& intensity, const vec3& attenuation){
		auto lamp = make_shared<spotlight>(position, intensity, attenuation, name);
		world.add(lamp);
	}

	void render(
		const char* node, const char* filename, int w, int h, const point3& eye, const point3& view,
		const point3& up, double fov, const color& ambient, vector<shared_ptr<hitable>> lights) {
		
		//Other Parameters	
		int samples_per_pixel = 100;
		const int max_depth = 50;
		auto aspect_ratio = w / h;
		auto aperture = 0.1;

		//Setup Camera
		auto disk_to_focus = 10.0;
		camera cam(eye,view, up, fov, aspect_ratio, aperture, disk_to_focus, 0.0, 1.0);

		//Adding lights
		for (const auto& lamps: lights) {
			world.add(lamps);
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
