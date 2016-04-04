#ifndef __OBJ_LOADER__
#define __OBJ_LOADER__

#include <map>
#include "headers.h"

#include "MeshObj.h"

class ObjLoader {
  public:
    ObjLoader();
    ~ObjLoader();
    MeshObj* loadObjFile(std::string fileName, std::string ID = "");
    MeshObj* getMeshObj(std::string ID);
  private:
    std::map<std::string, MeshObj*> mMeshMap;
};

#endif