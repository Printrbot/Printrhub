//
// Created by Mick Balaban on 10/27/16.
//

#ifndef MK20_INDEX_H
#define MK20_INDEX_H

#include "framework/core/StackArray.h"
#include "SD.h"

typedef struct IndexDbInfo {
  uint8_t rev;
  uint8_t totalProjectFiles;
} IndexDbInfo;

typedef struct Project {
  char index[9];
  uint8_t rev;
  char title[32];
  uint8_t jobs;
} Project;

class IndexDb {
public:
  IndexDb();
  ~IndexDb();
  static constexpr char * indexFileName = "/index";
  static constexpr char * projectFolderName = "/projects/";
  static constexpr char * jobsFolderName = "/jobs/";
  uint8_t getTotalProjects();
  Project * getProjectAt(uint16_t idx);
  String getProjectFilePath(char * projectFileName);
  void deleteProject(Project p);
  void addProjectFile(String fileName);
private:
  File _indexFile;
  const char * tempIndexFileName = "/index.tmp";
  IndexDbInfo * _iDBInfo;
};


#endif //MK20_INDEX_H
