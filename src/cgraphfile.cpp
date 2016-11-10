/*
 * cgraphfile.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

//local includes
#include <cgraphfile.hpp>
#include <cgraphnode.hpp>

//std includes
#include <iostream>     //std::cout
#include <algorithm>    // std::find

//boost includes
#include <boost/filesystem.hpp>

cgraph_file::cgraph_file(const std::string & file_name) :
		file_name
		{ file_name }
{
	std::ifstream input(file_name);
	if (!input)
	{
		std::cout << "failed to open file " << file_name << " \n";
	}
	std::vector<std::string> file_content;
	fill_vector_from_stream(file_content, input);
	input.close();
	get_optimized_symbol_table(file_content, raw_data);
}


void cgraph_file::get_optimized_symbol_table(
		const std::vector<std::string> & input, std::vector<std::string> & output)
{
	int begin = 0;
	int end = 0;
	output.clear();
	begin = std::find(input.begin(), input.end(), "Optimized Symbol table:")
			- input.begin();
	end = std::find(input.begin(), input.end(), "Materializing clones")
			- input.begin();
	begin++;
	output = std::vector<std::string>(&input[begin], &input[end]);
}

void cgraph_file::fill_vector_from_stream(std::vector<std::string> & vector,
		std::ifstream & stream)
{
	vector.push_back(std::string());
	while (!stream.eof())
	{
		char c = 0;
		while (stream.get(c))
		{
			if (c == '\r' || c == '\n')
			{
				if (vector.back().size() > 0)
					vector.push_back(std::string());
				break;
			}
			vector.back() += c;
		}
	}
}

void cgraph_file::get_graph_nodes(std::vector<cgraph_node> & nodes)
{
	if (raw_data.size() <= 0)
		return;
	std::vector<std::string> raw_node_data;
	raw_node_data.push_back(raw_data[0]);
	for (unsigned int i = 1; i < raw_data.size(); i++)
	{
		std::string str = raw_data[i];
		if (str.size() > 2 && (str.substr(0, 2).compare("  ") != 0))
		{
			cgraph_node n(raw_node_data);
			boost::filesystem::path p(file_name);
			n.filename = p.filename().string().substr(0,(p.filename().string().size() - 12));
			nodes.push_back(n);
			raw_node_data.clear();
		}
		raw_node_data.push_back(raw_data[i]);
	}
	nodes.push_back(cgraph_node(raw_node_data));
}
