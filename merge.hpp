namespace mygitmerge
{
#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>
#include <sys/stat.h>
#include <string>

using namespace std;

vector<string> merge_get_files(string );
string get_cwd();

int merge()  //removes files from cwd and move files from push folder to cwd (also removes file from push after copying)
{   
    //getting current working directory path
    string cwd = get_cwd();

    //removing files from current working directory
    vector<string> file_in_cwd = merge_get_files(cwd);
    for (auto it : file_in_cwd)
    {
        string rmfile = cwd + "/" + it;
        remove(rmfile.c_str());
    }

    //moving files from push folder to current working directory
    string push_path = cwd + "/push";
    vector<string> files = merge_get_files(push_path);
    for (auto it : files)
    {
        string src = push_path + "/" + it;
        string des = cwd + "/" + it;
        cout << "moving file " << src << " to " << des << endl;
        //moving
        ifstream fin;
        ofstream fout;
        fin.open(src, ios::in);
        fout.open(des, ios::out);
        string line;

        while (fin)
        {
            getline(fin, line);
            fout << line << endl;
        }
        remove(src.c_str());
        
        fin.close();
        fout.close();
        }
    return 0;
}

string get_cwd(){
    char cwd_path[PATH_MAX]; 
    string current_path;
    if (getcwd(cwd_path, sizeof(cwd_path)) != NULL)
    {
        current_path = cwd_path;
    }
    else
    {
        perror("unable to get current working directory");
        exit(1);
    }
    return current_path;
}

vector<string> merge_get_files(string path)
{
    
    struct dirent *pointerp;
    DIR *dir = opendir(path.c_str());
    if (dir == NULL)
    {
        cout << "Error in opening Directory";
        exit(EXIT_FAILURE);
    }

    vector<string> v;
    while ((pointerp = readdir(dir)) != NULL)
    {
        
        string file = pointerp->d_name;
        string filepath = path + "/" + file;
        if (file == "." || file == ".." || file == ".mygit" || file == "a.out" || file == ".vscode" || file == "mygit" || file=="status.hpp" || file=="add.hpp" || file=="commit.hpp" || file=="mygit.cpp" || file=="log.hpp" || file=="pull.hpp" || file=="push.hpp" || file=="merge.hpp" || file=="rollback.hpp" || file=="mygit" || file=="pull"|| file=="push")
            {
                continue;
            }
        else
            {
                v.push_back(file);
            }
    }
    closedir(dir);
    return v;
}

}
