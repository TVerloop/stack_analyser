/*
 * cgraphnode.hpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

#ifndef CGRAPHNODE_HPP_
#define CGRAPHNODE_HPP_

#include <string>
#include <vector>
#include <boost/regex.hpp>

enum class symbol_type
{
  undefined, variable, function, definition, analyzed
};

enum class availability_type
{
  undefined, local, not_available, available
};

enum class flag_type
{
  undefined, local, nonfreeing_fn, optimize_size, executed_once
};

class cgraph_node
{
public:

  bool is_function() const;
  bool is_variable() const;
  cgraph_node(const std::vector<std::string> & raw_node_data);
  virtual ~cgraph_node();
  std::string name;
  std::string mangled_name;
  std::vector<symbol_type> type;
  availability_type availability;
  std::vector<flag_type> flags;
  std::vector<std::pair<std::string, bool>> calls;
  std::vector<std::string> raw_data;

  static std::string symbol_type_enum_to_string(const symbol_type & st);
  static symbol_type string_to_symbol_type_enum(const std::string & st);

  static std::string availablility_type_enum_to_string(const availability_type & at);
  static availability_type string_to_availablility_type_enum(const std::string & at);

  static std::string flag_type_enum_to_string(const flag_type & ft);
  static flag_type string_to_flag_type_enum(const std::string & ft);

  const static std::string name_regex;
  const static std::string type_regex;
  const static std::string availability_regex;
  const static std::string flags_regex;
  const static std::string calls_regex;



private:
  void identify_line(const std::string & line);
  void parse_name_line(const std::string & line);
  void parse_type_line(const std::string & line);
  void parse_availablility_line(const std::string & line);
  void parse_flags_line(const std::string & line);
  void parse_calls_line(const std::string & line);

};

#endif /* CGRAPHNODE_HPP_ */
