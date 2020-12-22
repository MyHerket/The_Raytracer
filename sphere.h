#pragma once
#ifndef SPHEREH 
#define SPHEREH

#include "hitable.h"
#include "vec3.h"
#include "material.h"

class sphere : public hitable {
public:
    sphere() {}
    sphere(const point3& cen, double r, shared_ptr<material> m, const char* _name = "Example") : name(_name), center(cen), radius(r), mat_ptr(m) {};
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    virtual double pdf_value(const point3& o, const vec3& v) const override;
    virtual vec3 random(const vec3& o) const override; 
    virtual const char* get_name()const override {
        return name;
    }

public:
    const char* name;
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;

private: 
    static void get_sphere_uv(const point3& p, double& u, double& v) {
        //p: a given point on the sphere of radius one, centered at the origin. 
        //u: returned value [0,1] of angle around the Y axis from X=-1 
        //v: returned value [0,1] of angle from Y = -1 to Y = +1
        // <1 0 0> yields <0.50 0.50>         <-1 0 0> yields <0.00 0.50>
        // <0 1 0> yields <0.50 1.00>         <0 -1 0> yields <0.50 0.00>
        // <0 0 1> yields <0.25 0.50>         <0 0 -1> yields <0.75 0.50>

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi; 

        u = phi / (2 * pi); 
        v = theta / pi;
    }
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
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const  {
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}

double sphere::pdf_value(const point3& o, const vec3& v) const {
    hit_record rec; 
    if (!this->hit(ray(o, v), 0.0001, infinity, rec))
        return 0;
    auto cos_theta_max = sqrt(1 - radius * radius / (center - o).lenght_squared());
    auto solid_angle = 2 * pi * (1 - cos_theta_max); 

    return 1 / solid_angle;
}

vec3 sphere::random(const point3& o)const {
    vec3 direction = center - o; 
    auto distance_squared = direction.lenght_squared();
    onb uvw; 
    uvw.build_from_w(direction); 
    return uvw.local(random_to_sphere(radius, distance_squared));
}


class spotlight : public hitable {
public:
    spotlight() {}
    spotlight(const point3& cen, const color& intensity, const vec3& attenuation, const char* _name = "Example") : name(_name), center(cen), mat_ptr(make_shared<diffuse_light_s>(intensity, attenuation, 0.25)) {};
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    virtual double pdf_value(const point3& o, const vec3& v) const override;
    virtual vec3 random(const vec3& o) const override;
    virtual const char* get_name()const override {
        return name;
    }

public:
    const char* name;
    point3 center;
    double radius = 0.25;
    shared_ptr<material> mat_ptr;

private:
    static void get_spotlight_uv(const point3& p, double& u, double& v) {
        //p: a given point on the spotlight of radius one, centered at the origin. 
        //u: returned value [0,1] of angle around the Y axis from X=-1 
        //v: returned value [0,1] of angle from Y = -1 to Y = +1
        // <1 0 0> yields <0.50 0.50>         <-1 0 0> yields <0.00 0.50>
        // <0 1 0> yields <0.50 1.00>         <0 -1 0> yields <0.50 0.00>
        // <0 0 1> yields <0.25 0.50>         <0 0 -1> yields <0.75 0.50>

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
};

bool spotlight::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    double a = r.direction().lenght_squared();
    double half_b = dot(oc, r.direction());
    double c = oc.lenght_squared() - (radius * radius);

    double discriminant = (half_b * half_b) - a * c;
    if (discriminant < 0.0) return false;
    double sqrtd = sqrt(discriminant);

    double root = (-half_b - sqrtd) / a;
    if (!(tmax > root && root > tmin)) {
        return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_spotlight_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool spotlight::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius));
    return true;
}

double spotlight::pdf_value(const point3& o, const vec3& v) const {
    hit_record rec;
    if (!this->hit(ray(o, v), 0.0001, infinity, rec))
        return 0;
    auto cos_theta_max = sqrt(1 - radius * radius / (center - o).lenght_squared());
    auto solid_angle = 2 * pi * (1 - cos_theta_max);

    return 1 / solid_angle;
}

vec3 spotlight::random(const point3& o)const {
    vec3 direction = center - o;
    auto distance_squared = direction.lenght_squared();
    onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius, distance_squared));
}


#endif 