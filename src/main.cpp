/*
 * main.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

//local includes
#include <stackanalyser.hpp>

//boost includes
#include <boost/program_options.hpp> //bpo::options_description

//std includes
#include <iostream> //std::cout
#include <fstream>  //std::ofstream
#include <chrono>
const std::string program_description =
		"this tool can calculate the maximum stack depth for each function of your"
		" embedded programs using the .su files and .000i.cgraph files. to make "
		"your project generate these files you need to add the -fstack-usage and "
		"-fdump-ipa-cgraph flags to your compiler flags in order to make gcc "
		"generate these. The tool can generate .dot files with your programs call "
		"graph. it can print a table with stack usage/type inspired by the .su "
		"files. it also detects indirect calls and recursion, of wich it notifies "
		"the user with compiler warning style messages. this allows most ide's to "
		"jump to the file (and sometimes the line) at wich the problem area recides.";


const char * desc_help = "Print help messages";

const char * desc_input = "specify input directories from which to take .su and .000i.cgraph files";

const char * desc_stack_table = "specify output file for stack usage table (stdout if no args are specified)";

const char * desc_dot_graph = "prints callgraph in dot file (stdout if no args are specified)";

const char * desc_call_cost = "specifies the cost per function call";

namespace bpo = boost::program_options;

void print_stack_table(const std::string & output,stack_analyser * analyser)
{
	std::string output_2 = output;
	if(output_2.size() != 0)
	{
		if(output_2.length() > 4 &&output_2.substr(output_2.length() - 5).compare(".dot") == 0)
		{
			output_2 += ".dot";
		}
		std::ofstream stream;
		stream.open(output_2);
		if(!stream.is_open())
		{
			std::cerr << "could not write file " <<  output_2;
		}
		analyser->print_callgraph_dot(stream);
		stream.close();
	}else
	{
		analyser->print_callgraph_dot(std::cout);
	}
}

void print_dot_graph_table(const std::string & output,stack_analyser * analyser)
{
	std::string output_2 = output;
	if(output_2.size() != 0)
	{
		if(output_2.length() > 4 &&output_2.substr(output_2.length() - 5).compare(".txt") == 0)
		{
			output_2 += ".txt";
		}
		std::ofstream stream;
		stream.open(output_2);
		if(!stream.is_open())
		{
			std::cerr << "could not write file " <<  output_2;
		}
		analyser->print_analysis(stream);
		stream.close();
	}else
	{
		analyser->print_analysis(std::cout);
	}
}

int main(int argc, char ** argv)
{
	auto start = std::chrono::system_clock::now();

	int call_cost = 0;
	std::vector<std::string> input_folders = { };
	bool stack_table_flag = false;
	std::string stack_table_output = "";
	bool call_graph_flag = false;
	std::string call_graph_output = "";

	try
	{
		bpo::options_description desc("Options");
		desc.add_options()
				("help,h", desc_help)
				("input,i",bpo::value<std::vector<std::string> >(&input_folders)->required(),desc_input)
				("stack-table,s",bpo::value<std::string>(&stack_table_output)->implicit_value(""),desc_stack_table)
				("dot-graph,d",bpo::value<std::string>(&call_graph_output)->implicit_value(""),desc_dot_graph)
				("call-cost,c",bpo::value<int>(&call_cost),desc_call_cost);

		bpo::variables_map vm;
		try
		{
			bpo::store(
					bpo::parse_command_line(argc, argv, desc), vm);

			if (vm.count("help"))
			{
				std::cout << program_description << std::endl << desc << std::endl;
				return 0;
			}
			if (vm.count("stack-table"))
			{
				stack_table_flag = true;
			}
			if (vm.count("dot-graph"))
			{
				call_graph_flag = true;
			}

			bpo::notify(vm);
		} catch (bpo::error& e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
			std::cerr << desc << std::endl;
			return 1;
		}
		//-------------------------------------------------------------------------

		stack_analyser * analyser = new stack_analyser(input_folders,call_cost);

		if(call_graph_flag)
			print_dot_graph_table(call_graph_output,analyser);
		if(stack_table_flag)
			print_stack_table(call_graph_output,analyser);

		delete analyser;
		analyser = nullptr;

		//-------------------------------------------------------------------------

	} catch (std::exception & ex)
	{
		std::cout << "last moment exception caught :" << ex.what() << std::endl;
		return 1;
	}

	auto end = std::chrono::system_clock::now();
	auto elapsed =
	    std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << '\n';
	std::cout << "elapsed time: "<< elapsed.count() << " seconds" << '\n';
	return 0;
}
