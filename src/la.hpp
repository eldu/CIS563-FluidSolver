#pragma once

/** Linear Algebra Library
 *  Imported from base code from CIS 277 at the University of Pennsylvania
 *  Credits due to most probably to Adam Mally and Debanshu Singh
*/

//// NOTE: This #define forces GLM to use radians (not degrees) for ALL of its
//// angle arguments. The documentation may not always reflect this fact.
#define GLM_FORCE_RADIANS

#include <stdio.h>
#include <string>
#include <istream>

static const float PI = 3.14159265358979323846f;
static const float TWO_PI = 2 * PI;
static const float INV_PI = 1.0f / PI;
static const float DEG2RAD = PI / 180.f;
static const float RAD2DEG = 180.f / PI;
static const float GRAVITY = 9.80665f;

/////// 277 linear algebra namespace
//namespace la
//{
//using namespace glm;
//}

/// Float approximate-equality comparison
template<typename T>
inline bool fequal(T a, T b, T epsilon = 0.0001){
    if (a == b) {
        // Shortcut
        return true;
    }

    const T diff = std::abs(a - b);
    if (a * b == 0) {
        // a or b or both are zero; relative error is not meaningful here
        return diff < (epsilon * epsilon);
    }

    return diff / (std::abs(a) + std::abs(b)) < epsilon;
}
