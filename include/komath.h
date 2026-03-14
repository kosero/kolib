#ifndef KOMATH_H
#define KOMATH_H

float lerp(float a, float b, float t);
float clamp(float value, float min, float max);

typedef struct {
  float x;
  float y;
} Vec2;

Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_mul(Vec2 a, float scalar);
float vec2_length(Vec2 a);
float vec2_length_sq(Vec2 a);
Vec2 vec2_normalize(Vec2 a);
float vec2_dot(Vec2 a, Vec2 b);
float vec2_cross(Vec2 a, Vec2 b);
Vec2 vec2_from_angle(float angle);
float vec2_angle(Vec2 a);
float vec2_distance(Vec2 a, Vec2 b);
float vec2_distance_sq(Vec2 a, Vec2 b);
Vec2 vec2_lerp(Vec2 a, Vec2 b, float t);
Vec2 vec2_clamp(Vec2 a, Vec2 min, Vec2 max);
Vec2 vec2_reflect(Vec2 a, Vec2 normal);
Vec2 vec2_rotate(Vec2 a, float angle);
int vec2_equals(Vec2 a, Vec2 b);
Vec2 vec2_zero(void);
Vec2 vec2_one(void);

typedef struct {
  float x;
  float y;
  float z;
} Vec3;

Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 a, float scalar);
float vec3_length(Vec3 a);
float vec3_length_sq(Vec3 a);
Vec3 vec3_normalize(Vec3 a);
float vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_cross(Vec3 a, Vec3 b);
Vec3 vec3_angle(Vec3 a);
float vec3_angle_x(Vec3 a);
float vec3_angle_y(Vec3 a);
float vec3_distance(Vec3 a, Vec3 b);
float vec3_distance_sq(Vec3 a, Vec3 b);
Vec3 vec3_lerp(Vec3 a, Vec3 b, float t);
Vec3 vec3_clamp(Vec3 a, Vec3 min, Vec3 max);
Vec3 vec3_reflect(Vec3 a, Vec3 normal);
Vec3 vec3_rotate(Vec3 a, float angle);
int vec3_equals(Vec3 a, Vec3 b);
Vec3 vec3_zero(void);
Vec3 vec3_one(void);

typedef struct {
  Vec2 origin;
  Vec2 direction;
} Ray2;

float ray2_intersect_plane(Ray2 ray, Vec2 normal, float d);

#endif // KOMATH_H
