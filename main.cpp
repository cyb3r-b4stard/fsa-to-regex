/**
 * @main
 *
 * @brief Converts given FSA to Regular Expression.
 *
 * @author https://github.com/cyb3r-b4stard (Lomikovskiy Ivan).
 *
 * @project FSA to Regex Translator
 *
 */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "utils.h"
#include "fsa.h"

#define RESET   "\033[0m"
#define BLACK   "\033[30m"     
#define RED     "\033[31m"   
#define GREEN   "\033[32m"     
#define YELLOW  "\033[33m"      
#define BLUE    "\033[34m"    
#define MAGENTA "\033[35m"   
#define CYAN    "\033[36m"    
#define WHITE   "\033[37m"     
#define BOLDBLACK   "\033[1m\033[30m"    
#define BOLDRED     "\033[1m\033[31m"  
#define BOLDGREEN   "\033[1m\033[32m"    
#define BOLDYELLOW  "\033[1m\033[33m"     
#define BOLDBLUE    "\033[1m\033[34m"   
#define BOLDMAGENTA "\033[1m\033[35m"      
#define BOLDCYAN    "\033[1m\033[36m"   
#define BOLDWHITE   "\033[1m\033[37m"    

int main(int argc, char* argv[])
{
    std::string inputPath(argv[1]), outputPath(argv[2]);

    FSA fsa(inputPath);

    try
    {
        fsa.toRegex(outputPath);
        std::cout << BOLDGREEN << "OK: " << RESET << "FSA was successfully converted to Regex\n";
    }
    catch (const std::exception &e)
    {
        std::string exception_message(e.what()),
                    error_code(exception_message.substr(0, 2)), 
                    argument;

        if (error_code == "E0")
        {
            std::cerr << BOLDRED << "ERROR: " << RESET << "Input file is malformed\n";
        }
        if (error_code == "E1")
        {
            argument = exception_message.substr(2);
            std::cerr << BOLDRED << "ERROR: " << RESET << "A state " << argument << " is not in set of states\n";
        }
        if (error_code == "E2")
        {
            std::cerr << BOLDRED << "ERROR: " << RESET << "Some states are disjoint\n";
        }
        if (error_code == "E3")
        {
            argument = exception_message.substr(2);
            std::cerr << BOLDRED << "ERROR: " << RESET << "Transition " << argument
                << " is not represented in the alphabet\n";
        }
        if (error_code == "E4")
        {
            std::cerr << BOLDRED << "ERROR: " << RESET << "Starting state is not defined\n";
        }
        if (error_code == "E5")
        {
            std::cerr << BOLDRED << "ERROR: " << RESET << "FSA is nondeterministic\n";
        }
    }

    return 0;
}