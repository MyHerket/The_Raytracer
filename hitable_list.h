#pragma once

#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"
#include "Bounding_Volume.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hitable_list : public hitable {
public: 
    hitable_list() {}
    hitable_list(shared_ptr<hitable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hitable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
    virtual bool bounding_box(
        double time0, double time1, aabb& output_box
    )const override;

public:
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

bool hitable_list::bounding_box(double time0, double time1, aabb& output_box) const {
    if (objects.empty()) return false; 
    aabb temp_box; 
    bool first_box = true; 
    for (const auto& object : objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false; 
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }
    return true;
}

#endif // !HITABLELISTH
