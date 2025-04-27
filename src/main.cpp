#include <iostream>
#include <map>
#include <vector>
#include <dirent.h>
#include <dlfcn.h>
#include "plugin.h"

int main() {
    const char* plugdir = "./plugins";
    DIR* dp = opendir(plugdir);
    if(!dp) { perror("opendir"); return 1; }

    std::map<std::string, operate_fn> ops;
    struct dirent* ent;
    while((ent = readdir(dp))) {
        if(!strstr(ent->d_name, ".so")) continue;
        std::string path = std::string(plugdir) + "/" + ent->d_name;
        void* handle = dlopen(path.c_str(), RTLD_NOW);
        if(!handle) { std::cerr<<dlerror(); continue; }
        auto name = (name_fn)dlsym(handle, "name");
        auto op   = (operate_fn)dlsym(handle, "operate");
        if(name && op) ops[name()] = op;
    }
    closedir(dp);

    std::cout << "Available ops:";
    for(auto& [k,_]: ops) std::cout<<" "<<k;
    std::cout<<"\nType `<op> a b` or `exit`.\n";

    std::string cmd;
    while(true) {
        std::cout<<"> ";
        if(!std::getline(std::cin, cmd) || cmd=="exit") break;
        std::istringstream iss(cmd);
        std::string op; double a,b;
        if(!(iss>>op>>a>>b) || !ops.count(op)) {
            std::cout<<"Invalid.\n"; continue;
        }
        std::cout<<"= "<<ops[op](a,b)<<"\n";
    }
    return 0;
}
