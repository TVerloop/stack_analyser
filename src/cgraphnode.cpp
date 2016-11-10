/*
 * cgraphnode.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

//local include
#include <cgraphnode.hpp>

const std::string cgraph_node::name_regex = "(\\S+) (\\(.+\\)) (\\S+)";
const std::string cgraph_node::type_regex = "(\\S+)";
const std::string cgraph_node::availability_regex = "(\\S+)";
const std::string cgraph_node::flags_regex = "(\\S+)";
const std::string cgraph_node::calls_regex =
		"(\\S+\\/[0-9]+)(?: )?(\\(inlined\\))? (?:\\([0-9]+.[0-9]+ per call\\)?)";

cgraph_node::cgraph_node(const std::vector<std::string> & raw_node_data) : filename{""} ,
		availability
		{ availability_type::undefined }, raw_data(raw_node_data)
{
	parse_name_line(raw_data[0]);
	for (unsigned int i = 1; i < raw_data.size(); i++)
	{
		identify_line(raw_data[i]);
	}
}

std::string cgraph_node::symbol_type_enum_to_string(const symbol_type & st)
{
	switch (st)
	{
	case symbol_type::analyzed:
		return "analyzed";
	case symbol_type::definition:
		return "definition";
	case symbol_type::function:
		return "function";
	case symbol_type::variable:
		return "variable";
	case symbol_type::alias:
		return "alias";
	default:
		return "undefined";
	}
}

symbol_type cgraph_node::string_to_symbol_type_enum(const std::string & st)
{
	if (st.compare("analyzed") == 0)
		return symbol_type::analyzed;
	else if (st.compare("definition") == 0)
		return symbol_type::definition;
	else if (st.compare("alias") == 0)
		return symbol_type::alias;
	else if (st.compare("function") == 0)
		return symbol_type::function;
	else if (st.compare("variable") == 0)
		return symbol_type::variable;
	else
		return symbol_type::undefined;
}

std::string cgraph_node::availablility_type_enum_to_string(
		const availability_type & at)
{
	switch (at)
	{
	case availability_type::local:
		return "local";
	case availability_type::available:
		return "available";
	case availability_type::not_available:
		return "not_available";
	default:
		return "undefined";
	}
}

availability_type cgraph_node::string_to_availablility_type_enum(
		const std::string & at)
{
	if (at.compare("local") == 0)
		return availability_type::local;
	else if (at.compare("available") == 0)
		return availability_type::available;
	else if (at.compare("not_available") == 0)
		return availability_type::not_available;
	else
		return availability_type::undefined;
}

std::string cgraph_node::flag_type_enum_to_string(const flag_type & ft)
{
	switch (ft)
	{
	case flag_type::local:
		return "local";
	case flag_type::body:
		return "body";
	case flag_type::nonfreeing_fn:
		return "nonfreeing_fn";
	case flag_type::optimize_size:
		return "optimize_size";
	case flag_type::executed_once:
		return "executed_once";
	case flag_type::only_called_at_startup:
		return "only_called_at_startup";
	default:
		return "undefined";
	}
}

flag_type cgraph_node::string_to_flag_type_enum(const std::string & ft)
{
	if (ft.compare("local") == 0)
		return flag_type::local;
	else if (ft.compare("body") == 0)
		return flag_type::body;
	else if (ft.compare("nonfreeing_fn") == 0)
		return flag_type::nonfreeing_fn;
	else if (ft.compare("optimize_size") == 0)
		return flag_type::optimize_size;
	else if (ft.compare("executed_once") == 0)
		return flag_type::executed_once;
	else if (ft.compare("only_called_at_startup") == 0)
		return flag_type::only_called_at_startup;
	else
		return flag_type::undefined;
}

void cgraph_node::identify_line(const std::string & line)
{
	if (line.find("  Type: ") != std::string::npos)
	{
		parse_type_line(line.substr(8));
	}
	else if (line.find("  Availability: ") != std::string::npos)
	{
		parse_availablility_line(line.substr(16));
	}
	else if (line.find("  Function flags: ") != std::string::npos)
	{
		parse_flags_line(line.substr(18));
	}
	else if (line.find("  Calls: ") != std::string::npos)
	{
		parse_calls_line(line.substr(9));
	}
}

void cgraph_node::parse_name_line(const std::string & line)
{
	boost::regex expr(name_regex);

	std::string::const_iterator begin = line.begin();
	std::string::const_iterator end = line.end();
	boost::match_results<std::string::const_iterator> what;
	while (regex_search(begin, end, what, expr))
	{
		mangled_name = what[1].str();
		name = what[2].str().substr(1, what[2].str().size() - 2);
		begin = what[0].second;
	}
}

void cgraph_node::parse_type_line(const std::string & line)
{
	boost::regex expr(type_regex);

	std::string::const_iterator begin = line.begin();
	std::string::const_iterator end = line.end();
	boost::match_results<std::string::const_iterator> what;
	while (regex_search(begin, end, what, expr))
	{
		type.push_back(string_to_symbol_type_enum(what[1]));
		begin = what[0].second;
	}
}

void cgraph_node::parse_availablility_line(const std::string & line)
{
	boost::regex expr(availability_regex);

	std::string::const_iterator begin = line.begin();
	std::string::const_iterator end = line.end();
	boost::match_results<std::string::const_iterator> what;
	while (regex_search(begin, end, what, expr))
	{
		availability = string_to_availablility_type_enum(what[1]);
		begin = what[0].second;
	}
}

void cgraph_node::parse_flags_line(const std::string & line)
{
	boost::regex expr(flags_regex);

	std::string::const_iterator begin = line.begin();
	std::string::const_iterator end = line.end();
	boost::match_results<std::string::const_iterator> what;
	while (regex_search(begin, end, what, expr))
	{
		flags.push_back(string_to_flag_type_enum(what[1]));
		begin = what[0].second;
	}
}

void cgraph_node::parse_calls_line(const std::string & line)
{
	boost::regex expr(calls_regex);

	std::string::const_iterator begin = line.begin();
	std::string::const_iterator end = line.end();
	boost::match_results<std::string::const_iterator> what;
	while (regex_search(begin, end, what, expr))
	{
		calls.push_back(
				std::pair<std::string, bool>(what[1].str(),
						what[2].str().length() > 0));
		begin = what[2].second;
	}
}

static bool is_funct(const symbol_type & type)
{
	return type == symbol_type::function;
}

static bool is_var(const symbol_type & type)
{
	return type == symbol_type::variable;
}

bool cgraph_node::is_function() const
{
	return std::find_if(type.begin(), type.end(), is_funct) != type.end();
}

bool cgraph_node::is_variable() const
{
	return std::find_if(type.begin(), type.end(), is_var) != type.end();
}

static bool is_def(const symbol_type & type)
{
	return type == symbol_type::definition;
}

bool cgraph_node::is_definition() const
{
	return std::find_if(type.begin(), type.end(), is_def) != type.end();
}
