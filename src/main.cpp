/*
 * main.cpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Tom
 */

#include <boost/program_options.hpp> //boost::program_options::options_description
#include <iostream> //std::cout
#include <stackanalyser.hpp>
const std::string program_description =
    "this app can calculate the maximum stack depth for each function of your embedded programs using the .su files and .000i.cgraph files \n\n"

    "USAGE: stack_analyser_v2 [-h] [-o ARG] -i ARG\n\n"

    "-- Option Descriptions --\n";

int main(int argc, char ** argv)
{
  std::vector<std::string> input_folders = {};
  try
  {
    boost::program_options::options_description desc("Options");
    desc.add_options()("help,h", "Print help messages")("input,i",
        boost::program_options::value<std::vector<std::string> >(&input_folders)->required(), "specify input directories")(
        "output,o", boost::program_options::value<std::string>(),
        "specify output file (stdout if not specified)");

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

    stack_analyser * analyser = new stack_analyser(input_folders);
    analyser->print_analysis(std::cout);
    delete analyser;
    analyser = nullptr;

    //-------------------------------------------------------------------------

  } catch (std::exception & ex)
  {
    std::cout << "last moment exception caught :" << ex.what() << std::endl;
    return 1;
  }
  return 0;
}
