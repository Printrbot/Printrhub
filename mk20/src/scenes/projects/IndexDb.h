/*
 * A class holding information about an index file keeping track of various
 * things
 *
 * More Info and documentation:
 * http://www.appfruits.com/2016/11/behind-the-scenes-printrbot-simple-2016/
 *
 * Copyright (c) 2016 Printrbot Inc.
 * Author: Mick Balaban
 * https://github.com/Printrbot/Printrhub
 *
 * Developed in cooperation with Phillip Schuster (@appfruits) from appfruits.com
 * http://www.appfruits.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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
