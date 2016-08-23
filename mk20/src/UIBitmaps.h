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
  UIBitmap btn_filament = {20832,10416,62,84};
  UIBitmap btn_exit = {31248,5000,50,50};
  UIBitmap btn_password = {36248,10416,62,84};
  UIBitmap btn_settings = {46664,5000,50,50};
  UIBitmap btn_trash = {51664,5000,50,50};
  UIBitmap btn_wifi = {56664,10416,62,84};
  UIBitmap btn_update = {67080,10416,62,84};
  UIBitmap btn_yes = {77496,7800,78,50};
  UIBitmap btn_no = {85296,7800,78,50};
  UIBitmap btn_open = {93096,12000,120,50};
  UIBitmap btn_print = {105096,12000,120,50};
  UIBitmap icon_alert = {117096,9216,72,64};
  UIBitmap icon_download = {126312,9216,72,64};
  UIBitmap sidebar_jobs = {135528,19000,50,190};
  UIBitmap sidebar_project = {154528,19000,50,190};
  UIBitmap sidebar_downloading = {173528,19000,50,190};
  UIBitmap sidebar_settings = {192528,19000,50,190};
  UIBitmap splash = {211528,153600,320,240};

};


#endif
