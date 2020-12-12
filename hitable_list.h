#pragma once
#ifndef SPHEREH 
#define SPHEREH

#include "hitable.h"

class sphere : public hitable {
public:
    sphere() {};
	sphere(point3 cen, double r) : center(cen), radius(r) {}; 
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override; 

    point3 center;
    double radius;
};

bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = dot(oc, r.direction());
    auto c = dot(oc, oc) - double(radius * radius);
    auto discriminant = (b * b) -  (a * c);

    if (discriminant > 0.0) {
        double temp = (double(-b) - sqrt(discriminant)) / (a); 
        if (temp < tmax || temp > tmin) {
            rec.t = temp; 
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            return true;
        }
    }
    return false;
}

#endif // !SPHEREH 

#ifndef HITABLELISTH
#define HITABLELISTH

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hitable_list : public hitable {
public: 
    hitable_list() {};
    hitable_list(shared_ptr<hitable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hitable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

    std::vector<shared_ptr<hitable>> objects;
};

bool hitable_list::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    hit_record temp_rec; 
    bool hit_anything = false;
    auto closest_so_far = tmax; 
    for (const auto& object : objects) {
        if (object->hit(r, tmin, closest_so_far, temp_rec)) {
            hit_anything = true; 
            closest_so_far = temp_rec.t; 
            rec = temp_rec;
        }
    }
    return hit_anything;
}
#endif // !HITABLELISTH
