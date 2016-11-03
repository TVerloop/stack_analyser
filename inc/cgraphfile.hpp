/*
 * cgraphfile.hpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

#ifndef CGRAPHFILE_HPP_
#define CGRAPHFILE_HPP_

#include <vector> //std::vector<T>
#include <string> //std::string
#include <cgraphnode.hpp>
#include <fstream> //std::ifstream


class cgraph_file
{
public:
  cgraph_file(const std::string & file_name);
  virtual ~cgraph_file();
  void get_graph_nodes(std::vector<cgraph_node> & nodes);
private:
  static void get_optimized_symbol_table(const std::vector<std::string> & input, std::vector<std::string> & output);
  static void fill_vector_from_stream(std::vector<std::string> & vector, std::ifstream & stream);
  std::vector<std::string> raw_data;
  std::string file_name;
};

#endif /* CGRAPHFILE_HPP_ */
