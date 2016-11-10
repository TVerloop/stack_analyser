/**
 * \file cgraphnode.hpp
 *
 */

#ifndef CGRAPHNODE_HPP_
#define CGRAPHNODE_HPP_

//local includes
#include <sunode.hpp>

//boost includes
#include <boost/regex.hpp>

//std includes
#include <string> //std::string
#include <vector> //std::vector<T>

/** all types a cgraph_node can be */
enum class symbol_type
{
	undefined, /**< symbol type has not been deducted (yet)*/
	variable, /**< symbol is a variable*/
	function,  /**< symbol is a function*/
	definition,  /**< symbol is defined in current unit*/
	analyzed,  /**< symbol is analyzed*/
	alias  /**< symbol is an alias*/
};

/** all availablilities a cgraph_node can be */
enum class availability_type
{
	undefined, /**< symbol availability has not been deducted (yet)*/
	local,  /**< symbol availability is local*/
	not_available,  /**< symbol is not available */
	available  /**< symbol is available */
};

/** all flags a cgraph_node can have */
enum class flag_type
{
	undefined, /**< a unknown flag has been found*/
	local, /**< flag local is set*/
	nonfreeing_fn, /**< flag nonfreeing_fn is set*/
	optimize_size, /**< flag optimize_size is set*/
	executed_once, /**< flag executed_once is set*/
	body, /**< flag body is set*/
	only_called_at_startup /**< flag only_called_at_startup is set*/
};

/**
 * \class cgraph_node
 *
 *
 * \brief a node which contains the data of a symbol
 *
 * This class contains most data of a symbol
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

class cgraph_node
{
public:

	cgraph_node(const std::vector<std::string> & raw_node_data);

	/**
	 *
	 *	\brief returns true if the symbol is a function
   *	\return true if function
   *
	 */
	bool is_function() const;

	/**
	 *
	 *	\brief returns true if the symbol is a variable
   *	\return true if variable
   *
	 */
	bool is_variable() const;

	/**
	 *
	 *	\brief returns true if the symbol is a defined function
   *	\return true if  defined
   *
	 */
	bool is_definition() const;

	std::string filename; /**< contains the filename in which the symbol exsists */
	std::string name; /**< contains the symbol name */
	std::string mangled_name; /**< contains the mangled name */
	std::vector<symbol_type> type; /**< contains all types that this symbol contains */
	availability_type availability; /**< contains the availability that this symbol contains */
	std::vector<flag_type> flags; /**< contains all flags that this symbol contains */
	std::vector<std::pair<std::string, bool>> calls; /**< contains all calls that this symbol contains */
	std::vector<std::string> raw_data; /**< contains the raw data of the symbol */


	/**
	 *
	 *	\brief converts a symbol_type to a string
	 *	\param st the symbol_type which has to be converted to a string
   *	\return string version of the symbol_type
   *
	 */
	static std::string symbol_type_enum_to_string(const symbol_type & st);

	/**
	 *
	 *	\brief converts a string to a symbol_type
	 *	\param st the string which has to be converted to a symbol_type
   *	\return the symbol_type
   *
	 */
	static symbol_type string_to_symbol_type_enum(const std::string & st);

	/**
	 *
	 *	\brief converts a availablility_type to a string
	 *	\param st the availablility_type which has to be converted to a string
   *	\return string version of the availablility_type
   *
	 */
	static std::string availablility_type_enum_to_string(
			const availability_type & at);

	/**
	 *
	 *	\brief converts a string to a availablility_type
	 *	\param st the string which has to be converted to a availablility_type
   *	\return the availablility_type
   *
	 */
	static availability_type string_to_availablility_type_enum(
			const std::string & at);

	/**
	 *
	 *	\brief converts a flag_type to a string
	 *	\param st the flag_type which has to be converted to a string
   *	\return string version of the flag_type
   *
	 */
	static std::string flag_type_enum_to_string(const flag_type & ft);

	/**
	 *
	 *	\brief converts a string to a flag_type
	 *	\param st the string which has to be converted to a flag_type
   *	\return the flag_type
   *
	 */
	static flag_type string_to_flag_type_enum(const std::string & ft);

	const static std::string name_regex;/**< contains the regex to extract the names */
	const static std::string type_regex;/**< contains the regex to extract the types */
	const static std::string availability_regex;/**< contains the regex to extract the availability */
	const static std::string flags_regex;/**< contains the regex to extract the flags */
	const static std::string calls_regex;/**< contains the regex to extract the calls */

private:

	/**
	 *
	 *	\brief identifies the content of a line and calls the proper parsing function
	 *	\param line contains the line
   *	\return none
   *
	 */
	void identify_line(const std::string & line);

	/**
	 *
	 *	\brief parses the names from a line
	 *	\param line contains the line
   *	\return none
   *
	 */
	void parse_name_line(const std::string & line);

	/**
	 *
	 *	\brief parses the types from a line
	 *	\param line contains the line
   *	\return none
   *
	 */
	void parse_type_line(const std::string & line);

	/**
	 *
	 *	\brief parses the availability from a line
	 *	\param line contains the line
   *	\return none
   *
	 */
	void parse_availablility_line(const std::string & line);

	/**
	 *
	 *	\brief parses the flags from a line
	 *	\param line contains the line
   *	\return none
   *
	 */
	void parse_flags_line(const std::string & line);

	/**
	 *
	 *	\brief calls the flags from a line
	 *	\param line contains the line
   *	\return none
   *
	 */
	void parse_calls_line(const std::string & line);

};

#endif /* CGRAPHNODE_HPP_ */
