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
		
		output_box = aabb(min, max);
		return true;
	}

	shared_ptr<material> mp;
	point3 v1; 
	point3 v2; 
	point3 v3;
};

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	auto t = (k - r.origin().z()) / r.direction().z();
	if (t<t_min || t>t_max)
		return false;
	auto x = r.origin().x() + t * r.direction().x();
	auto y = r.origin().y() + t * r.direction().y();
	if (x<x0 || x>x1 || y<y0 || y>y1)
		return false;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	auto outward_normal = vec3(0, 0, 1);
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mp;
	rec.p = r.at(t);
	return true;
}

#endif // !POLYGON_H
