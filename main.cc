#include "core.h"
#include <string>
#include <iostream>
#include "variable.h"

void display_header()
{
    std::cout << "" << std::endl;
    std::cout << " □ ■ □ □ □ ■ ■ ■ □ ■ □ ■ □ ■ □" << std::endl;
    std::cout << " □ ■ □ □ □ ■ □ ■ □ ■ ■ □ □ □ □" << std::endl;
    std::cout << " □ ■ □ □ □ ■ □ ■ □ ■ □ ■ □ ■ □" << std::endl;
    std::cout << " □ ■ ■ ■ □ ■ ■ ■ □ ■ □ ■ □ ■ □" << std::endl;
    std::cout << "" << std::endl;
}
int prc_main()
{
    core* c = new core();
    while(std::cin)
    {
        // read
        std::string input_line;
        getline(std::cin,input_line);
        // special case for 'exit'
        if(input_line.compare("exit")==0)
            break;
        // evaluate, display result
        std::string out = c->evaluate_to_string(input_line);
        std::cout << out << std::endl;
    }
    delete c;
}
int cmd_main()
{
    // create core
    core* c = new core();
    // query user input
    while(std::cin)
    {
        // display prompt
        std::string input_line;
        std::cout << " ▶ ";
        getline(std::cin,input_line);
        // special case for 'exit'
        if(input_line.compare("exit")==0)
            break;
        // evaluate, display result
        std::string out = c->evaluate_to_string(input_line);
        std::cout << out << std::endl;
    }
    // dump core
    delete c;
}
void test_main()
{

}
int main(int argc, char** argv)
{

    // no arguments => command line mode
    if(argc==1)
    {
        display_header();
        std::cout << " Command Line Mode" << std::endl;
        std::cout << "" << std::endl;
        cmd_main();
        // other arguments? => determine mode
    }
    else
    {
        std::string arg = argv[1];
        if(arg.compare("c")==0)
        {
            display_header();
            std::cout << "Command Line Mode" << std::endl;
            std::cout << "" << std::endl;
            cmd_main();
        }
        else if(arg.compare("p")==0)
        {
            prc_main();
        }
    }

}

