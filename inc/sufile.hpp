/**
 * \file sufile.hpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

#ifndef SUFILE_HPP_
#define SUFILE_HPP_


//local includes
#include <sunode.hpp>

//std includes
#include <vector> //std::vector<T>
#include <string> //std::string
#include <fstream> //std::ifstream

/**
 * \class su_file
 *
 *
 * \brief parses a .su files
 *
 * This class is used to extract the stack usage lines .su
 * file and parse this to su_node(s) while keeping all raw data for debugging
 * purposes
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

class su_file
{
public:
	su_file(const std::string & file_name);

	/**
	 *
	 *	\brief returns a list of su_nodes parsed for the file
	 *	\param nodes a vector for cgraph_nodes which it will use to append its nodes
   *	\return none
   *
	 */
	void get_su_nodes(std::vector<su_node> & nodes);
	virtual ~su_file();
private:

	/**
	 *
	 *	\brief fills a vector of strings using a file stream
	 *	\param vector the vector to fill with lines
	 *	\param stream the stream to read from
   *	\return none
   *
	 */
	void fill_vector_from_stream(std::vector<std::string> & vector,
			std::ifstream & stream);

	std::vector<std::string> raw_data; /**< containing the raw data from the file */
	std::string file_name; /**< name of used file */
};

#endif /* SUFILE_HPP_ */
