# stack_analyser
a tool to analyze the required stack of your program

this app can calculate the maximum stack depth for each function of your embedded programs using the .su files and .000i.cgraph filesto make your project generate these files you need to add -fstack-usage and -fdump-ipa-cgraph to your compiler flags in order to make gcc generate these 

BUILD:

to build you need the following boost libraries

-lboost_filesystem \n
-lboost_graph \n
-lboost_program_options \n
-lboost_regex \n
-lboost_system

paths and names should be set in the makefile.cfg

furthermore you require gcc toolchain to compile.

USAGE: 

stack_analyser [-h] [-o ARG] [-c ARG] [-d ARG] [-c ARG] -i ARG

-- Option Descriptions --

Options:
  -h [ --help ]            Print help messages\n
  -i [ --input ] arg       specify input directories from which to take .su and
                           .000i.cgraph files\n
  -s [ --stack-table ] arg specify output file for stack usage table (stdout if
                           no args are specified)\n
  -d [ --dot-graph ] arg   prints callgraph in dot file (stdout if no args are 
                           specified)\n
  -c [ --call-cost ] arg   specifies the cost per function call\n

