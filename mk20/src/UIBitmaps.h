#ifndef UI_BITMAPS_H
#define UI_BITMAPS_H

#include "framework/core/UIBitmap.h"
#include "SD.h"

class UIBitmaps {
public:
  UIBitmaps();
  ~UIBitmaps();
  UIBitmap btn_calibrate = {0,10416,62,84};
  UIBitmap btn_cloud = {10416,10416,62,84};
  UIBitmap btn_done = {20832,10000,100,50};
  UIBitmap btn_exit = {30832,5000,50,50};
  UIBitmap btn_filament = {35832,10416,62,84};
  UIBitmap btn_jobs = {46248,10416,62,84};
  UIBitmap btn_load = {56664,8500,85,50};
  UIBitmap btn_no = {65164,7800,78,50};
  UIBitmap btn_open = {72964,12000,120,50};
  UIBitmap btn_password = {84964,10416,62,84};
  UIBitmap btn_print = {95380,12000,120,50};
  UIBitmap btn_settings = {107380,5000,50,50};
  UIBitmap btn_trash = {112380,5000,50,50};
  UIBitmap btn_unload = {117380,11600,116,50};
  UIBitmap btn_update = {128980,10416,62,84};
  UIBitmap btn_wifi = {139396,10416,62,84};
  UIBitmap btn_yes = {149812,7800,78,50};
  UIBitmap icon_alert = {157612,9216,72,64};
  UIBitmap icon_download = {166828,8192,64,64};
  UIBitmap icon_filament = {175020,20000,100,100};
  UIBitmap icon_load_filmanet = {195020,20000,100,100};
  UIBitmap icon_time = {215020,20000,100,100};
  UIBitmap icon_unload_filament = {235020,20000,100,100};
  UIBitmap sidebar_calibrate = {255020,19000,50,190};
  UIBitmap sidebar_cloud = {274020,19000,50,190};
  UIBitmap sidebar_downloading = {293020,19000,50,190};
  UIBitmap sidebar_filament = {312020,19000,50,190};
  UIBitmap sidebar_jobs = {331020,19000,50,190};
  UIBitmap sidebar_password = {350020,19000,50,190};
  UIBitmap sidebar_project = {369020,19000,50,190};
  UIBitmap sidebar_settings = {388020,19000,50,190};
  UIBitmap sidebar_update = {407020,19000,50,190};
  UIBitmap sidebar_wifi = {426020,19000,50,190};
  UIBitmap splash = {445020,153600,320,240};
};


#endif
