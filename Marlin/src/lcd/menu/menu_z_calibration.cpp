/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

//
// Delta Calibrate Menu
//

#include "../../inc/MarlinConfigPre.h"

// #if HAS_MARLINUI_MENU && EITHER(DELTA_CALIBRATION_MENU, DELTA_AUTO_CALIBRATION)

#include "menu_item.h"
#include "../../module/delta.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "menu.h"
// #include "menu_motion.h"

extern void _menu_move_distance(const AxisEnum axis, const screenFunc_t func, const int8_t eindex=active_extruder);

void set_delta_height() {
  if (!ui.wait_for_move) {
    float z_pos = current_position.z;
    delta_height = delta_height - current_position.z;
    
    // LCD_MESSAGE_F("Z-Height has been set");
    // safe_delay(500);  // Display the message for 5000 milliseconds (5 seconds)
    ui.store_settings();

  }
}

void menu_z_calibration() {
  const bool busy = printer_busy();
  START_MENU();

  //
  // ^ Main
  //
  BACK_ITEM(MSG_MAIN);

  //
  // Auto Home
  //
  #if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
    SUBMENU(MSG_HOMING, menu_home);
  #else
    GCODES_ITEM(MSG_AUTO_HOME, G28_STR);
  #endif
  
  //
  // Move Axis
  //
  if (TERN1(DELTA, all_axes_homed())){
      SUBMENU(MSG_MOVE_Z, []{ _menu_move_distance(Z_AXIS, lcd_move_z); });
      ACTION_ITEM(MSG_DELTA_HEIGHT_CALIBRATE, []{ set_delta_height(); });
  }
  
  if (!busy) ACTION_ITEM(MSG_RESTORE_DEFAULTS, ui.reset_settings);

  END_MENU();

}

// Functions from menu_motion.cpp because I cant figure out how to include them here. 
// screenFunc_t _manual_move_func_ptr;

// void _goto_manual_move(const_float_t scale) {
//   ui.defer_status_screen();
//   ui.manual_move.menu_scale = scale;
//   ui.goto_screen(_manual_move_func_ptr);
// }

// void _menu_move_distance(const AxisEnum axis, const screenFunc_t func, const int8_t eindex=active_extruder) {
//   _manual_move_func_ptr = func;
//   START_MENU();
//   if (LCD_HEIGHT >= 4) {
//     switch (axis) {
//       case X_AXIS: STATIC_ITEM(MSG_MOVE_X, SS_DEFAULT|SS_INVERT); break;
//       case Y_AXIS: STATIC_ITEM(MSG_MOVE_Y, SS_DEFAULT|SS_INVERT); break;
//       case Z_AXIS: STATIC_ITEM(MSG_MOVE_Z, SS_DEFAULT|SS_INVERT); break;
//       default:
//         TERN_(MANUAL_E_MOVES_RELATIVE, manual_move_e_origin = current_position.e);
//         STATIC_ITEM(MSG_MOVE_E, SS_DEFAULT|SS_INVERT);
//         break;
//     }
//   }

//   BACK_ITEM(MSG_MOVE_AXIS);
//   if (parser.using_inch_units()) {
//     if (LARGE_AREA_TEST) SUBMENU(MSG_MOVE_1IN, []{ _goto_manual_move(IN_TO_MM(1.000f)); });
//     SUBMENU(MSG_MOVE_01IN,   []{ _goto_manual_move(IN_TO_MM(0.100f)); });
//     SUBMENU(MSG_MOVE_001IN,  []{ _goto_manual_move(IN_TO_MM(0.010f)); });
//     SUBMENU(MSG_MOVE_0001IN, []{ _goto_manual_move(IN_TO_MM(0.001f)); });
//   }
//   else {
//     if (LARGE_AREA_TEST) SUBMENU(MSG_MOVE_100MM, []{ _goto_manual_move(100); });
//     SUBMENU(MSG_MOVE_10MM, []{ _goto_manual_move(10);    });
//     SUBMENU(MSG_MOVE_1MM,  []{ _goto_manual_move( 1);    });
//     SUBMENU(MSG_MOVE_01MM, []{ _goto_manual_move( 0.1f); });
//     if (axis == Z_AXIS && (FINE_MANUAL_MOVE) > 0.0f && (FINE_MANUAL_MOVE) < 0.1f) {
//       // Determine digits needed right of decimal
//       constexpr uint8_t digs = !UNEAR_ZERO((FINE_MANUAL_MOVE) * 1000 - int((FINE_MANUAL_MOVE) * 1000)) ? 4 :
//                                !UNEAR_ZERO((FINE_MANUAL_MOVE) *  100 - int((FINE_MANUAL_MOVE) *  100)) ? 3 : 2;
//       PGM_P const label = GET_TEXT(MSG_MOVE_N_MM);
//       char tmp[strlen_P(label) + 10 + 1], numstr[10];
//       sprintf_P(tmp, label, dtostrf(FINE_MANUAL_MOVE, 1, digs, numstr));
//       #if DISABLED(HAS_GRAPHICAL_TFT)
//         SUBMENU_P(NUL_STR, []{ _goto_manual_move(float(FINE_MANUAL_MOVE)); });
//         MENU_ITEM_ADDON_START(0 + ENABLED(HAS_MARLINUI_HD44780));
//         lcd_put_u8str(tmp);
//         MENU_ITEM_ADDON_END();
//       #else
//         SUBMENU_P(tmp, []{ _goto_manual_move(float(FINE_MANUAL_MOVE)); });
//       #endif
//     }
//   }
//   END_MENU();
// }

