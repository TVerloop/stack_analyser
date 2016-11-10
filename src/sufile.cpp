/*
 * sufile.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

//local includes
#include <sufile.hpp>

//std includes
#include <iostream> //std::cout

su_file::su_file(const std::string & file_name) :
		file_name
		{ file_name }
{
	std::ifstream input(file_name);
	if (!input)
	{
		std::cout << "failed to open file " << file_name << " \n";
	}
	fill_vector_from_stream(raw_data, input);
	input.close();
}

void su_file::fill_vector_from_stream(std::vector<std::string> & vector,
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

void su_file::get_su_nodes(std::vector<su_node> & nodes)
{
	if (raw_data.size() <= 0)
		return;
	for (const std::string & line : raw_data)
	{
		if (line.length() > 0)
			nodes.push_back(su_node(line));
	}
}

su_file::~su_file()
{
	// TODO Auto-generated destructor stub
}

