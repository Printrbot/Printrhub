#include <Arduino.h>
#include "Configuration.h"
#include "autolevel.h"
#include "printer.h"
#include "stepper.h"
#include "planner.h"

Autolevel::Autolevel() {
}

void Autolevel::setupForEndstopMove() {
  Printer::saved_feedrate = Printer::feedrate;
  Printer::saved_feed_multiply = Printer::feed_multiply;
  Printer::feed_multiply = 100;
  Printer::last_cmd_time = millis();
  enable_endstops(true);
}

void Autolevel::cleanupAfterEndstopMove() {
  Printer::feedrate = Printer::saved_feedrate;
  Printer::feed_multiply = Printer::saved_feed_multiply;
  Printer::last_cmd_time = millis();
}

void Autolevel::runZProbe() {
  plan_bed_level_matrix.set_to_identity();
  Printer::feedrate = Printer::homing_feedrate[Z_AXIS];

  // move down until you find the bed
  float zPosition = -10;
  plan_buffer_line(Printer::current_position[X_AXIS],
                   Printer::current_position[Y_AXIS],
                   zPosition,
                   Printer::current_position[E_AXIS],
                   Printer::feedrate/60,
                   Printer::active_extruder);
  st_synchronize();

  // we have to let the planner know where we are right now as it is not where we said to go.
  zPosition = st_get_position_mm(Z_AXIS);
  plan_set_position(Printer::current_position[X_AXIS],
                    Printer::current_position[Y_AXIS],
                    zPosition,
                    Printer::current_position[E_AXIS]);

  // move up the retract distance
  zPosition += Z_HOME_RETRACT_MM;
  plan_buffer_line(Printer::current_position[X_AXIS],
                   Printer::current_position[Y_AXIS],
                   zPosition,
                   Printer::current_position[E_AXIS],
                   Printer::feedrate/60,
                   Printer::active_extruder);
  st_synchronize();

  // move back down slowly to find bed
  Printer::feedrate = Printer::homing_feedrate[Z_AXIS]/4;
  zPosition -= Z_HOME_RETRACT_MM * 2;
  plan_buffer_line(Printer::current_position[X_AXIS],
                   Printer::current_position[Y_AXIS],
                   zPosition,
                   Printer::current_position[E_AXIS],
                   Printer::feedrate/60,
                   Printer::active_extruder);
  st_synchronize();

  Printer::current_position[Z_AXIS] = st_get_position_mm(Z_AXIS);
  // make sure the planner knows where we are as it may be a bit different than we last said to move to
  plan_set_position(Printer::current_position[X_AXIS],
                    Printer::current_position[Y_AXIS],
                    Printer::current_position[Z_AXIS],
                    Printer::current_position[E_AXIS]);
}


void Autolevel::doBlockingMove(float x, float y, float z) {
    float oldFeedRate = Printer::feedrate;
    Printer::feedrate = XY_TRAVEL_SPEED;
    Printer::current_position[X_AXIS] = x;
    Printer::current_position[Y_AXIS] = y;
    Printer::current_position[Z_AXIS] = z;
    plan_buffer_line(Printer::current_position[X_AXIS],
                     Printer::current_position[Y_AXIS],
                     Printer::current_position[Z_AXIS],
                     Printer::current_position[E_AXIS],
                     Printer::feedrate/60,
                     Printer::active_extruder);
    st_synchronize();

    Printer::feedrate = oldFeedRate;
}

