/**
 * \file cgraphfile.hpp
 *
 */

#ifndef CGRAPHFILE_HPP_
#define CGRAPHFILE_HPP_

//local includes
#include <cgraphnode.hpp>

//std includes
#include <vector> //std::vector<T>
#include <string> //std::string
#include <fstream> //std::ifstream

/**
 * \class cgraph_file
 *
 *
 * \brief parses a .000i.cgraph files
 *
 * This class is used to extract the optimized symbol table from a .000i.cgraph
 * file and parse this to graph_node(s) while keeping all raw data for debugging
 * purposes
 *
 *
 * \author Tom Verloop
 *
 * \version 1.0
 *
 *
 * Contact: Tom_Verloop@outlook.com
 *
 * Created on: Nov 2, 2016
 *
 */


class cgraph_file
{
public:
	cgraph_file(const std::string & file_name);

	/**
	 *
	 *	\brief returns a list of graph_nodes parsed for the file
	 *	\param nodes a vector for cgraph_nodes which it will use to append its nodes
   *	\return none
   *
	 */
	void get_graph_nodes(std::vector<cgraph_node> & nodes);
private:

	/**
	 *
	 *	\brief extracts the raw optimized symbol table from the file
	 *	\param input string vector containing all lines for the file
	 *	\param output string vector containing all lines for the file
	 *	containing only the optimized symbol table
   *	\return none
   *
	 */
	static void get_optimized_symbol_table(const std::vector<std::string> & input,
			std::vector<std::string> & output);

	/**
	 *
	 *	\brief fills a vector of strings using a file stream
	 *	\param vector the vector to fill with lines
	 *	\param stream the stream to read from
   *	\return none
   *
	 */
	static void fill_vector_from_stream(std::vector<std::string> & vector,
			std::ifstream & stream);

	std::vector<std::string> raw_data; /**< containing the raw data from the file */
	std::string file_name; /**< name of used file */
};

#endif /* CGRAPHFILE_HPP_ */
