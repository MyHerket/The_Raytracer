#pragma once
#ifndef SPHEREH 
#define SPHEREH

#include "hitable.h"
#include "vec3.h"

class sphere : public hitable {
public:
    sphere() {}
    sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;


public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    vec3 oc = r.origin() - center; 
    double a = r.direction().lenght_squared();
    double half_b = dot(oc, r.direction()); 
    double c = oc.lenght_squared() - (radius * radius); 

    double discriminant = (half_b * half_b) - a * c;
   if (discriminant < 0.0) return false; 
    double sqrtd = sqrt(discriminant); 

    double root = (-half_b - sqrtd) / a; 
    if (!(tmax> root && root > tmin)){
            return false;
    }

    rec.t = root; 
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const  {
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}

#endif 