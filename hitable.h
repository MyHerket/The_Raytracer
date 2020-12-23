#pragma once
#ifndef HITABLEH
#define HITABLEH

#include "rtweekend.h"
#include "Bounding_Volume.h"

class material;

struct hit_record {
	double t; 
	double u; 
	double v;
	point3 p; 
	vec3 normal;
	shared_ptr<material> mat_ptr;
	bool front_face; 

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hitable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
	virtual double pdf_value(const point3& o, const vec3& v)const {
		return 0.0;
	}
	virtual vec3 random(const vec3& o) const {
		return vec3(1, 0, 0);
	}
	virtual const char* get_name()const {
		return "NULL";
	}
	virtual vec3 get_center()const {
		return vec3(0, 0, 0);
	}
	virtual color get_intensity()const {
		return color(0, 0, 0);
	}
};


class translate : public hitable {
public: 
	translate(shared_ptr<hitable> p, const vec3& displacement): ptr(p), offset(displacement){}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box)const override;

public:
	shared_ptr<hitable> ptr;
	vec3 offset;
};

bool translate::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	ray moved_r(r.origin() - offset, r.direction(), r.time());
	if (!ptr->hit(moved_r, t_min, t_max, rec))
		return false;
	rec.p += offset; 
	rec.set_face_normal(moved_r, rec.normal);
	return true;
}

bool translate::bounding_box(double time0, double time1, aabb& output_box) const {
	if (!ptr->bounding_box(time0, time1, output_box))
		return false; 
	output_box = aabb(output_box.min() + offset, output_box.max() + offset);

	return true;
}


//Rotation y axis
class rotate_y : public hitable {
public: 
	rotate_y(shared_ptr<hitable> p, double angle);

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec
	)const override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
		output_box = bbox; 
		return hasbox;
	}


	shared_ptr<hitable> ptr; 
	double sin_theta; 
	double cos_theta; 
	bool hasbox;
	aabb bbox;
};

rotate_y::rotate_y(shared_ptr<hitable> p, double angle) : ptr(p) {
	auto radians = degrees_to_radians(angle); 
	sin_theta = sin(radians); 
	cos_theta = cos(radians); 
	hasbox = ptr->bounding_box(0, 1, bbox);

	point3 min(infinity, infinity, infinity); 
	point3 max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				auto x = double(i) * bbox.max().x() + (1.0 - double(i)) * bbox.min().x();
				auto y = double(j) * bbox.max().y() + (1.0 - double(j)) * bbox.min().y();
				auto z = double(k) * bbox.max().z() + (1.0 - double(k)) * bbox.min().z();

				auto newx = cos_theta * x + sin_theta * z;
				auto newz = -sin_theta * x + cos_theta * z; 

				vec3 tester(newx, y, newz); 

				for (int c = 0; c < 3; c++) {
					min[c] = fmin(min[c], tester[c]); 
					max[c] = fmax(max[c], tester[c]);
				}
			}
		}
	}

	bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	auto origin = r.origin(); 
	auto direction = r.direction(); 
	origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
	origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

	direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
	direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];

	ray rotated_r(origin, direction, r.time()); 
	if (!ptr->hit(rotated_r, t_min, t_max, rec))
		return false; 

	auto p = rec.p;
	auto normal = rec.normal; 

	p[0] = cos_theta*rec.p[0] + sin_theta*rec.p[2];
	p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];

	normal[0] = cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
	normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

	rec.p = p; 
	rec.set_face_normal(rotated_r, normal); 
	return true;

}


//Rotation z_axis
class rotate_z : public hitable {
public:
	rotate_z(shared_ptr<hitable> p, double angle);

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec
	)const override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
		output_box = bbox;
		return hasbox;
	}


	shared_ptr<hitable> ptr;
	double sin_theta;
	double cos_theta;
	bool hasbox;
	aabb bbox;
};

