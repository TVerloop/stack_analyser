/**
 * \file stackanalyser.hpp
 *
 */

#ifndef STACKANALYSER_HPP_
#define STACKANALYSER_HPP_

//local includes
#include <propertypair.hpp>
#include <cgraphnode.hpp>
#include <sunode.hpp>
#include <analysisreport.hpp>
//std includes
#include <vector> //std::vector<T>
#include <string> //std::string

/**
 * \class stack_analyser
 *
 *
 * \brief the main class of the program
 *
 *	deducts all information from all .000i.cgraph and .su files in specified folder
 *
 * \author Tom Verloop
 *
 * \version 1.0
 *
 *
 * Contact: Tom_Verloop@outlook.com
 *
 * Created on: Nov 2, 2016
 *
 */

class stack_analyser
{
public:
	stack_analyser(const std::vector<std::string> & input_folders, int call_cost = 0);
	stack_analyser(const std::string & input_folders);

	/**
	 *
	 *	\brief prints a table of all found functions
	 *	\param stream contains the stream to print to.
   *	\return none
   *
	 */
	void print_analysis(std::ostream & stream);

	/**
	 *
	 *	\brief prints a callgraph in dot format
	 *	\param stream contains the stream to print to.
   *	\return none
   *
	 */
	void print_callgraph_dot(std::ostream & stream);

	/**
	 *
	 *	\brief prints all found symbols
	 *	\param stream contains the stream to print to.
   *	\return none
   *
	 */
	void print_symbols(std::ostream & stream);

	/**
	 *
	 *	\brief prints the raw data of all found symbols
	 *	\param stream contains the stream to print to.
   *	\return none
   *
	 */
	void print_symbols_raw_data(std::ostream & stream);

	/**
	 *
	 *	\brief prints all found su nodes
	 *	\param stream contains the stream to print to.
   *	\return none
   *
	 */
	void print_su_nodes(std::ostream & stream);

	/**
	 *
	 *	\brief returns the stack usage of a specified function
	 *	\param function_name contains the name of the function
   *	\return the stack usage of said function
   *
	 */
	int get_max_stackusage(const std::string & function_name);

	/**
	 *
	 *	\brief analyses a specified function
	 *	\param function the position of the function in the function vector
	 *	\param report the report used to save data while recursively calling itself
	 *	\param inlined true if the call_edge was of type inlined
   *	\return the analysis report containing information regarding analysed function
   *
	 */
	analysis_report analyse_function(int function,analysis_report report = analysis_report() ,bool inlined = false);

private:
	/**
	 *
	 *	\brief gets all the file_names of all .000i.cgraph and .su files ins specified directories
	 *	\param cgraph_files a vector to append all cgraph_files to
	 *	\param su_files a vector to append all su_files to
	 *	\param paths a list of paths to scan
   *	\return none
   *
	 */
	static void get_all_files(std::vector<std::string> & cgraph_files,
			std::vector<std::string> & su_files,
			const std::vector<std::string> & paths);
	/**
	 *
	 *	\brief gets all the file_names of all .000i.cgraph and .su files ins specified directories
	 *	\param cgraph_files a vector to append all cgraph_files to
	 *	\param su_files a vector to append all su_files to
	 *	\param path the paths to scan
   *	\return none
   *
	 */
	static void get_all_files(std::vector<std::string> & cgraph_files,
			std::vector<std::string> & su_files, const std::string & path);

	/**
	 *
	 *	\brief fills the cgraph_nodes vector with specified files
	 *	\param cgraph_file_names list of files to extract nodes from
   *	\return none
   *
	 */
	void get_cgraph_nodes(std::vector<std::string> & cgraph_file_names);

	/**
	 *
	 *	\brief fills the su_nodes vector with specified files
	 *	\param su_file_names list of files to extract nodes from
   *	\return none
   *
	 */
	void get_su_nodes(std::vector<std::string> & su_file_names);

	/**
	 *
	 *	\brief generates call_edges from all cgraph_nodes
   *	\return none
   *
	 */
	void create_call_graph();

	/**
	 *
	 *	\brief resolves the link between not_available and available nodes
   *	\return none
   *
	 */
	void resolve_unavailable();

	/**
	 *
	 *	\brief resolves the link between su_nodes and cgraph_nodes
   *	\return none
   *
	 */
	void resolve_stack_usage();

	/**
	 *
	 *	\brief resolves the call_edges
   *	\return none
   *
	 */
	void resolve_calls();

	/**
	 *
	 *	\brief resolves the call_edges for a single function
	 *	\param function_nr the pos of the function to resolve call for
	 *	\param function_nr the pos of the call to resolve
   *	\return true if resolved
   *
	 */
	bool resolve_single_call(int function_nr, int call_nr);

	/**
	 *
	 *	\brief print all unresolved symbols to cerr
   *
	 */
	void print_unresolved();

	int call_cost;/**< contains the cost of a call */
	std::vector<int> unresolved_unavailabe_cgraph_nodes;/**< contains unresolved unavailable nodes */
	std::vector<int> unresolved_su_nodes;/**< contains unresolved su nodes */
	std::vector<std::pair<int, int>> unresolved_calls;/**< contains unresolved calls */
	std::vector<cgraph_node> cgraph_nodes;/**< contains all cgraph_nodes */
	std::vector<cgraph_node> functions;/**< contains all cgraph_nodes that are a function */
	std::vector<cgraph_node> variables;/**< contains all cgraph_nodes that are a variables */
	std::vector<su_node> su_nodes;/**< contains all su nodes */
	std::vector<property_pair<int, int, cgraph_edge_type>> call_edges;/**< contains all call_edges */
	std::vector<std::pair<int, int>> su_edges;/**< contains all su edges */
};

#endif /* STACKANALYSER_HPP_ */
