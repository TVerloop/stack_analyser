/*
 * stackanalyser.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

#include "stackanalyser.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <ostream>
#include <cgraphfile.hpp>
#include <sufile.hpp>
#include <sunode.hpp>

stack_analyser::stack_analyser(const std::vector<std::string> & input_folders)
{
  std::vector<std::string> su_files;
  std::vector<std::string> cgraph_files;

  get_all_files(cgraph_files, su_files, input_folders);
  get_cgraph_nodes(cgraph_files);
  get_su_nodes(su_files);
}

stack_analyser::stack_analyser(const std::string & input_folders)
{

  std::vector<std::string> su_files;
  std::vector<std::string> cgraph_files;

  get_all_files(cgraph_files, su_files, input_folders);
  get_cgraph_nodes(cgraph_files);

}

stack_analyser::~stack_analyser()
{

}

void stack_analyser::print_analysis(std::ostream & stream) const
{
  for(const su_node & node : su_nodes)
  {
    stream << node.name << " " << node.usage << " " << su_node::usage_type_enum_to_string(node.su_type) << std::endl;
  }
}

int stack_analyser::get_max_stackusage(const std::string & function_name) const
{
  return 0;
}

void stack_analyser::get_all_files(std::vector<std::string> & cgraph_files,
    std::vector<std::string> & su_files, const std::vector<std::string> & paths)
{
  for (const std::string & path_string : paths)
  {
    get_all_files(cgraph_files, su_files, path_string);
  }
}

void stack_analyser::get_all_files(std::vector<std::string> & cgraph_files,
    std::vector<std::string> & su_files, const std::string & path)
{
  boost::filesystem::path p(path);
  boost::filesystem::directory_iterator end_itr;
  for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
  {
    if (is_regular_file(itr->path()))
    {
      if (itr->path().string().find(".000i.cgraph") != std::string::npos)
      {
        cgraph_files.push_back(itr->path().string());
      }
      else if (itr->path().string().find(".su") != std::string::npos)
      {
        su_files.push_back(itr->path().string());
      }
    }
  }
}

void stack_analyser::get_cgraph_nodes(
    std::vector<std::string> & cgraph_file_names)
{
  for (const std::string & file_name : cgraph_file_names)
  {
    cgraph_file f(file_name);
    f.get_graph_nodes(cgraph_nodes);
  }
}

void stack_analyser::get_su_nodes(std::vector<std::string> & su_file_names)
{
  for (const std::string & file_name : su_file_names)
  {
    su_file f(file_name);
    f.get_su_nodes(su_nodes);
  }
}
