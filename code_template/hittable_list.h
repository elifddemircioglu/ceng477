#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "ray.h"

#include <vector>

class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> triangles;
    std::vector<shared_ptr<hittable>> meshes;
    std::vector<shared_ptr<hittable>> spheres;

    hittable_list() {}
    

    void clear() { objects.clear(); }

    void add_triangle(shared_ptr<hittable> object) {
        triangles.push_back(object);
    }
    void add_mesh(shared_ptr<hittable> object) {
        meshes.push_back(object);
    }
    void add_spheres(shared_ptr<hittable> object) {
        spheres.push_back(object);
    }

    bool hit(const Ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : triangles) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        for (const auto& object : meshes) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        for (const auto& object : spheres) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }







        return hit_anything;
    }
};

#endif