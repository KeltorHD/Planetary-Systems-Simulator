#include "pch.h"
#include "engine/program.hpp"

int main()
{
    system("chcp 65001>nul");

    try
    {
        Program pr;

        pr.run();
    }
    catch (std::string e)
    {
        std::cerr << e << std::endl;
    }

    return 0;
}