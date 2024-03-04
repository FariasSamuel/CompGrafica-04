#pragma once
#pragma once
#ifndef PLANE_H
#define PLANE_H
#include "vec3.h"
#include "hittable.h"
#include "ray.h"

class plane : public  hittable {
public:
    vec3 color;
    vec3 coord;
    vec3 normal;

    plane(vec3 _color, vec3 _coord, vec3 _normal) {
        this->coord = _coord;
        this->color = _color;
        this->normal = _normal;
        std::cout << coord << " " << color << " " << normal;
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hitInfo& rec) const {

        float denom = dot(normal, unit_vector(r.direction()));
        if (denom < 1e-6) {
            vec3 p0l0 = coord - r.origin();
            float t = dot(p0l0, normal) / denom;
            //::cout << coord << " " << color << " " << normal;
            if (t >= 0) {
                rec.color = this->color;
                rec.t = t;
                rec.hitPoint = r.at(rec.t);
                vec3 outward_normal = this->normal;
                rec.set_face_normal(r, outward_normal);
                return true;
            }
            return false;
        }

        return false;
    }
};

#endif