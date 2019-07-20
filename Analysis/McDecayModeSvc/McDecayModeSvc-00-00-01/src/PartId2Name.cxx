#include <unistd.h>
#include <iostream>
#include <fstream>
#include "McDecayModeSvc/PartId2Name.h"

PartId2Name* PartId2Name::s_instance = NULL;

PartId2Name* PartId2Name::instance(const std::string& pdt_f) {

  if ( s_instance == NULL && !pdt_f.empty() ) {
    s_instance = new PartId2Name(pdt_f.c_str());
  }

  return s_instance;
}

void PartId2Name::release() {
   if ( s_instance != NULL ) {
      delete s_instance;
      s_instance = NULL;
   }
}

std::string PartId2Name::operator()(int id) const {

  std::map<int, std::string>::const_iterator it = m_pdt.find(id);

  if (it != m_pdt.end()) return it->second;

  return "Unknown";
}

PartId2Name::PartId2Name(const char* pdt_f)
{
  if ( access(pdt_f, F_OK) < 0 ) {
    std::cerr << "Can't find the particle-defination-table file: " << pdt_f << std::endl;
    return;
  }

  std::string opt, p, type, name;
  int id;

  std::ifstream fs(pdt_f);

  char buf[128];
  while ( fs.good() && !fs.eof() ) {
    fs >> opt;
    if (opt == "add") {
      fs >> p >> type >> name >> id;
      m_pdt.insert(make_pair(id, name));
    }
    fs.getline(buf, 128);
  }
}
