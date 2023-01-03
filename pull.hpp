namespace mygitpull
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

string get_cwd();
vector<string> pull_get_files(string);

int pull() //copies files from current version to pull folder
{
    //create pull folder and copy current version files into it

    //get cwd
    string cwd_path = get_cwd();
    string mygit_path = cwd_path;
    mygit_path += "/.mygit/";


    //retrieve version no
    string version_no;
    string version_no_file_path = mygit_path + "version_no.txt";
    fstream version_no_file(version_no_file_path, std::ios_base::in);
    version_no_file >> version_no;
    version_no_file.close();

    //copy files from current version to pull folder
    string src_path = mygit_path + version_no;
    string des_path = cwd_path + "/pull";

    cout << "copying all files from " << src_path << endl;
    vector<string> files = pull_get_files(src_path);
    
    //creating pull folder
    if (mkdir(des_path.c_str(), 0777) == -1 && errno != EEXIST)
    {
        cerr << "unable to make pull directory " << strerror(errno) << endl;
        return 0;
    }
    //copy
    for (auto it : files)
    {
        cout << "file going to be copied : " << it << endl;
        string src = src_path + "/" + it;
        string des = des_path + "/" + it;
        string cmd = "cp " + src + " " + des;
        cout << "copying " << src << " to" << des << endl;
        system(cmd.c_str());
    }
    cout<<"successfully pulled files to: "<<des_path<<endl;
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


vector<string> pull_get_files(string path)
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
        
        string filename = pointerp->d_name;
        if ( filename == "." || filename == ".." || filename == "index.txt" )
        {
            continue;
        }
        else
        {
            v.push_back(filename);
        }
    }
    closedir(dir);
    return v;
}

}
