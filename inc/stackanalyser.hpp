/*
 * stackanalyser.hpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

#ifndef STACKANALYSER_HPP_
#define STACKANALYSER_HPP_
#include <vector>
#include <string>
#include <cgraphnode.hpp>
#include <sunode.hpp>
#include <propertypair.hpp>

enum class cgraph_edge_type
{
  call, inlined_call, definition_link
};

class stack_analyser
{
public:
  stack_analyser(const std::vector<std::string> & input_folders);
  stack_analyser(const std::string & input_folders);
  void print_analysis(std::ostream & stream) const;
  int get_max_stackusage(const std::string & function_name) const;
  virtual ~stack_analyser();

private:
  static void get_all_files(std::vector<std::string> & cgraph_files, std::vector<std::string> & su_files, const std::vector<std::string> & paths);
  static void get_all_files(std::vector<std::string> & cgraph_files, std::vector<std::string> & su_files, const std::string & path);
  void get_cgraph_nodes(std::vector<std::string> & cgraph_file_names);
  void get_su_nodes(std::vector<std::string> & su_file_names);

  std::vector<cgraph_node> cgraph_nodes;
  std::vector<su_node> su_nodes;
  std::vector<property_pair<int,int,cgraph_edge_type>> call_edges;
  std::vector<std::pair<int,int>> su_edges;
};

#endif /* STACKANALYSER_HPP_ */
