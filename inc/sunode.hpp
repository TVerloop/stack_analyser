/**
 * \file sunode.hpp
 *
 */

#ifndef SUNODE_HPP_
#define SUNODE_HPP_

//std includes
#include <string> //std::string

/** all stack usage types */
enum class usage_type
{
	undefined = 1, /**< usage type has not been deducted (yet)*/
	static_usage = 2, /**< usage type is static*/
	dynamic_bounded_usage = 3, /**< usage type is dynamic but bounded*/
	dynamic_usage = 4 /**< usage type is dynamic and can not be trusted*/
};

/**
 * \class cgraph_node
 *
 *
 * \brief a node which contains the stack_usage of a function
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

class su_node
{
public:
	su_node(const std::string & line);

	const static std::string usage_regex;/**< contains the regex to extract the data */
	std::string name; /**< contains the function name */
	std::string file_name; /**< contains the filename in which the function exsists */
	unsigned int usage; /**< contains the usage of the function */
	usage_type su_type; /**< contains the usage type of the function */

	/**
	 *
	 *	\brief converts a usage_type to a string
	 *	\param st the usage_type which has to be converted to a string
   *	\return string version of the usage_type
   *
	 */
	static std::string usage_type_enum_to_string(const usage_type & ut);

	/**
	 *
	 *	\brief converts a string to a usage_type
	 *	\param st the string which has to be converted to a usage_type
   *	\return the usage_type
   *
	 */
	static usage_type string_to_usage_type_enum(const std::string & ut);

};

#endif /* SUNODE_HPP_ */
