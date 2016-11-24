/*
 * analysisreport.cpp
 *
 *  Created on: Nov 9, 2016
 *      Author: tom
 */

#include "analysisreport.hpp"

#include <algorithm>

analysis_report::analysis_report() : stack_usage{0},
stack_usage_type{usage_type::undefined},
is_recursive{false}
{
	// TODO Auto-generated constructor stub

}

bool analysis_report::add_function(int function,int call_cost)
{
	if(std::find(functions.begin(), functions.end(), function) != functions.end())
	{
		is_recursive = true;
		return false;
	}
		stack_usage += call_cost;
	functions.push_back(function);
	return true;
}

int analysis_report::get_depth()
{
	return functions.size() - 1;
}
