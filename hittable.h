#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

struct hitInfo {
    vec3 normal;
    vec3 hitPoint;
    double t;
    bool front_face;
    vec3 color;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hitInfo& rec) const = 0;
};

#endif