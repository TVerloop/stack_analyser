/*
 * sunode.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

//local includes
#include <sunode.hpp>

//boost includes
#include <boost/regex.hpp>

//std includes
#include <stdlib.h>

const std::string su_node::usage_regex =
		"(\\S+):[0-9]+:[0-9]+:(.+)\\t([0-9]+)\\t(\\S+)";

su_node::su_node(const std::string & line) :
		usage
		{ 0 }, su_type
		{ usage_type::undefined }
{
	boost::regex expr(usage_regex);

	std::string::const_iterator begin = line.begin();
	std::string::const_iterator end = line.end();
	boost::match_results<std::string::const_iterator> what;
	if (regex_search(begin, end, what, expr))
	{
		file_name = what[1];
		name = what[2];
		usage = atoi(what[3].str().c_str());
		if (what[4].compare("static") == 0)
			su_type = usage_type::static_usage;
		else if (what[4].compare("dynamic") == 0)
			su_type = usage_type::dynamic_usage;
		else if (what[4].compare("bounded") == 0)
			su_type = usage_type::dynamic_bounded_usage;
		begin = what[0].second;
	}
}

std::string su_node::usage_type_enum_to_string(const usage_type & ut)
{
	switch (ut)
	{
	case usage_type::static_usage:
		return "static_usage";
	case usage_type::dynamic_bounded_usage:
		return "dynamic_bounded_usage";
	case usage_type::dynamic_usage:
		return "dynamic_usage";
	default:
		return "undefined";
	}
}

usage_type su_node::string_to_usage_type_enum(const std::string & ut)
{
	if (ut.compare("static_usage") == 0)
		return usage_type::static_usage;
	else if (ut.compare("dynamic_bounded_usage") == 0)
		return usage_type::dynamic_bounded_usage;
	else if (ut.compare("dynamic_usage") == 0)
		return usage_type::dynamic_usage;
	else
		return usage_type::undefined;
}

