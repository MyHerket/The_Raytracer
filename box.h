#pragma once
#ifndef BOX_H

#define BOX_H
#include "rtweekend.h"
#include "aarect.h"
#include "hitable_list.h"

class box : public hitable {
public: 
	box(){}
	box(const char* _name, const point3& p0, const point3& p1, shared_ptr<material> ptr); 

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
		output_box = aabb(box_min, box_max); 
		return true;
	}

public: 
	const char* name;
	point3 box_min; 
	point3 box_max; 
	hitable_list sides;
};

box::box(const char* _name, const point3& p0, const point3& p1, shared_ptr<material> ptr) {
	name = _name;
	box_min = p0; 
	box_max = p1; 

	sides.add(make_shared<xy_rect>(_name, p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
	sides.add(make_shared<xy_rect>(_name, p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
								
	sides.add(make_shared<xz_rect>(_name, p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
	sides.add(make_shared<xz_rect>(_name, p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
							
	sides.add(make_shared<yz_rect>(_name, p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
	sides.add(make_shared<yz_rect>(_name, p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool box::hit(const ray& r, double t_min, double t_max, hit_record& rec)const {
	return sides.hit(r, t_min, t_max, rec);
}
#endif // !BOX_H
