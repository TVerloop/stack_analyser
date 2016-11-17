/*
 * main.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

//local includes
#include <stackanalyser.hpp>

//boost includes
#include <boost/program_options.hpp> //boost::program_options::options_description

//std includes
#include <iostream> //std::cout
#include <fstream>  //std::ofstream
#include <chrono>
const std::string program_description =
		"this app can calculate the maximum stack depth for each function of your embedded programs using the .su files and .000i.cgraph files \n\n"

				"USAGE: stack_analyser [-h] [-o ARG] [-c ARG] [-d ARG] [-c ARG] -i ARG\n\n"

				"-- Option Descriptions --\n";

int main(int argc, char ** argv)
{
	int call_cost = 0;
	auto start = std::chrono::system_clock::now();
	std::vector<std::string> input_folders =
	{ };
	std::string output = "";
	std::string call_graph_output = "";
	try
	{
		boost::program_options::options_description desc("Options");
		desc.add_options()("help,h", "Print help messages")("input,i",
				boost::program_options::value<std::vector<std::string> >(&input_folders)->required(),
				"specify input directories")("output,o",
				boost::program_options::value<std::string>(&output),
				"specify output file (stdout if not specified)")
				("dot-graph,d",
						boost::program_options::value<std::string>(&call_graph_output),
						"prints callgraph in dot file")
				("call-cost,c",
						boost::program_options::value<int>(&call_cost),
						"specifies the cost per function call");

		boost::program_options::variables_map vm;
		try
		{
			boost::program_options::store(
					boost::program_options::parse_command_line(argc, argv, desc), vm);

			if (vm.count("help"))
			{
				std::cout << program_description << std::endl << desc << std::endl;
				return 0;
			}

			boost::program_options::notify(vm);
		} catch (boost::program_options::error& e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
			std::cerr << desc << std::endl;
			return 1;
		}
		//-------------------------------------------------------------------------

		stack_analyser * analyser = new stack_analyser(input_folders,call_cost);
		if(call_graph_output.size() != 0)
		{
			if(call_graph_output.length() > 4 &&call_graph_output.substr(call_graph_output.length() - 5).compare(".dot") == 0)
			{
				call_graph_output += ".dot";
			}
			std::ofstream stream;
			stream.open(call_graph_output);
			if(!stream.is_open())
			{
				std::cerr << "could not write file " <<  call_graph_output;
			}
			analyser->print_callgraph_dot(stream);
			stream.close();
		}
		analyser->print_analysis(std::cout);
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
