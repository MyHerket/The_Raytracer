#pragma once
#ifndef POLYGON_H
#define POLYGON_H

#include "rtweekend.h"
#include "hitable_list.h"

class triangle : public hitable {
public:
	triangle() {}
	triangle(const point3& vertex_1, const point3& vertex_2, const point3& vertex_3, shared_ptr<material> mat) :
		v1(vertex_1), v2(vertex_2), v3(vertex_3), mp(mat) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box)const override {
		//The bounding box must have non-zero width in each dimension, so pad the Z
		//dimension a small amount
		point3 min(fmin(fmin(v1.x(), v2.x()), v3.x()), fmin(fmin(v1.y(), v2.y()), v3.y()), fmin(fmin(v1.z(), v2.z()), v3.z()));
		point3 max(fmax(fmax(v1.x(), v2.x()), v3.x()), fmax(fmax(v1.y(), v2.y()), v3.y()), fmax(fmax(v1.z(), v2.z()), v3.z()));

		auto outward_normal = unit_vector(cross((v2 - v1), (v3 - v1)));
		if (outward_normal == vec3(1, 0, 0)){
			min += vec3(-0.0001, 0, 0);
			max += vec3(0.0001, 0, 0);
		}
		else if (outward_normal == vec3(0, 1, 0)) {
			min += vec3(0, -0.0001, 0);
			max += vec3(0, 0.0001, 0);
		}
		else if (outward_normal == vec3(0, 0, 1)) {
			min += vec3(0, 0, -0.0001);
			max += vec3(0, 0, 0.0001);
		}

		output_box = aabb(min, max);
		return true;
	}

	shared_ptr<material> mp;
	point3 v1; 
	point3 v2; 
	point3 v3;
};

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	auto outward_normal = unit_vector(cross((v2-v1), (v3-v1)));
	auto dist = -dot(outward_normal, v1);
	auto t = -(dot(outward_normal, r.origin()) + dist) / (dot(outward_normal, r.direction()));
	if (t<t_min || t>t_max)
		return false;

	auto p = t * r.direction() + r.origin();
	auto s1 = dot(outward_normal, cross(v2 - v1, p - v1));
	auto s2 = dot(outward_normal, cross(v3 - v2, p - v2));
	auto s3 = dot(outward_normal, cross(v1 - v3, p - v3));

	if (s1 * s2 < 0 || s1 * s3 < 0 || s2 * s3 < 0)
		return false;

	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.at(t);
	return true;
}

//Polygon es un caso particular de una malla

class polygon : public hitable {
public:
	polygon() {}
	polygon(const std::vector<point3>& positions, std::vector<int> vertex, shared_ptr<material> ptr);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
		return sides.bounding_box(time0, time1, output_box);
	}

public:
	std::vector<int> vertices;
	hitable_list sides;
};

polygon::polygon(const std::vector<point3>& positions, std::vector<int> v, shared_ptr<material> ptr) {
	vertices = v;
	for (std::size_t i = 1; i < v.size()-1; ++i) {
		sides.add(make_shared<triangle>(positions[v[0]], positions[v[i]], positions[v[i + 1]], ptr));
	}
}

bool polygon::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	return sides.hit(r, t_min, t_max, rec);
}

#endif // !POLYGON_H
