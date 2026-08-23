#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include "drive.h"
#define PI_F 3.14159265358979323846f

#define WHEEL_RADIUS 0.15f
#define WHEEL_SEPARATION 0.77f
#define MAX_LINEAR_VELOCITY 1.0f
#define MAX_ANGULAR_VELOCITY 2.0f
#define MAX_WHEEL_VELOCITY 10.0f
#define HEADING_GAIN 1.25f

#define TARGET_TOLERANCE 0.10f
#define DRIVE_DT_SECONDS 0.02f
#define MAX_DRIVE_STEPS 6000

/*
* Latitude and longitude are normalized local simulation coordinates measured
* in metres. Latitude is the north axis and longitude is the east axis. The
* differential-drive rover is planar, so altitude is received but not changed.
*/


/* Heading is in radians: zero points east and positive rotation is CCW. */

static float clampf(float value, float minimum, float maximum) {
 if (value < minimum) {
   return minimum;
 }
 if (value > maximum) {
   return maximum;
 }
 return value;
}

static float normalize_angle(float angle) {
 angle = fmodf(angle + PI_F, 2.0f * PI_F);
 if (angle < 0.0f) {
   angle += 2.0f * PI_F;
 }
 return angle - PI_F;
}

static bool coordinate_is_finite(const struct coordinate *coordinate) {
 return coordinate != NULL && isfinite(coordinate->latitude) &&
        isfinite(coordinate->longitude) &&
        isfinite(coordinate->altitude);
}

static bool rover_is_valid(const struct rover_state *rover) {
 return rover != NULL && coordinate_is_finite(&rover->position) &&
        isfinite(rover->heading_rad);
}

static struct wheel_velocity limit_wheel_velocities(
   struct wheel_velocity velocity) {
 const float largest_magnitude =
     fmaxf(fabsf(velocity.left), fabsf(velocity.right));

 if (largest_magnitude > MAX_WHEEL_VELOCITY) {
   const float scale = MAX_WHEEL_VELOCITY / largest_magnitude;
   velocity.left *= scale;
   velocity.right *= scale;
 }

 return velocity;
}

static bool apply_wheel_velocities(struct rover_state *rover,
                                  struct wheel_velocity velocity) {
 if (!rover_is_valid(rover) || !isfinite(velocity.left) ||
     !isfinite(velocity.right) ||
     fabsf(velocity.left) > MAX_WHEEL_VELOCITY ||
     fabsf(velocity.right) > MAX_WHEEL_VELOCITY) {
   return false;
 }

 const float linear_velocity =
     WHEEL_RADIUS * (velocity.left + velocity.right) / 2.0f;
 const float angular_velocity =
     WHEEL_RADIUS * (velocity.right - velocity.left) / WHEEL_SEPARATION;

 rover->heading_rad = normalize_angle(
     rover->heading_rad + angular_velocity * DRIVE_DT_SECONDS);
 rover->position.longitude +=
     linear_velocity * cosf(rover->heading_rad) * DRIVE_DT_SECONDS;
 rover->position.latitude +=
     linear_velocity * sinf(rover->heading_rad) * DRIVE_DT_SECONDS;

 return rover_is_valid(rover);
}

enum drive_status drive_to_target(struct rover_state *rover,
                                 const struct coordinate *target) {
 if (!rover_is_valid(rover) || !coordinate_is_finite(target)) {
   return DRIVE_INVALID_INPUT;
 }

 rover->heading_rad = normalize_angle(rover->heading_rad);

 for (int step = 0; step < MAX_DRIVE_STEPS; step++) {
   const float north_error =
       target->latitude - rover->position.latitude;
   const float east_error =
       target->longitude - rover->position.longitude;
   const float distance = hypotf(north_error, east_error);

   if (distance <= TARGET_TOLERANCE) {
     const struct wheel_velocity stopped = {0.0f, 0.0f};
     if (!apply_wheel_velocities(rover, stopped)) {
       return DRIVE_INVALID_COMMAND;
     }
     return DRIVE_REACHED_TARGET;
   }

   const float target_heading = atan2f(north_error, east_error);
   const float heading_error =
       normalize_angle(target_heading - rover->heading_rad);

   const float angular_velocity =
       clampf(HEADING_GAIN * heading_error, -MAX_ANGULAR_VELOCITY,
              MAX_ANGULAR_VELOCITY);

   /* Slow near the target and stop forward motion while facing away. */
   const float distance_limited_velocity =
       fminf(MAX_LINEAR_VELOCITY, distance);
   const float heading_alignment = fmaxf(0.0f, cosf(heading_error));
   const float linear_velocity =
       distance_limited_velocity * heading_alignment;

   struct wheel_velocity velocity = {
       .left = (linear_velocity -
                angular_velocity * WHEEL_SEPARATION / 2.0f) /
               WHEEL_RADIUS,
       .right = (linear_velocity +
                 angular_velocity * WHEEL_SEPARATION / 2.0f) /
                WHEEL_RADIUS,
   };

   velocity = limit_wheel_velocities(velocity);

   if (!apply_wheel_velocities(rover, velocity)) {
     return DRIVE_INVALID_COMMAND;
   }
 }

 return DRIVE_MAX_STEPS_EXCEEDED;
}
