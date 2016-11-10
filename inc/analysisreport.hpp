/**
 * \file analysisreport.hpp
 *
 */

#ifndef ANALYSISREPORT_HPP_
#define ANALYSISREPORT_HPP_

//local includes
#include <sunode.hpp>

//std includes
#include <vector> //std::vector

/**
 * \class analysis_report
 *
 *
 * \brief results of an analysis
 *
 * This class is used to keep track of data during an analysis of a function
 * and afterwards it is used to keep the results.
 *
 *
 * \author Tom Verloop
 *
 * \version 1.0
 *
 *
 * Contact: Tom_Verloop@outlook.com
 *
 * Created on: Nov 9, 2016
 *
 */

class analysis_report
{
public:
	analysis_report();

	/**
	 *
	 *	\brief adds a function to the call path, and adds a optional
	 *	call cost to the stack usage
	 *	\param function the integer which represents a function node.
   *	\param call_cost added to the stack usage.
   *	\return none
   *
	 */
	void add_function(int function,int call_cost = 0);
	/**
	 *
	 *	\brief returns the depth of the path with the highest stack usage
   *	\return none
   *
	 */
	int get_depth();

	int stack_usage;/**< keeps stack usage of function and its callees */

	usage_type stack_usage_type; /**< keeps the type of stack usage */

	bool is_recursive;/**< if true the function or its callees is recursive */

	std::vector<int> functions; /**< keeps the call path of the highest stack usage */
};


#endif /* ANALYSISREPORT_HPP_ */
