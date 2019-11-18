#include "interface.h"
#include <math.h> // Can we include math.h?

#define P_NOM 1 // Nominal power; Must determine value
#define J_T 1 // Turbine total inertia; Must determine value
#define R 1 // Rotor radius; Must determine value

#define RHO 1 // Air density; Must determine value
#define PI 3.14159265358979323846 // Replace with math.h's pi

 /*
 Should be constant for particular wind turbine but perhaps can update
 these if necessary?
 */
static float constants[] = {1, 1, 1, 1, 1, 1}, x_constant = 1;
float calculate_power_coefficient (float tip_speed_ratio, float pitch_angle) {
  // WT_PERF by NREL
  float relational_constant = 1 / (tip_speed_ratio + 0.08 * pitch_angle)
  - 0.035 / (1 + pow (pitch_angle, 3));
  return constants[0] * (constants[1] * relational_constant
    - constants[2] * pitch_angle
    - constants[3] * pow (pitch_angle, x)
    - constants[4]) * exp (-constants[5] * relational_constant);

}

float calculate_aerodynamic_torque (float rotor_speed, float pitch_angle,
  float wind_speed) {
  // Section 2.1
  float tip_speed_ratio = rotor_speed * R / wind_speed; // Equation (2)
  return RHO * PI * pow(R, 3)
      * calculate_power_coefficient (tip_speed_ratio, pitch_angle)
      * pow (wind_speed, 2) / 2; // Equation (4)
}

float calculate_generator_torque (float rotor_speed, float electrical_power) {
  // Section 3.2.1
  float power_tracking_error = P_NOM - electrical_power; // Must calc c_0; When deriv + val * c_0 = 0
  // Following for simple quadratic formula to solve equation (15)
  float a_const = 1 / (rotor_speed * J_T);
  float b_const = - calculate_aerodynamic_torque (/* Must calculate variables or collect input*/) / (rotor_speed * J_T)
      - rotor_speed * K_T /* Must calculate K_T */ / (rotor_speed * J_T) - 1;
  float c_const = c_0 * power_tracking_error / rotor_speed;

  return (- b_const + sqrt (pow (b_const, 2) - 4 * a_const * c_const)) / (2 * a_const); // Which value will be used? +/- version?
}

void main(void) {

}
