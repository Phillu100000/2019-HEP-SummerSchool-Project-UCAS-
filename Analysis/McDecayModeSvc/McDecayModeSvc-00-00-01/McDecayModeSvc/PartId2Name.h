#ifndef PART_ID_TO_NAME_H
#define PART_ID_TO_NAME_H

#include <map>
#include <string>

class PartId2Name
{
public:
  static PartId2Name* instance(const std::string& pdt_f = "");

  static void release();

  std::string operator()(int id) const;

private:
  PartId2Name(const char* pdt_f);
  PartId2Name();

private:
  std::map<int, std::string> m_pdt;

  static PartId2Name* s_instance;
};

#endif
