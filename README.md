# UDACITY COURSE - HTOP

- Pre-requisites ncurses g++

On Ubuntu:
```sudo apt install libncurses-dev g++```

To compile run
``` g++ -std="c++17" main.cpp Process.cpp util.cpp  ProcessParser.cpp SysInfo.cpp ProcessContainer.cpp -lncurses -o a.out && ./a.out```

