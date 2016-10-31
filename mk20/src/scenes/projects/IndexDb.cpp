//
// Created by Mick Balaban on 10/27/16.
//

#include "IndexDb.h"
#include "framework/core/StackArray.h"
#include "framework/core/MemoryStream.h"

IndexDb::IndexDb() {
  this->_iDBInfo = (IndexDbInfo*) malloc(sizeof(IndexDbInfo));
  //SD.remove(indexFileName);
  // if there is no index file, create it
  if (!SD.exists(IndexDb::indexFileName)) {
    // create file
    _indexFile = SD.open(indexFileName, FILE_WRITE);
    // check if project folder exists, if not create one
    _iDBInfo->rev = 0;
    _iDBInfo->totalProjectFiles = 0;
    _indexFile.write((uint8_t *) &_iDBInfo, sizeof(IndexDbInfo));

    if (!SD.exists(projectFolderName)) {
      SD.mkdir(projectFolderName);
    } else {
      // since there is a project folder, check if there are
      // any files in it already
      File pdir = SD.open(projectFolderName);
      pdir.rewindDirectory();
      while(true) {
        File pfile = pdir.openNextFile();
        if (!pfile) break;
        if (pfile.isDirectory()) continue;
        // add project name to index
        String fn = pfile.name();
        _indexFile.write(fn.c_str(), 9);
        _iDBInfo->totalProjectFiles++;
      };
    }
    _indexFile.seek(0);
    _indexFile.write((uint8_t) 0);
    _indexFile.write((uint8_t)_iDBInfo->totalProjectFiles);
    _indexFile.close();
  }

  _indexFile = SD.open(IndexDb::indexFileName, FILE_READ);
  _indexFile.read(&_iDBInfo->rev, 1);
  _indexFile.read(&_iDBInfo->totalProjectFiles, 1);
}

uint8_t IndexDb::getTotalProjects() {
  return _iDBInfo->totalProjectFiles;
}

IndexDb::~IndexDb() {
  _indexFile.close();
  free (_iDBInfo);
  _iDBInfo = NULL;
}

Project * IndexDb::getProjectAt(uint16_t idx) {
  Serial2.println(sizeof(IndexDbInfo) + (idx * 9));
  _indexFile.seek(sizeof(IndexDbInfo) + (idx * 9));
  Project * p = (Project*) malloc(sizeof(Project));
  String path = _indexFile.readString((size_t) 9);
  Serial2.println(path);

  path = IndexDb::projectFolderName + path;
  File pf = SD.open(path.c_str(), FILE_READ);
  pf.seek(32);
  pf.read(p, sizeof(Project));
  pf.close();
  return p;
}

void IndexDb::addProjectFile(String fileName) {
  // increment totalProjectFiles
  MemoryStream * m = new MemoryStream(9);
  m->write((uint8_t)_iDBInfo->rev);
  m->write((uint8_t)_iDBInfo->totalProjectFiles+1);
  m->print(fileName);
  m->print('\0');

  for (uint8_t i=0; i<_iDBInfo->totalProjectFiles; i++) {
    _indexFile.seek((i * 9)+2);
    char pn[9];
    _indexFile.read(&pn, 9);
    m->print(pn);
    m->print('\0');
  }
  _indexFile.close();
  SD.remove(indexFileName);
  _indexFile = SD.open(indexFileName, FILE_WRITE);
  while (m->available()) {
    byte b = m->read();
    _indexFile.write(b);
  }
  delete m;
}

void IndexDb::deleteProject(Project p) {

  MemoryStream * m = new MemoryStream(9);
  m->write((uint8_t)_iDBInfo->rev);
  m->write((uint8_t)_iDBInfo->totalProjectFiles-1);
  // write filtered project array to memory
  for (uint8_t i=0; i<_iDBInfo->totalProjectFiles; i++) {
    _indexFile.seek((i * 9)+2);
    String _idx = _indexFile.readString(9);
    _idx.toUpperCase();
    String _idx2 = p.index;
    _idx2.toUpperCase();

    if (!_idx.equals(_idx2)) {
      m->print(_idx);
      m->print('\0');
    }
  }

  // create fresh index file
  _indexFile.close();
  SD.remove(indexFileName);
  _indexFile = SD.open(indexFileName, FILE_WRITE);

  while (m->available()) {
    byte b = m->read();
    _indexFile.write(b);
  }
  delete m;
  _indexFile.close();
  _indexFile = SD.open(indexFileName, FILE_READ);

  // delete files
  String path = String(projectFolderName) + p.index;
  SD.remove(path.c_str());
  // remove job directory and all files in it
  String jpath = String(jobsFolderName) + p.index;
  if (SD.exists(jpath.c_str())) {
    File jobsFolder = SD.open(jpath.c_str());
    while (true) {
      File jf = jobsFolder.openNextFile();
      if (!jf) {
        break;
      }
      String jfn = jpath + String("/");
      jfn = jfn + String(jf.name());
      jf.close();
      SD.remove(jfn.c_str());
    }
    // remove now empty directory
    SD.rmdir(jpath.c_str());
  }
}