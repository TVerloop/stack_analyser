/*
 * stackanalyser.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

//local includes
#include <stackanalyser.hpp>
#include <cgraphfile.hpp>
#include <propertypair.hpp>
#include <sufile.hpp>
#include <sunode.hpp>
#include <analysisreport.hpp>

//boost includes
#include <boost/filesystem.hpp>

//std includes
#include <iostream> //std::cout
#include <ostream>  //std::ostream
#include <algorithm> //std::sort

stack_analyser::stack_analyser(const std::vector<std::string> & input_folders,int call_cost) : call_cost{call_cost}
{
	std::vector<std::string> su_files;
	std::vector<std::string> cgraph_files;

	get_all_files(cgraph_files, su_files, input_folders);
	get_cgraph_nodes(cgraph_files);
	get_su_nodes(su_files);
	create_call_graph();
}

stack_analyser::stack_analyser(const std::string & input_folders)
{

	std::vector<std::string> su_files;
	std::vector<std::string> cgraph_files;

	get_all_files(cgraph_files, su_files, input_folders);
	get_cgraph_nodes(cgraph_files);

}

usage_type merge_usage_type(usage_type lhs, usage_type rhs)
{
	if (lhs >= rhs)
		return lhs;
	else
		return rhs;
}

analysis_report stack_analyser::analyse_function(int function,
		analysis_report report, bool inlined)
{
	report.add_function(function, call_cost);
	for (const auto & edge : su_edges)
	{
		if (edge.second == function)
		{
			report.stack_usage += su_nodes[edge.first].usage;
			report.stack_usage_type = merge_usage_type(report.stack_usage_type,
					su_nodes[edge.first].su_type);
			break;
		}
	}
	std::vector<analysis_report> reports;
	for (const auto & edge : call_edges)
	{
		if (edge.first == function)
		{
			reports.push_back(
					analyse_function(int(edge.second), report, bool(edge.property)));
		}
	}
	for (const analysis_report & r : reports)
	{
		if (r.stack_usage > report.stack_usage)
			report = r;
	}
	return report;
}

bool sort_reports(const analysis_report & lhs, const analysis_report & rhs)
{
	return lhs.stack_usage > rhs.stack_usage;
}

void stack_analyser::print_analysis(std::ostream & stream)
{
	std::vector<analysis_report> reports;
	for (unsigned int i = 0; i < functions.size(); i++)
	{
		if (functions[i].is_definition())
		{
			reports.push_back(analyse_function(i));
		}
	}

	std::sort(reports.begin(), reports.end(), &sort_reports);
	for (const auto & report : reports)
	{
		stream << functions[report.functions[0]].name << "\t" << report.stack_usage
				<< "\t" << su_node::usage_type_enum_to_string(report.stack_usage_type)
				<< std::endl;

	}
}

std::string int_to_graph_label(int i)
{
	std::string str;
	while (i > 25)
	{
		i -= 25;
		str = str + 'Z';
	}
	str += char((int(i) + int('A')));
	return str;
}

void stack_analyser::print_callgraph_dot(std::ostream & stream)
{
	stream << "graph callgraph {\n";
	for (unsigned int i = 0; i < functions.size(); i++)
	{
		auto report = analyse_function(i);
		stream << int_to_graph_label(i) << " [label=\"" << functions[i].filename
				<< "\n" << functions[i].name << "\n" << report.stack_usage << "\"];" << std::endl;
	}
	for (unsigned int i = 0; i < call_edges.size(); i++)
	{
		stream << int_to_graph_label(call_edges[i].first) << " -- "
				<< int_to_graph_label(call_edges[i].second);
		switch (call_edges[i].property)
		{
		case cgraph_edge_type::definition_link:
			stream << " [style=dotted][label=\"D\"];";
			break;
		case cgraph_edge_type::inlined_call:
			stream << " [style=dotted][label=\"I\"];";
			break;
		default:
			stream << ";";
		}

		stream << std::endl;
	}
	stream << std::endl << "}" << std::endl;
}

void stack_analyser::print_symbols(std::ostream & stream)
{
	for (const cgraph_node & node : cgraph_nodes)
	{
		if (node.is_function())
		{
			stream << node.mangled_name << " " << node.name << std::endl;

			std::cout << "type: ";
			for (const symbol_type & flag : node.type)
			{
				std::cout << cgraph_node::symbol_type_enum_to_string(flag) << " ";
			}
			stream << std::endl;

			std::cout << "availability: "
					<< cgraph_node::availablility_type_enum_to_string(node.availability);
			stream << std::endl;

			std::cout << "flags: ";
			for (const flag_type & flag : node.flags)
			{
				std::cout << cgraph_node::flag_type_enum_to_string(flag) << " ";
			}
			stream << std::endl;

			std::cout << "calls: ";
			for (const std::pair<std::string, bool> & pair : node.calls)
			{
				std::cout << pair.first << " " << (pair.second ? "(inlined) " : " ");
			}
			stream << std::endl;
		}
	}
}

void stack_analyser::print_symbols_raw_data(std::ostream & stream)
{
	for (const cgraph_node & node : cgraph_nodes)
	{
		for (const std::string & str : node.raw_data)
			std::cout << str << std::endl;
	}
	std::cout << std::endl;
}

void stack_analyser::print_su_nodes(std::ostream & stream)
{
	for (const su_node & node : su_nodes)
	{
		std::cout << node.file_name << " " << node.name << " " << node.usage << " "
				<< su_node::usage_type_enum_to_string(node.su_type) << std::endl;
	}
	std::cout << std::endl;
}

int stack_analyser::get_max_stackusage(const std::string & function_name)
{
	for(unsigned int i = 0;i < functions.size();i++)
	{
		if(functions[i].name.compare(function_name) == 0)
		{
			analysis_report rep = analyse_function(i);
			return rep.stack_usage;
		}
	}
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

void stack_analyser::create_call_graph()
{
	for (cgraph_node & node : cgraph_nodes)
	{
		if (node.is_function())
			functions.push_back(node);
		else if (node.is_variable())
			variables.push_back(node);
	}
	resolve_unavailable();
	resolve_stack_usage();
	resolve_calls();
	print_unresolved();

}

void stack_analyser::resolve_unavailable()
{
	for (unsigned int i = 0; i < functions.size(); i++)
	{
		if (functions[i].availability == availability_type::not_available)
		{
			bool resolved = false;
			for (unsigned int j = 0; j < functions.size(); j++)
			{
				if (functions[i].name.compare(functions[j].name) == 0
						&& functions[j].is_definition())
				{
					resolved = true;
					property_pair<int, int, cgraph_edge_type> p;
					p.first = static_cast<int>(i);
					p.second = static_cast<int>(j);
					p.property = cgraph_edge_type::definition_link;
					call_edges.push_back(p);
					break;
				}
			}
			if (!resolved)
			{
				unresolved_unavailabe_cgraph_nodes.push_back(static_cast<int>(i));
			}
		}
	}
}

void stack_analyser::resolve_stack_usage()
{
	for (unsigned int i = 0; i < su_nodes.size(); i++)
	{
		bool resolved = false;
		for (unsigned int j = 0; j < functions.size(); j++)
		{
			if (su_nodes[i].name.compare(functions[j].name) == 0
					&& functions[j].is_definition())
			{
				std::pair<int, int> p;
				p.first = static_cast<int>(i);
				p.second = static_cast<int>(j);
				resolved = true;
				su_edges.push_back(p);
			}
		}
		if (!resolved)
			unresolved_su_nodes.push_back(static_cast<int>(i));
	}
}

void stack_analyser::resolve_calls()
{
	for (unsigned int i = 0; i < functions.size(); i++)
	{
		for (unsigned int j = 0; j < functions[i].calls.size(); j++)
		{
			if (!resolve_single_call(i, j))
			{
				std::pair<int, int> p;
				p.first = static_cast<int>(i);
				p.second = static_cast<int>(j);
				unresolved_calls.push_back(p);
			}
		}
	}
}

bool stack_analyser::resolve_single_call(int function_nr, int call_nr)
{
	bool resolved = false;
	for (unsigned int i = 0; i < functions.size(); i++)
	{
		if (functions[i].mangled_name.compare(
				functions[function_nr].calls[call_nr].first) == 0)
		{
			resolved = true;
			property_pair<int, int, cgraph_edge_type> p;
			p.first = function_nr;
			p.second = static_cast<int>(i);
			if (functions[function_nr].calls[call_nr].second)
				p.property = cgraph_edge_type::inlined_call;
			else
				p.property = cgraph_edge_type::call;

			call_edges.push_back(p);
			break;
		}
	}
	return resolved;
}

void stack_analyser::print_unresolved()
{
	if (unresolved_unavailabe_cgraph_nodes.size() > 0)
	{
		std::cerr << "Warning : " << unresolved_unavailabe_cgraph_nodes.size()
				<< " unresolved unavailable functions \nno available functions for:\n";
		for (const int & i : unresolved_unavailabe_cgraph_nodes)
		{
			std::cerr << functions[i].name << "\n";
		}
		std::cerr << "\n";
	}
	if (unresolved_su_nodes.size() > 0)
	{
		std::cerr << "Warning : " << unresolved_su_nodes.size()
				<< " unresolved stack usage lines \nno functions for for:\n";
		for (const int & i : unresolved_su_nodes)
		{
			std::cerr << su_nodes[i].name << "\n";
		}
		std::cerr << "\n";
	}
	if (unresolved_calls.size() > 0)
	{
		std::cerr << "Warning : " << unresolved_calls.size()
				<< " unresolved function calls \nno available functions for:\n";
		for (const std::pair<int, int> & i : unresolved_calls)
		{
			std::cerr << "caller: " << functions[i.first].name << " Calling: "
					<< functions[i.first].calls[i.second].first << "\n";
		}
		std::cerr << "\n";
	}
}


