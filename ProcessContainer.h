#pragma once
#include <string>
#include <vector>
#include "Process.h"

class ProcessContainer{
    private:
        std::vector<Process>_list;
    public:
        ProcessContainer()
        {
            this->refreshList();
        }
        void refreshList();
        std::string printList();
        std::vector<std::string> getList();
};