//
// Created by matteo on 29/04/2021.
//

#include "subsystems/gps.h"
#include "stdlib.h"

#include "state.h"
#include "mcu_periph/sys_time.h"

#include "subsystems/datalink/downlink.h"

#include "impulse_control.h"

#define DEBUG TRUE

uint8_t impulse_use_motor_l = false;
uint8_t impulse_use_motor_r = false;
uint8_t impulse_use_elevon_l = false;
uint8_t impulse_use_elevon_r = false;

float ml_value = 0.;  // [-9600, 9600] pprz actuator value
float mr_value = 0.;
float el_value = 0.;
float er_value = 0.;

// impulse at this azimuth value [degrees]
uint16_t elevon_activate_at = 0;
uint16_t motor_activate_at = 0;

// Turn impulse into step (activate within azimuth arc) [degrees]
float elevon_half_arc = 2;
float motor_half_arc = 2;

float old_t = 0;
float old_r = 0;
double old_gps_yaw = 0;
float filtered_yaw;

float heading_weight = 0.98;

// Debug
#if DEBUG
float send_values[4];
#endif

float complementary_filter() {
  double current_gps_yaw = (double)gps.course / 1e7;  // rad [0, 2pi]
  // First check for optitrack update
  if (current_gps_yaw != old_gps_yaw) {
    old_t = get_sys_time_float();
    old_gps_yaw = current_gps_yaw;
    filtered_yaw = current_gps_yaw;
  } else {
    float t = get_sys_time_float();
    float dt = t - old_t;
    filtered_yaw = filtered_yaw * heading_weight + (1 - heading_weight) * dt * stateGetBodyRates_f()->r;
    old_t = t;
  }
#if DEBUG
  send_values[0] = current_gps_yaw;
  send_values[1] = stateGetBodyRates_f()->r;
  send_values[2] = filtered_yaw;
  DOWNLINK_SEND_PAYLOAD_FLOAT(DefaultChannel, DefaultDevice, 8, send_values);
#endif
  return filtered_yaw;
}

void impulse_control_values(struct fs_landing_t *actuator_values) {
//  float current_yaw = stateGetNedToBodyEulers_f() -> psi;
//  double current_yaw = (double)gps.course / 1e7;  // rad [0, 2pi]
  float current_yaw = complementary_filter();

  float ele_h_arc = RadOfDeg(elevon_half_arc);
  float mot_h_arc = RadOfDeg(motor_half_arc);

//  elevon_activate_at = ((elevon_activate_at % 180) + 180) % 180;
//  motor_activate_at = ((motor_activate_at % 180) + 180) % 180;

  if (impulse_use_elevon_l) {
    float ele_az = RadOfDeg(elevon_activate_at);
    if (((ele_az - ele_h_arc) < current_yaw) && (current_yaw < (ele_az + ele_h_arc))) {
      int32_t elevon_l = (int32_t)(el_value);
      actuator_values->commands[SERVO_S_ELEVON_LEFT] = elevon_l;
    }
  }
  if (impulse_use_elevon_r) {
    float ele_az = RadOfDeg(elevon_activate_at);
    if (((ele_az - ele_h_arc) < current_yaw) && (current_yaw < (ele_az + ele_h_arc))) {
      int32_t elevon_r = (int32_t)(er_value);
      actuator_values->commands[SERVO_S_ELEVON_RIGHT] = elevon_r;
    }
  }
  if (impulse_use_motor_l) {
    float mot_az = RadOfDeg(motor_activate_at);
    if (((mot_az - mot_h_arc) < current_yaw) && (current_yaw < (mot_az + mot_h_arc))) {
      int32_t motor_l = (int32_t)(ml_value);
      actuator_values->commands[SERVO_S_THROTTLE_LEFT] = motor_l;
    }
  }
  if (impulse_use_motor_r) {
    float mot_az = RadOfDeg(motor_activate_at);
    if (((mot_az - mot_h_arc) < current_yaw) && (current_yaw < (mot_az + mot_h_arc))) {
      int32_t motor_r = (int32_t)(mr_value);
      actuator_values->commands[SERVO_S_THROTTLE_RIGHT] = motor_r;
    }
  }
}