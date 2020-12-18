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

using namespace std;

class gr {
public: 
	hitable_list world; 
	ifstream in_file;
	shared_ptr<material> mat;

	gr() {}
	gr(const char* name) {
		//Este archivo leerá los archivos lua y los interpretará para construir la escena.
		string line;
		in_file.open(name); 
		if (!in_file) {
			cerr << "No se pudo abrir el archivo " << name << endl;
			exit(1);   // call system to stop
		}
		while (getline (in_file, line)) {
			cout << line << endl
		}

		mat = make_shared<metal>(color(0.05, 0.05, 0.7), 0.4);
	}

	void nh_box(const char* name, const point3& corner, const double r) const {
		world.add(make_shared<box>(name, corner, corner + r, mat));
	}

	void nh_sphere(const char* name, const point3& center, const double r) const {
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

	void render(const char* node, const char* filename, int w, int h, const point3& eye, const point3& view, const point3& up, double fov, const color& ambient, vector<make_shared<light>> lights) {

	}



};
#endif // !RENDER_H
