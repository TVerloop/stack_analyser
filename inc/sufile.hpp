/*
 * sufile.hpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

#ifndef SUFILE_HPP_
#define SUFILE_HPP_

#include <vector>
#include <string>
#include <sunode.hpp>
#include <fstream> //std::ifstream

class su_file
{
public:
  su_file(const std::string & file_name);
  void get_su_nodes(std::vector<su_node> & nodes);
  virtual ~su_file();
private:
  std::vector<std::string> raw_data;
  void fill_vector_from_stream(std::vector<std::string> & vector, std::ifstream & stream);
  std::string file_name;
};

#endif /* SUFILE_HPP_ */
