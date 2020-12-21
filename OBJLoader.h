#pragma once
#ifndef OBJLOADER_H
#define OBJLOADER_H

#pragma once
//STD Libs

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<algorithm>
#include "vec3.h"



struct Mesh_Struct {
	const char* name;
	std::vector<vec3> position;
	std::vector<std::vector<int>> index_vertices;
};

static Mesh_Struct loadOBJ(const char* file_name) { //Revisar el tipo de salida

	//Vertex portions

	//Face vectors
	std::vector<int> vertex_position_indicies;

	//Vertex array
	Mesh_Struct vertices;

	std::stringstream ss;
	std::ifstream in_file(file_name);

	std::string objname;
	std::string line = "";
	std::string prefix = "";

	vec3 temp_vec3;

	int temp_glint = 0;

	//File open error check
	if (!in_file.is_open()) {
		throw "ERROR::OBJLOADER::Could not open file.";
	}



	//Read one line at a time
	while (std::getline(in_file, line)) {
		//Get the prefix of the line
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#") {
		}
		else if (prefix == "o") { //Object name
			ss >> objname;
			vertices.name = objname.c_str();
		}
		else if (prefix == "s") {
		}
		else if (prefix == "use_mtl") {
		}
		else if (prefix == "v") {  //Vertex position
			ss >> temp_vec3;
			vertices.position.push_back(temp_vec3);
		}
		else if (prefix == "f") {
			while (ss>> temp_glint) {
				vertex_position_indicies.push_back(temp_glint-1);

				//Handling characters
				if (ss.peek() == '/') {
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ') {
					ss.ignore(1, ' ');
				}
			}
			vertices.index_vertices.push_back(vertex_position_indicies);
			vertex_position_indicies.clear();
		}
		else {
		}
	}

	return vertices;
}
#endif // !OBJLOADER_H
