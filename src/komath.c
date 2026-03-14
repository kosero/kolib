#include "komath.h"
#include <math.h>

float lerp(float a, float b, float t) { return a + (b - a) * t; }
float clamp(float value, float min, float max) {
  return fmaxf(min, fminf(value, max));
}

// Vector2
Vec2 vec2_add(Vec2 a, Vec2 b) { return (Vec2){a.x + b.x, a.y + b.y}; }
Vec2 vec2_sub(Vec2 a, Vec2 b) { return (Vec2){a.x - b.x, a.y - b.y}; }
Vec2 vec2_mul(Vec2 a, float scalar) {
  return (Vec2){a.x * scalar, a.y * scalar};
}
float vec2_length(Vec2 a) { return sqrtf(a.x * a.x + a.y * a.y); }
float vec2_length_sq(Vec2 a) { return a.x * a.x + a.y * a.y; }
Vec2 vec2_normalize(Vec2 a) {
  float len = vec2_length(a);
  if (len == 0.0f)
    return (Vec2){0, 0};
  return (Vec2){a.x / len, a.y / len};
}
float vec2_dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
float vec2_cross(Vec2 a, Vec2 b) { return a.x * b.y - a.y * b.x; }
float vec2_distance(Vec2 a, Vec2 b) { return vec2_length(vec2_sub(a, b)); }
float vec2_distance_sq(Vec2 a, Vec2 b) {
  return vec2_length_sq(vec2_sub(a, b));
}
Vec2 vec2_from_angle(float angle) { return (Vec2){cosf(angle), sinf(angle)}; }
float vec2_angle(Vec2 a) { return atan2f(a.y, a.x); }
Vec2 vec2_lerp(Vec2 a, Vec2 b, float t) {
  return (Vec2){lerp(a.x, b.x, t), lerp(a.y, b.y, t)};
}
Vec2 vec2_clamp(Vec2 a, Vec2 min, Vec2 max) {
  return (Vec2){clamp(a.x, min.x, max.x), clamp(a.y, min.y, max.y)};
}
Vec2 vec2_reflect(Vec2 a, Vec2 normal) {
  return vec2_sub(a, vec2_mul(normal, 2 * vec2_dot(a, normal)));
}
Vec2 vec2_rotate(Vec2 a, float angle) {
  float cos = cosf(angle);
  float sin = sinf(angle);
  return (Vec2){a.x * cos - a.y * sin, a.x * sin + a.y * cos};
}
int vec2_equals(Vec2 a, Vec2 b) { return a.x == b.x && a.y == b.y; }
Vec2 vec2_zero(void) { return (Vec2){0, 0}; }
Vec2 vec2_one(void) { return (Vec2){1, 1}; }

// Vector3
Vec3 vec3_add(Vec3 a, Vec3 b) {
  return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}
Vec3 vec3_sub(Vec3 a, Vec3 b) {
  return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}
Vec3 vec3_mul(Vec3 a, float scalar) {
  return (Vec3){a.x * scalar, a.y * scalar, a.z * scalar};
}
float vec3_length(Vec3 a) { return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z); }
float vec3_length_sq(Vec3 a) { return a.x * a.x + a.y * a.y + a.z * a.z; }
Vec3 vec3_normalize(Vec3 a) {
  float len = vec3_length(a);
  if (len == 0.0f)
    return (Vec3){0, 0, 0};
  return (Vec3){a.x / len, a.y / len, a.z / len};
}
float vec3_dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vec3 vec3_cross(Vec3 a, Vec3 b) {
  return (Vec3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x};
}
Vec3 vec3_angle(Vec3 a) {
  return (Vec3){atan2f(a.y, a.x), atan2f(a.z, sqrtf(a.x * a.x + a.y * a.y)),
                0.0f};
}
float vec3_angle_x(Vec3 a) { return atan2f(a.y, a.x); }
float vec3_angle_y(Vec3 a) { return atan2f(a.z, sqrtf(a.x * a.x + a.y * a.y)); }
float vec3_distance(Vec3 a, Vec3 b) { return vec3_length(vec3_sub(a, b)); }
float vec3_distance_sq(Vec3 a, Vec3 b) {
  return vec3_length_sq(vec3_sub(a, b));
}
Vec3 vec3_lerp(Vec3 a, Vec3 b, float t) {
  return (Vec3){lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t)};
}
Vec3 vec3_reflect(Vec3 a, Vec3 normal) {
  return vec3_sub(a, vec3_mul(normal, 2.0f * vec3_dot(a, normal)));
}
int vec3_equals(Vec3 a, Vec3 b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}
Vec3 vec3_zero(void) { return (Vec3){0, 0, 0}; }
Vec3 vec3_one(void) { return (Vec3){1, 1, 1}; }

// ray2
float ray2_intersect_plane(Ray2 ray, Vec2 normal, float d) {
    float denom = vec2_dot(normal, ray.direction);
    if (denom == 0.0f)
        return -1.0f;
    float t = (d - vec2_dot(normal, ray.origin)) / denom;
    return t;
}
