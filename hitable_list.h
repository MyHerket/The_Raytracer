#pragma once
#ifndef SPHEREH 
#define SPHEREH

#include "hitable.h"

class sphere : public hitable {
public:
	sphere(); 
	sphere(vec3 cen, float r) : center(cen), radius(r) {}; 
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const; 

    vec3 center;
    float radius;
};

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - (radius * radius);
    float discriminant = (b * b) -  (a * c);

    if (discriminant > 0.0) {
        float temp = (double(-b) - sqrt(discriminant)) / (a); 
        if (temp < tmax && temp > tmin) {
            rec.t = temp; 
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius; 
            return true;
        }
    }
    return false;
}

#endif // !SPHEREH 

#ifndef HITABLELISTH
#define HITABLELISTH

class hitable_list : public hitable {
public: 
    hitable** list; 
    int list_size;


    hitable_list();
    hitable_list(hitable** l, int n) { list = l; list_size = n; }
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

};

bool hitable_list::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    hit_record temp_rec; 
    bool hit_anything = false;
    double closest_so_far = tmax; 
    for (int i = 0; i < list_size; i++) {
        if (list[i]->hit(r, tmin, closest_so_far, temp_rec)) {
            hit_anything = true; 
            closest_so_far = temp_rec.t; 
            rec = temp_rec;
        }
    }
    return hit_anything;
}
#endif // !HITABLELISTH
