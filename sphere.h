#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include "vec3.h"
#include "hittable.h"
#include "ray.h"

class sphere : public  hittable{
public:
    double rEsfera;
    vec3 esfColor;
    vec3 coord;

    sphere(double _rEsfera, vec3 _esfColor, vec3 _coord) {
        this->coord = _coord;
        this->rEsfera = _rEsfera;
        this->esfColor = _esfColor;
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hitInfo& rec) const {
        vec3 oc = r.origin() - this->coord;
        double a = r.direction().length_squared();
        double b = dot(r.direction(), oc);
        double c = oc.length_squared() - this->rEsfera * this->rEsfera;

        auto discriminant = b * b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        auto root = (-b - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (-b + sqrtd) / a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }
        rec.color = this->esfColor;
        rec.t = root;
        rec.hitPoint = r.at(rec.t);
        //rec.normal = (rec.hitPoint - esfera.coord) ;
        vec3 outward_normal = (rec.hitPoint - this->coord) / this->rEsfera;
        rec.set_face_normal(r, outward_normal);
        return true;
    }
};

#endif