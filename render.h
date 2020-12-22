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
#include "OBJLoader.h"

using namespace std;

struct compare
{
	const char* key;
	compare(const char* i) : key(i) { }

	bool operator()(shared_ptr<hitable> i)
	{
		return (strcmp(i->get_name(), key));
	}
};

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
		return (1.0 - 2 * t) * back2 + 2 * t * back3;
	}

}

color ray_color(
	const ray& r, 
	const color& ambient,
	const hitable& world, 
	shared_ptr<hitable> lights, 
	int depth, 
	int first_depth) {
	hit_record rec;
	if (depth <= 0)
		return ambient;

	if (world.hit(r, 0.001, infinity, rec)) {
		scatter_record srec; 
		color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
		if (!rec.mat_ptr->scatter(r, rec, srec))
			return emitted;
		if (srec.is_specular)
			return srec.attenuation
			* ray_color(srec.specular_ray, ambient, world, lights, depth - 1, first_depth);
		auto light_ptr = make_shared<hitable_pdf>(lights, rec.p);
		mixture_pdf p(light_ptr, srec.pdf_ptr);

		ray scattered = ray(rec.p, p.generate(), r.time());
		auto pdf_val = p.value(scattered.direction());

		return emitted + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered)
			* ray_color(scattered, ambient, world, lights, depth - 1, first_depth) / pdf_val;
	
	}
	else if (depth == first_depth)
		return background(r);
	else {
		return ambient;
	}
}

class scene {
public: 
	hitable_list world; 
	ifstream in_file;
	shared_ptr<material> def;
	shared_ptr<hitable_list> Lamps;

	scene() {}
	scene(const char* name, const shared_ptr<material> mtp) {
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

	void loadMesh(const char* name, const char* filename, const shared_ptr<material> mat) {
		Mesh_Struct OBJMesh = loadOBJ(filename); 
		mesh(name, OBJMesh.position, OBJMesh.index_vertices, mat);
	}

	int find(const char* name) {
		auto itr = find_if(world.objects.cbegin(), world.objects.cend(), compare("name"));

		if (itr != world.objects.cend()) {
			cout << "Element present at index " <<
				distance(world.objects.cbegin(), itr);
			return distance(world.objects.cbegin(), itr);
		}
		else {
			std::cout << "Element not found";
			return NULL;
		}
	}

	void move(const char* name, const vec3& displacement) {
		int itr = find(name); 
		world.objects[itr] = make_shared<translate>(world.objects[itr], displacement);
	}

	void rotate(const char* name, const vec3& rotation) {
		int itr = find(name); 
		world.objects[itr] = make_shared<rotate_x>(world.objects[itr], rotation[0]);
		world.objects[itr] = make_shared<rotate_y>(world.objects[itr], rotation[1]);
		world.objects[itr] = make_shared<rotate_z>(world.objects[itr], rotation[2]);
	}

	void render(
		const char* node, const char* filename, int w, int h, const point3& eye, const point3& view,
		const point3& up, double fov, const color& ambient, shared_ptr<hitable> lights) {
		
		//Other Parameters	
		int samples_per_pixel = 50;
		const int max_depth = 50;
		auto aspect_ratio = w / h;
		auto aperture = 0.0;

		//Setup Camera
		auto disk_to_focus = 10.0;
		camera cam(eye,view, up, fov, aspect_ratio, aperture, disk_to_focus, 0.0, 1.0);


		//Render

		std::ofstream file(filename);
		if (!file.is_open()) {
			std::cout << "Error de apertura de archivo\n";
			exit(1);
		}

		file << "P3\n" << w << " " << h << "\n255\n";

		for (int j = h - 1; j >= 0; --j) {
			std::cout << "\rScanlines remaining: " << j << " Processed " <<(1.0 - double(j/(h-1)))*100 << "%" << " " << std::flush;
			for (int i = 0; i < w; ++i) {
				color pixel_color(0.0, 0.0, 0.0);
				for (int s = 0; s < samples_per_pixel; ++s) {
					auto u = (i + random_double()) / (w - 1.0);
					auto v = (j + random_double()) / (h - 1.0);
					ray r = cam.get_ray(u, v);
					vec3 p = r.at(2.0);
					pixel_color += ray_color(r, ambient, world, lights, max_depth, max_depth);
				}
				write_color(file, pixel_color, samples_per_pixel);

			}
		}

		std::cout << "\nDone\n";
	}
};
#endif // !RENDER_H
