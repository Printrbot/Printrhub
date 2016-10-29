#ifndef UI_BITMAPS_H
#define UI_BITMAPS_H

#include "framework/core/UIBitmap.h"
#include "SD.h"

class UIBitmaps {
public:
  UIBitmaps();
  ~UIBitmaps();
  UIBitmap btn_back = {0,23500,235,50};
  UIBitmap btn_calibrate = {23500,10416,62,84};
  UIBitmap btn_cancel = {33916,23500,235,50};
  UIBitmap btn_cancel_print = {57416,23500,235,50};
  UIBitmap btn_cloud = {80916,10416,62,84};
  UIBitmap btn_delete_project = {91332,23500,235,50};
  UIBitmap btn_done = {114832,23500,235,50};
  UIBitmap btn_down = {138332,11200,112,50};
  UIBitmap btn_exit = {149532,5000,50,50};
  UIBitmap btn_filament = {154532,10416,62,84};
  UIBitmap btn_jobs = {164948,10416,62,84};
  UIBitmap btn_light = {175364,10416,62,84};
  UIBitmap btn_load = {185780,23500,235,50};
  UIBitmap btn_materials = {209280,10416,62,84};
  UIBitmap btn_no = {219696,11200,112,50};
  UIBitmap btn_open = {230896,10200,102,50};
  UIBitmap btn_password = {241096,10416,62,84};
  UIBitmap btn_print = {251512,10200,102,50};
  UIBitmap btn_print_download = {261712,11300,113,50};
  UIBitmap btn_print_start = {273012,11300,113,50};
  UIBitmap btn_save = {284312,23500,235,50};
  UIBitmap btn_settings = {307812,5000,50,50};
  UIBitmap btn_trash = {312812,5000,50,50};
  UIBitmap btn_unload = {317812,23500,235,50};
  UIBitmap btn_up = {341312,11200,112,50};
  UIBitmap btn_update = {352512,10416,62,84};
  UIBitmap btn_upgrade = {362928,23500,235,50};
  UIBitmap btn_wifi = {386428,10416,62,84};
  UIBitmap btn_yes = {396844,11200,112,50};
  UIBitmap downloading_scene = {408044,129600,270,240};
  UIBitmap heating_screen = {537644,129600,270,240};
  UIBitmap hotend = {667244,21780,121,90};
  UIBitmap hotend_offset = {689024,5876,113,26};
  UIBitmap icon_alert = {694900,14792,86,86};
  UIBitmap icon_download = {709692,20000,100,100};
  UIBitmap icon_filament = {729692,20000,100,100};
  UIBitmap icon_load_filmanet = {749692,20000,100,100};
  UIBitmap icon_time = {769692,20000,100,100};
  UIBitmap icon_unload_filament = {789692,20000,100,100};
  UIBitmap job_finish_scene = {809692,56000,200,140};
  UIBitmap load_scene = {865692,69600,240,145};
  UIBitmap scene_empty_project = {935292,129600,270,240};
  UIBitmap sidebar_calibrate = {1064892,19000,50,190};
  UIBitmap sidebar_downloading = {1083892,19000,50,190};
  UIBitmap sidebar_filament = {1102892,19000,50,190};
  UIBitmap sidebar_firmware = {1121892,19000,50,190};
  UIBitmap sidebar_jobs = {1140892,19000,50,190};
  UIBitmap sidebar_password = {1159892,19000,50,190};
  UIBitmap sidebar_printing = {1178892,19000,50,190};
  UIBitmap sidebar_project = {1197892,19000,50,190};
  UIBitmap sidebar_settings = {1216892,19000,50,190};
  UIBitmap sidebar_update = {1235892,19000,50,190};
  UIBitmap sidebar_wifi = {1254892,19000,50,190};
  UIBitmap splash = {1273892,153600,320,240};
  UIBitmap unload_scene = {1427492,69600,240,145};
  UIBitmap upgrade_scene = {1497092,56000,200,140};
  UIBitmap upgrading_scene = {1553092,129600,270,240};
};


#endif
