#ifndef MK20_DATASTORE_H
#define MK20_DATASTORE_H

#include "../materials/MaterialView.h"

typedef struct EEData {
  float mk20fw;     // firmware versions
  float espfw;
  float g2fw;
  float headOffset;
  Material material;
} EEData;



class DataStore {
public:
  DataStore();
  ~DataStore();
  void save();
  void setHeadOffset(float val);
  float getHeadOffset();
  Material * getLoadedMaterial();
  void setLoadedMaterial(Material material);

private:
  EEData _data;
};


#endif
