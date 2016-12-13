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

#include <iomanip>

//std includes
#include <iostream> //std::cout
#include <ostream>  //std::ostream
#include <algorithm> //std::sort


const std::string stack_analyser::unresolved_unavailable_message =
		"no definition found for function: ";

const std::string stack_analyser::unresolved_clone_message =
		"no source found for cloned function: ";

stack_analyser::stack_analyser(const std::vector<std::string> & input_folders,
		int call_cost) :
		call_cost
		{ call_cost }
{
	std::vector<std::string> su_files;
	std::vector<std::string> cgraph_files;

	get_all_files(cgraph_files, su_files, input_folders);
	get_cgraph_nodes(cgraph_files);
	get_su_nodes(su_files);
	create_call_graph();
	print_indirect_calls();
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
	if (!report.add_function(function, call_cost))
	{
		print_compiler_warning(std::cout, functions[function].filename, 0, 0,
				"function " + functions[function].name + " is recursive");
		return report;
	}
	if (functions[function].su != nullptr)
	{
		if (!inlined)
		{
			report.stack_usage += functions[function].su->usage;
		}
		report.stack_usage_type = merge_usage_type(report.stack_usage_type,
				functions[function].su->su_type);
	}
	std::vector<analysis_report> reports;
	for (const auto & edge : call_edges)
	{
		if (edge.first == function)
		{
			reports.push_back(
					analyse_function(int(edge.second), report,
							edge.property == cgraph_edge_type::inlined_call));
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

void stack_analyser::print_analysis(std::ostream & stream,
		bool print_undefined_usage)
{
	std::vector<analysis_report> reports;
	for (unsigned int i = 0; i < functions.size(); i++)
	{
		if (functions[i].is_definition()
				&& (print_undefined_usage || functions[i].su != nullptr))
		{

			reports.push_back(analyse_function(i));
		}
	}

	std::sort(reports.begin(), reports.end(), &sort_reports);
	stream << std::setw(28) << std::left << "Name:" <<
			std::setw(6) << std::left << "Usage" <<
			std::setw(6) << std::left << "Frame" <<
			std::setw(6) << std::left << "Depth" <<
			std::setw(6) << std::left << "Type" << std::endl;
	for (const auto & report : reports)
	{
		stream <<
				std::setw(28) << std::left << functions[report.functions[0]].name <<
				std::setw(6) << report.stack_usage <<
				std::setw(6) << ((functions[report.functions[0]].su != nullptr) ?
						functions[report.functions[0]].su->usage : 0) <<
				std::setw(6) << report.functions.size() <<
				std::setw(6) << std::left << su_node::usage_type_enum_to_string(report.stack_usage_type)
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
		if (functions[i].removed_body||!functions[i].is_definition())
			continue;
		auto report = analyse_function(i);
		stream << int_to_graph_label(i) << " [label=\""
				<< boost::filesystem::path(functions[i].filename).filename().string()
				<< "\\n" << functions[i].name << "\\n" << report.stack_usage << "\"];"
				<< std::endl;
	}
	for (unsigned int i = 0; i < call_edges.size(); i++)
	{
		if (call_edges[i].property == cgraph_edge_type::clone_link
				|| call_edges[i].property == cgraph_edge_type::definition_link)
			continue;
		stream << int_to_graph_label(call_edges[i].first) << " -- "
				<< int_to_graph_label(call_edges[i].second);
		switch (call_edges[i].property)
		{
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
			stream << str << std::endl;
	}
	stream << std::endl;
}

void stack_analyser::print_su_nodes(std::ostream & stream)
{
	for (const su_node & node : su_nodes)
	{
		stream << node.file_name << " " << node.name << " " << node.usage << " "
				<< su_node::usage_type_enum_to_string(node.su_type) << std::endl;
	}
	stream << std::endl;
}

int stack_analyser::get_max_stackusage(const std::string & function_name)
{
	int i = -1;
	for (unsigned int i = 0; i < functions.size(); i++)
	{
		if (functions[i].name.compare(function_name) == 0)
		{
			analysis_report rep = analyse_function(i);
			i = rep.stack_usage;
		}
	}
	return i;
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
	resolve_clones();
	eliminate_useless_edges();
	print_unresolved();
}

void stack_analyser::resolve_unavailable()
{
	for (unsigned int i = 0; i < functions.size(); i++)
	{
		if (functions[i].availability == availability_type::not_available
				&& !functions[i].removed_body)
		{
			bool resolved = false;
			for (unsigned int j = 0; j < functions.size(); j++)
			{
				if (functions[i].mangled_name.compare(functions[j].mangled_name) == 0
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
				functions[j].su = &su_nodes[i];
				resolved = true;
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
		std::string f_name = functions[i].mangled_name + "/"
				+ std::to_string(functions[i].symbol_nr);
		if (f_name.compare(functions[function_nr].calls[call_nr].first) == 0)
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

void stack_analyser::resolve_clones()
{
	for (unsigned int i = 0; i < functions.size(); i++)
	{
		if (functions[i].clone_off.length() != 0)
		{
			bool resolved = false;
			for (unsigned int j = 0; j < functions.size(); j++)
			{
				if (functions[i].clone_off.compare(functions[j].mangled_name) == 0
						&& functions[j].is_definition())
				{
					resolved = true;
					property_pair<int, int, cgraph_edge_type> p;
					p.first = static_cast<int>(i);
					p.second = static_cast<int>(j);
					p.property = cgraph_edge_type::clone_link;
					call_edges.push_back(p);
					break;
				}
			}
			if (!resolved)
				unresolved_clones.push_back(static_cast<int>(i));
		}
	}
}

void stack_analyser::print_unresolved()
{
	if (unresolved_unavailabe_cgraph_nodes.size() > 0)
	{

		for (const int & i : unresolved_unavailabe_cgraph_nodes)
		{
			print_compiler_warning(std::cout, functions[i].filename, 0, 0,
					unresolved_unavailable_message + functions[i].mangled_name);
		}
		std::cout << "\n";
	}
	if (unresolved_su_nodes.size() > 0)
	{
		for (const int & i : unresolved_su_nodes)
		{
			print_compiler_warning(std::cout, su_nodes[i].file_name,
					su_nodes[i].line_nr, su_nodes[i].char_nr,
					"unresolved stack usage " + su_nodes[i].name);
		}
		std::cout << "\n";
	}
	if (unresolved_calls.size() > 0)
	{
		for (const std::pair<int, int> & i : unresolved_calls)
		{
			print_compiler_warning(std::cout, functions[i.first].filename, 0, 0,
					"unresolved call caller: " + functions[i.first].name + " Calling: "
							+ functions[i.first].calls[i.second].first);
		}
	}
	if (unresolved_clones.size() > 0)
	{
		for (const int & i : unresolved_su_nodes)
		{
			print_compiler_warning(std::cout, functions[i].filename, 0, 0,
					unresolved_clone_message + functions[i].mangled_name);
		}
		std::cout << "\n";
	}
}

void stack_analyser::print_indirect_calls()
{
	for (auto & node : functions)
	{
		if (node.indirect_calls > 0)
		{
			std::string warning = node.name + " contains "
					+ std::to_string(node.indirect_calls) + " indirect calls";
			print_compiler_warning(std::cout, node.get_filename(), node.get_linenr(),
					node.get_charnr(), warning);
		}
	}
}

bool compare_edges(const property_pair<int, int, cgraph_edge_type> & lhs,
		const property_pair<int, int, cgraph_edge_type> & rhs)
{
	return (lhs.first == rhs.first && lhs.second == rhs.second
			&& lhs.property == rhs.property);
}

bool sort_edges(const property_pair<int, int, cgraph_edge_type> & lhs,
		const property_pair<int, int, cgraph_edge_type> & rhs)
{
	return (lhs.first < rhs.first
			|| (lhs.first == rhs.first && lhs.second < rhs.second)
			|| (lhs.first == rhs.first && lhs.second == rhs.second
			&& lhs.property < rhs.property));

}

void stack_analyser::eliminate_useless_edges()
{
	std::vector<int> eliminated_edges;
	std::vector<int> eliminated_nodes;
	for (unsigned int i = 0; i < call_edges.size(); i++)
	{
		if (call_edges[i].property == cgraph_edge_type::clone_link
				|| call_edges[i].property == cgraph_edge_type::definition_link)
		{
			for (unsigned int j = 0; j < call_edges.size(); j++)
			{
				if (call_edges[j].second == call_edges[i].first)
				{
					call_edges[j].second = call_edges[i].second;

					eliminated_edges.push_back(i);
					eliminated_nodes.push_back(call_edges[i].first);
				}
			}
		}
	}
	std::sort(call_edges.begin(),call_edges.end(),sort_edges);
	call_edges.erase(unique(call_edges.begin(), call_edges.end(), compare_edges),
			call_edges.end());
}

void stack_analyser::print_compiler_warning(std::ostream & stream,
		const std::string & file_name, const int & line, const int & character,
		const std::string & warning)
{
	print_compiler_message(stream, file_name, line, character,
			"warning: " + warning);
}
void stack_analyser::print_compiler_error(std::ostream & stream,
		const std::string & file_name, const int & line, const int & character,
		const std::string & error)
{
	print_compiler_message(stream, file_name, line, character, "error: " + error);
}
void stack_analyser::print_compiler_message(std::ostream & stream,
		const std::string & file_name, const int & line, const int & character,
		const std::string & message)
{
	stream << file_name << ':' << line << ':' << character << ": " << message
			<< std::endl;
	stream.flush();
}

