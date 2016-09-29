#ifndef MK20_DATASTORE_H
#define MK20_DATASTORE_H



typedef struct EEData {
  float mk20fw;     // firmware versions
  float espfw;
  float g2fw;
  char material[9]; // loaded material
  float headOffset;
} EEData;



class DataStore {
public:
  DataStore();
  ~DataStore();
  void save();
  void setHeadOffset(float val);
  float getHeadOffset();

private:
  EEData _data;
};


#endif