void Autolevel::run() {

  st_synchronize();
  // make sure the bed_level_rotation_matrix is identity or the planner will get it incorectly

  plan_bed_level_matrix.set_to_identity();
  vector_3 uncorrected_position = plan_get_position();

  Printer::current_position[X_AXIS] = uncorrected_position.x;
  Printer::current_position[Y_AXIS] = uncorrected_position.y;
  Printer::current_position[Z_AXIS] = uncorrected_position.z;

  plan_set_position(Printer::current_position[X_AXIS],
                    Printer::current_position[Y_AXIS],
                    Printer::current_position[Z_AXIS],
                    Printer::current_position[E_AXIS]);

  Autolevel::setupForEndstopMove();

  Printer::feedrate = Printer::homing_feedrate[Z_AXIS];

  // prob 1
  Autolevel::doBlockingMove(Printer::current_position[X_AXIS],
                      Printer::current_position[Y_AXIS],
                      Z_RAISE_BEFORE_PROBING);

  Autolevel::doBlockingMove(LEFT_PROBE_BED_POSITION - Printer::probe_offset[0],
                      BACK_PROBE_BED_POSITION - Printer::probe_offset[1],
                      Printer::current_position[Z_AXIS]);

  Autolevel::runZProbe();
  float z_at_xLeft_yBack = Printer::current_position[Z_AXIS];

  // prob 2
  Autolevel::doBlockingMove(Printer::current_position[X_AXIS],
                      Printer::current_position[Y_AXIS],
                      Printer::current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS);

  Autolevel::doBlockingMove(LEFT_PROBE_BED_POSITION - Printer::probe_offset[0],
                      FRONT_PROBE_BED_POSITION - Printer::probe_offset[1],
                      Printer::current_position[Z_AXIS]);

  Autolevel::runZProbe();
  float z_at_xLeft_yFront = Printer::current_position[Z_AXIS];

  // prob 3
  Autolevel::doBlockingMove(Printer::current_position[X_AXIS],
                      Printer::current_position[Y_AXIS],
                      Printer::current_position[Z_AXIS] + Z_RAISE_BETWEEN_PROBINGS);

  // the current position will be updated by the blocking move so the head will not lower on this next call.
  Autolevel::doBlockingMove(RIGHT_PROBE_BED_POSITION - Printer::probe_offset[0],
                      FRONT_PROBE_BED_POSITION - Printer::probe_offset[1],
                      Printer::current_position[Z_AXIS]);

  Autolevel::runZProbe();
  float z_at_xRight_yFront = Printer::current_position[Z_AXIS];

  Autolevel::cleanupAfterEndstopMove();

  Autolevel::setBedLevelEquation(z_at_xLeft_yFront, z_at_xRight_yFront, z_at_xLeft_yBack);
  st_synchronize();

  // The following code correct the Z height difference from z-probe position and hotend tip position.
  // The Z height on homing is measured by Z-Probe, but the probe is quite far from the hotend.
  // When the bed is uneven, this height must be corrected.
  float real_z = float(st_get_position(Z_AXIS))/axis_steps_per_unit[Z_AXIS];  //get the real Z (since the auto bed leveling is already correcting the plane)
  float x_tmp = Printer::current_position[X_AXIS] + Printer::probe_offset[0];
  float y_tmp = Printer::current_position[Y_AXIS] + Printer::probe_offset[1];
  float z_tmp = Printer::current_position[Z_AXIS];

   //Apply the correction sending the probe offset
  apply_rotation_xyz(plan_bed_level_matrix, x_tmp, y_tmp, z_tmp);
  Printer::current_position[Z_AXIS] = z_tmp - real_z + Printer::current_position[Z_AXIS];   //The difference is added to current position and sent to planner.

  // temp
  plan_bed_level_matrix.set_to_identity();

  plan_set_position(Printer::current_position[X_AXIS],
                    Printer::current_position[Y_AXIS],
                    Printer::current_position[Z_AXIS],
                    Printer::current_position[E_AXIS]);

  Printer::destination[X_AXIS] = Printer::current_position[X_AXIS];
  Printer::destination[Y_AXIS] = Printer::current_position[Y_AXIS];
  Printer::destination[Z_AXIS] = Printer::current_position[Z_AXIS];
  Printer::destination[E_AXIS] = Printer::current_position[E_AXIS];
}


void Autolevel::setBedLevelEquation(float z_at_xLeft_yFront, float z_at_xRight_yFront, float z_at_xLeft_yBack) {
    plan_bed_level_matrix.set_to_identity();

    vector_3 xLeftyFront = vector_3(LEFT_PROBE_BED_POSITION, FRONT_PROBE_BED_POSITION, z_at_xLeft_yFront);
    vector_3 xLeftyBack = vector_3(LEFT_PROBE_BED_POSITION, BACK_PROBE_BED_POSITION, z_at_xLeft_yBack);
    vector_3 xRightyFront = vector_3(RIGHT_PROBE_BED_POSITION, FRONT_PROBE_BED_POSITION, z_at_xRight_yFront);

    vector_3 xPositive = (xRightyFront - xLeftyFront).get_normal();
    vector_3 yPositive = (xLeftyBack - xLeftyFront).get_normal();
    vector_3 planeNormal = vector_3::cross(xPositive, yPositive).get_normal();

    //planeNormal.debug("planeNormal");
    //yPositive.debug("yPositive");
    plan_bed_level_matrix = matrix_3x3::create_look_at(planeNormal);
    //bedLevel.debug("bedLevel");

    //plan_bed_level_matrix.debug("bed level before");
    //vector_3 uncorrected_position = plan_get_position_mm();
    //uncorrected_position.debug("position before");

    // and set our bed level equation to do the right thing
    //plan_bed_level_matrix.debug("bed level after");

    vector_3 corrected_position = plan_get_position();
    //corrected_position.debug("position after");
    Printer::current_position[X_AXIS] = corrected_position.x;
    Printer::current_position[Y_AXIS] = corrected_position.y;
    Printer::current_position[Z_AXIS] = corrected_position.z;

    // but the bed at 0 so we don't go below it.
    Printer::current_position[Z_AXIS] = -Printer::probe_offset[2];

    plan_set_position(Printer::current_position[X_AXIS],
                      Printer::current_position[Y_AXIS],
                      Printer::current_position[Z_AXIS],
                      Printer::current_position[E_AXIS]);
}
