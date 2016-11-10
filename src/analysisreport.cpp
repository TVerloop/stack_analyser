/*
 * analysisreport.cpp
 *
 *  Created on: Nov 9, 2016
 *      Author: tom
 */

#include "analysisreport.hpp"

analysis_report::analysis_report() : stack_usage{0},
stack_usage_type{usage_type::undefined},
is_recursive{false}
{
	// TODO Auto-generated constructor stub

}

void analysis_report::add_function(int function,int call_cost)
{
	if(functions.size() != 0)
		stack_usage += call_cost;
	functions.push_back(function);
}

int analysis_report::get_depth()
{
	return functions.size() - 1;
}
