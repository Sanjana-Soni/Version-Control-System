namespace mygitCommit
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

vector<string> get_files(string);
string get_cwd();

int commit()
{
    
    string cwd = get_cwd();
    
    string version_no_path = cwd + "/.mygit/version_no.txt";

    //read current version no
    ifstream ifile(version_no_path.c_str());
    int version;
    ifile >> version;

    //new version
    version = version + 1;
    ifile.close();
    //write new version no 

    ofstream ofile(version_no_path, ios::trunc);
    ofile << version;

    //in mygit create a new version folder
    string newDir = cwd + "/.mygit/";
    newDir = newDir + to_string(version);
    int check = mkdir(newDir.c_str(), 0777);
    if (!check)
        printf("New version created\n");
    else
    {
        printf("Unable to create new version\n");
        exit(1);
    }
    
    //copy all files to new version from older version
    string cmd = "cp -r";
    cmd = cmd + " " + cwd + "/.mygit/";
    int x = version - 1;
    cmd = cmd + to_string(x);
    cmd = cmd + "/. ";
    cmd = cmd + cwd + "/.mygit/";
    cmd = cmd + to_string(version);
    cout << cmd << endl;
    system(cmd.c_str());

    string base_path = cwd + "/.mygit/base/";
    vector<string> files = get_files(cwd); 
    for (int i = 0; i < files.size(); i++)
    {   //diff base_path/file file >> cwd/.mygit/lastversion/file.patch
        //diff btw base file and version file and create patch
        string diff_cmd = "diff " + base_path + files[i];
        diff_cmd = diff_cmd + " " + files[i];
        diff_cmd = diff_cmd + " >> " + cwd + "/.mygit/" + to_string(version - 1) + "/" + files[i] + ".patch";
        system(diff_cmd.c_str());
        
    }

    //remove files after creating patch for it
    for (int i = 0; i < files.size(); i++)
    {
        if(files[i] != "index.txt"){
        string rm_cmd = "rm ";
        rm_cmd = rm_cmd + cwd + "/.mygit/" + to_string(version - 1) + "/" + files[i];
        system(rm_cmd.c_str());
        
        }
    }
  
    cout<<"successfully committed previous version and moved to new version"<<endl;
    return 0;
}

vector<string> get_files(string cwd)
{
    struct dirent *pointerp;
    DIR *dir = opendir(cwd.c_str()); //pointer to the directory whose files we wish to copy
    if (dir == NULL)
    {
        cout << "Failed to open the Directory";
        exit(EXIT_FAILURE);
    }

    vector<string> v; //vector to store the filenames
    while ((pointerp = readdir(dir)) != NULL) //readdir(dir) will point to the files one by one
    {
        //cout << pointerp->d_name << endl;
        string file = pointerp->d_name;
        if (file != "." && file != ".." && file != ".mygit" && file != "n.out" && file != ".vscode" && file!="mygit"  && file!="status.hpp" && file!="add.hpp" && file!="commit.hpp" && file!="mygit.cpp" && file!="log.hpp" && file!="pull.hpp" && file!="push.hpp" && file!="merge.hpp" && file!="rollback.hpp" && file!="mygit" && file!="pull" && file!="push"  )
        {
            v.push_back(file); //push the file in vector
        }
        
    }
    closedir(dir); //close the directory 
    return v; //return the vector
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

} 
