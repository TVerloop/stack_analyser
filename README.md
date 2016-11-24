# stack_analyser
a tool to analyze the required stack of your program

This tool can calculate the maximum stack depth for each function of your embedded programs using the .su files and .000i.cgraph files. to make your project generate these files you need to add the -fstack-usage and -fdump-ipa-cgraph flags to your compiler flags in order to make gcc generate these. The tool can generate .dot files with your programs call graph. it can print a table with stack usage/type inspired by the .su files. it also detects indirect calls and recursion, of which​ it notifies the user with compiler warning style messages. this allows most ide's to jump to the file (and sometimes the line) at which the problem area resides.

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

stack_analyser [-h] [-s[ARG]] [-c ARG] [-d[ARG]] [-c ARG] -i ARG


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

EXAMPLES:

./stack_analyser -i <path to build dir>
this will output only warnings about recursion, indirect calls, and missing log files.

./stack_analyser -s -i <path to build dir>
this will output warnings about recursion, indirect calls, and missing log files. And
will print a sorted table with functions and their respective stack cost in the stdout

./stack_analyser -sstack-table.txt -i <path to build dir>
this will output warnings about recursion, indirect calls, and missing log files. And
will print a sorted table with functions and their respective stack cost in the file stack-table.txt

./stack_analyser -sstack-table.txt -dcallgraph.dot -i <path to build dir>
this will output warnings about recursion, indirect calls, and missing log files. And
will print a sorted table with functions and their respective stack cost in the file stack-table.txt
and print an dot callgraph in callgraph.dot file which can be compiled to another format using a dot
comiler

./stack_analyser -sstack-table.txt -dcallgraph.dot -c 4 -i <path to build dir>
this will output warnings about recursion, indirect calls, and missing log files. And
will print a sorted table with functions and their respective stack cost in the file stack-table.txt
and print an dot callgraph in callgraph.dot file and for every function call it will add an extra 4 bytes to
the stack-cost.

TODO:

-print callgraph in format colapsable by most text editors (xml,json)  
-identify root/main/task functions for rtos programs.  
-identify never called functions.  
-identify removed functions.  
