/*
 * sunode.hpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

#ifndef SUNODE_HPP_
#define SUNODE_HPP_

#include <string>

enum class usage_type
{
  undefined, static_usage, dynamic_bounded_usage, dynamic_usage
};

class su_node
{
public:
  su_node(const std::string & line);
  virtual ~su_node();
  const static std::string usage_regex;
  std::string name;
  unsigned int usage;
  usage_type su_type;

  static std::string usage_type_enum_to_string(const usage_type & ut);
  static usage_type string_to_usage_type_enum(const std::string & ut);

};

#endif /* SUNODE_HPP_ */
