# stack_analyser
a tool to analyze the required stack of your program

this tool can calculate the maximum stack depth for each function of your embedded programs using the .su files and .000i.cgraph files. to make your project generate these files you need to add the -fstack-usage and -fdump-ipa-cgraph flags to your compiler flags in order to make gcc generate these. The tool can generate .dot files with your programs call graph.
it can print a table with stack usage/type inspired by the .su files. it also detects indirect calls and recursion, of wich it notifies the user with compiler warning style messages. this allows most ide's to jump to the file (and sometimes the line) at wich the problem area recides.

BUILD:

to build you need the following boost libraries

-lboost_filesystem  
-lboost_graph  
-lboost_program_options  
-lboost_regex  
-lboost_system  

paths and names should be set in the makefile.cfg

furthermore you require gcc toolchain to compile.

USAGE: 

stack_analyser [-h] [-o ARG] [-c ARG] [-d ARG] [-c ARG] -i ARG

-- Option Descriptions --

Options:  
  -h [ --help ]            Print help messages  
  -i [ --input ] arg       specify input directories from which to take .su and
                           .000i.cgraph files  
  -s [ --stack-table ] arg specify output file for stack usage table (stdout if
                           no args are specified)  
  -d [ --dot-graph ] arg   prints callgraph in dot file (stdout if no args are 
                           specified)  
  -c [ --call-cost ] arg   specifies the cost per function call  

TODO:

-print callgraph in format colapsable by most text editors (xml,json)  
-identify root/main/task functions for rtos programs.  
-identify never called functions.  
-identify removed functions.  
