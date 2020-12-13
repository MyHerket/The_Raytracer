#pragma once
/*#ifndef SPHEREH 
#define SPHEREH

#include "hitable.h"
#include "vec3.h"

class sphere : public hitable {
public:
    sphere() {}
    sphere(point3 cen, double r) : center(cen), radius(r) {};
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

public:
    point3 center;
    double radius;
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
    if (root < tmin || tmax < root) {
        root = (-half_b + sqrtd) / a; 
        if (root < tmin || tmax < root)
            return false;
    }

    rec.t = root; 
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}
 */
#ifndef SPHEREH 
#define SPHEREH

#include "hitable.h"

class sphere : public hitable {
public:
    sphere();
    sphere(vec3 cen, double r) : center(cen), radius(r) {};
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

    vec3 center;
    float radius;
};

bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - (radius * radius);
    double discriminant = (b * b) - (a * c);

    if (discriminant > 0.0) {
        float temp = (double(-b) - sqrt(discriminant)) / (a);
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}
#endif // !SPHEREH 