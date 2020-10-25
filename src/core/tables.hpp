#ifndef GAS_SRC_CORE_TABLES_HPP_
#define GAS_SRC_CORE_TABLES_HPP_

#include "sql/table.hpp"

using namespace gas;

struct resource {
  MARK_AS_TABLE(resource)
  TABLE_FIELD(id, int)
  TABLE_FIELD(name, std::string)
  TABLE_FIELD(description, std::string)
  TABLE_FIELD(size, int)
  TABLE_FIELD(checksum, int)
  TABLE_FIELD(type, int)
  TABLE_FIELD(data, std::string)
};

struct dev_role {
  MARK_AS_TABLE(dev_role)
  TABLE_FIELD(id, int)
  TABLE_FIELD(role, std::string)
};

#endif //GAS_SRC_CORE_TABLES_HPP_
