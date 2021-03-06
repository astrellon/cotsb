#pragma once

#include <SFML/Graphics.hpp>

#include "ray.h"
#include "circle.h"

#include <random>
#include <math.h>
#include <stdint.h>
#include <vector>

namespace cotsb
{
    namespace utils
    {
        class AnglePair
        {
            public:
                inline AnglePair(float to_angle, float delta_angle) :
                    _to_angle(to_angle),
                    _delta_angle(delta_angle)
            {

            }
                inline AnglePair(double to_angle, double delta_angle) :
                    _to_angle(static_cast<float>(to_angle)),
                    _delta_angle(static_cast<float>(delta_angle))
            {

            }
                inline float to_angle() const
                {
                    return _to_angle;
                }
                inline float delta_angle() const
                {
                    return _delta_angle;
                }

            private:
                float _to_angle;
                float _delta_angle;
        };

        class Utils
        {
            public:
                static const float degrees_to_radians;
                static const float radians_to_degrees;
                static const float pi;

                static const uint32_t max_uint;
                static const int32_t max_int;
                static const int32_t min_int;

                static AnglePair calculate_angles(const sf::Vector2f &pos1, const sf::Vector2f &pos2, float orig_angle, float offset_angle); 

                static inline float lerp(float init, float target, float t)
                {
                    return (target - init) * t + init;
                }
                static inline sf::Vector2f lerp(const sf::Vector2f &init, const sf::Vector2f &target, float t)
                {
                    float x = lerp(init.x, target.x, t);
                    float y = lerp(init.y, target.y, t);
                    return sf::Vector2f(x, y);
                }

                static inline float para_lerp(float init, float target, float t)
                {
                    auto x = (t - 1);
                    x = -x*x + 1;
                    return (target - init) * x + init;
                }
                static inline sf::Vector2f para_lerp(const sf::Vector2f &init, const sf::Vector2f &target, float t)
                {
                    float x = para_lerp(init.x, target.x, t);
                    float y = para_lerp(init.y, target.y, t);
                    return sf::Vector2f(x, y);
                }

                static inline float vector_degrees(const sf::Vector2f &vec)
                {
                    return atan2(vec.y, vec.x) * radians_to_degrees;
                }
                static inline float vector_radians(const sf::Vector2f &vec)
                {
                    return atan2(vec.y, vec.x);
                }

                static inline sf::Vector2f degrees_vector(float degrees)
                {
                    auto radians = degrees * degrees_to_radians;
                    return sf::Vector2f(cos(radians), sin(radians));
                }
                static inline sf::Vector2f radians_vector(float radians)
                {
                    return sf::Vector2f(cos(radians), sin(radians));
                }

                static inline float vector_dot(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
                {
                    return vec1.x * vec2.x + vec1.y * vec2.y;
                }
                static inline float vector_length(const sf::Vector2f &vec)
                {
                    return sqrt(vec.x * vec.x + vec.y * vec.y);
                }
                static inline sf::Vector2f vector_unit(const sf::Vector2f &vec)
                {
                    return vec * (1.0f / vector_length(vec));
                }

                static bool ray_circle_intersect(const Ray &ray, const Circle &circle, 
                        sf::Vector2f points[2], sf::Vector2f normals[2]);

                static inline float abs(float value)
                {
                    if (value < 0.0f)
                    {
                        return -value;
                    }
                    return value;
                }

                static inline sf::Vector2f transform_direction(const sf::Transform &trans, const sf::Vector2f &vector)
                {
                    auto matrix = trans.getMatrix();
                    auto x = vector.x * matrix[0] - vector.y * matrix[1];
                    auto y = -vector.x * matrix[4] + vector.y * matrix[5];
                    return sf::Vector2f(x, y);
                }

                static inline float vector_cross_amount(const sf::Vector2f &vec1, const sf::Vector2f &vec2)
                {
                    return vec1.x * vec2.y - vec1.y * vec2.x;
                }

                static inline float clamp(float input, float min, float max)
                {
                    return input > max ? max : (input < min ? min : input);
                }

                static inline float round(float input)
                {
                    return roundf(input);
                }

            private:
                static std::random_device s_rd;
                static std::mt19937 s_rand;
                static float s_time_since_start;

            public:
                static inline float randf(float min = 0.0f, float max = 1.0f)
                {
                    std::uniform_real_distribution<float> dist(min, max);
                    return dist(s_rand);
                }
                static inline sf::Vector2f rand_vec(float min = 0.0f, float max = 1.0f)
                {
                    return sf::Vector2f(randf(min, max), randf(min, max));
                }

                static inline void update(float dt)
                {
                    s_time_since_start += dt;
                }

                static inline float time_since_start()
                {
                    return s_time_since_start;
                }

        };
                
        template <class T>
            static inline typename std::vector<T>::const_iterator find_vector(
                    const std::vector<T> &vec, T obj)
            {
                for (auto iter = vec.begin(); iter != vec.end(); ++iter)
                {
                        if (*iter == obj)
                        {
                            return iter;
                        }
                    }
                    return vec.end();
                }
    }
}