rotate_z::rotate_z(shared_ptr<hitable> p, double angle) : ptr(p) {
	auto radians = degrees_to_radians(angle);
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0, 1, bbox);

	point3 min(infinity, infinity, infinity);
	point3 max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				auto x = double(i) * bbox.max().x() + (1.0 - double(i)) * bbox.min().x();
				auto y = double(j) * bbox.max().y() + (1.0 - double(j)) * bbox.min().y();
				auto z = double(k) * bbox.max().z() + (1.0 - double(k)) * bbox.min().z();

				auto newx = cos_theta * x + sin_theta * y;
				auto newy = -sin_theta * x + cos_theta * y;

				vec3 tester(newx, newy, z);

				for (int c = 0; c < 3; c++) {
					min[c] = fmin(min[c], tester[c]);
					max[c] = fmax(max[c], tester[c]);
				}
			}
		}
	}

	bbox = aabb(min, max);
}

bool rotate_z::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	auto origin = r.origin();
	auto direction = r.direction();
	origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[1];
	origin[1] = sin_theta * r.origin()[0] + cos_theta * r.origin()[1];

	direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[1];
	direction[1] = sin_theta * r.direction()[0] + cos_theta * r.direction()[1];

	ray rotated_r(origin, direction, r.time());
	if (!ptr->hit(rotated_r, t_min, t_max, rec))
		return false;

	auto p = rec.p;
	auto normal = rec.normal;

	p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[1];
	p[1] = -sin_theta * rec.p[0] + cos_theta * rec.p[1];

	normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[1];
	normal[1] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[1];

	rec.p = p;
	rec.set_face_normal(rotated_r, normal);
	return true;

}


//Rotation x_axis
class rotate_x : public hitable {
public:
	rotate_x(shared_ptr<hitable> p, double angle);

	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec
	)const override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
		output_box = bbox;
		return hasbox;
	}


	shared_ptr<hitable> ptr;
	double sin_theta;
	double cos_theta;
	bool hasbox;
	aabb bbox;
};

rotate_x::rotate_x(shared_ptr<hitable> p, double angle) : ptr(p) {
	auto radians = degrees_to_radians(angle);
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0, 1, bbox);

	point3 min(infinity, infinity, infinity);
	point3 max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				auto x = double(i) * bbox.max().x() + (1.0 - double(i)) * bbox.min().x();
				auto y = double(j) * bbox.max().y() + (1.0 - double(j)) * bbox.min().y();
				auto z = double(k) * bbox.max().z() + (1.0 - double(k)) * bbox.min().z();

				auto newz = cos_theta * y + sin_theta * z;
				auto newy = -sin_theta * y + cos_theta * z;

				vec3 tester(x, newy, newz);

				for (int c = 0; c < 3; c++) {
					min[c] = fmin(min[c], tester[c]);
					max[c] = fmax(max[c], tester[c]);
				}
			}
		}
	}

	bbox = aabb(min, max);
}

bool rotate_x::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	auto origin = r.origin();
	auto direction = r.direction();
	origin[1] = cos_theta * r.origin()[1] - sin_theta * r.origin()[2];
	origin[2] = sin_theta * r.origin()[1] + cos_theta * r.origin()[2];

	direction[1] = cos_theta * r.direction()[1] - sin_theta * r.direction()[2];
	direction[2] = sin_theta * r.direction()[1] + cos_theta * r.direction()[2];

	ray rotated_r(origin, direction, r.time());
	if (!ptr->hit(rotated_r, t_min, t_max, rec))
		return false;

	auto p = rec.p;
	auto normal = rec.normal;

	p[1] = cos_theta * rec.p[1] + sin_theta * rec.p[2];
	p[2] = -sin_theta * rec.p[1] + cos_theta * rec.p[2];

	normal[1] = cos_theta * rec.normal[1] + sin_theta * rec.normal[2];
	normal[2] = -sin_theta * rec.normal[1] + cos_theta * rec.normal[2];

	rec.p = p;
	rec.set_face_normal(rotated_r, normal);
	return true;

}

class scale : public hitable {
public:
	scale(shared_ptr<hitable> p, double scalation) : ptr(p), sc(scalation) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box)const override;

public:
	shared_ptr<hitable> ptr;
	double sc;
};

bool scale::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	ray moved_r(r.origin()*sc, r.direction(), r.time());
	if (!ptr->hit(moved_r, t_min, t_max, rec))
		return false;
	rec.p *= sc;
	rec.set_face_normal(moved_r, rec.normal);
	return true;
}

bool scale::bounding_box(double time0, double time1, aabb& output_box) const {
	if (!ptr->bounding_box(time0, time1, output_box))
		return false;
	output_box = aabb(output_box.min() *sc, output_box.max() *sc);

	return true;
}

#endif

