namespace mygitrollback
{
#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>
#include <string>


using namespace std;

string base_path;
char cwd_path[PATH_MAX];
string mygit_path;
string version_no_file_path;
string version_no;
string indexpath;
map<string, string> map_index;
using namespace std;

string get_cwd();
map<string, string> get_map(string path);
vector<string> get_files(string path);



int rollback()
{   
    
    //getting current working directory

    string cwd_path = get_cwd();
    string mygit_path = cwd_path + "/.mygit/";
    

    //retrieving version no 
    version_no_file_path = mygit_path + "version_no.txt";
    fstream version_no_file(version_no_file_path, std::ios_base::in);
    version_no_file >> version_no;
    int current_version = atoi(version_no.c_str());

    //getting last version
    int to_version_no = current_version - 1;
    string last_version_no = to_string(to_version_no);
    
    string current_version_path = mygit_path+version_no;
    
    indexpath = mygit_path + version_no + "/" + "index.txt";
    
    //removing the files in current version
    DIR* folder = opendir(current_version_path.c_str());
    struct dirent *next;
    string filepath;
     while ( (next = readdir(folder)) != NULL )
    {   
        filepath=current_version_path;
        filepath+= next->d_name;
        remove(filepath.c_str());
    }
    closedir(folder);

    //copy files from last version to current version (there will be index file and patch files in last version)
    string lastversionpath = mygit_path+last_version_no;
    string cp_cmd = "cp -a "+lastversionpath+"/. "+current_version_path;
    system(cp_cmd.c_str());

    base_path = mygit_path+"base/";

    map_index = get_map(indexpath);
    
    //copying the files from base folder in current version
    for (auto it=map_index.begin();it!=map_index.end();it++){
        string filename = it->first;
        string src_path = base_path+filename;
        string dest_path = current_version_path+"/"+filename;

        ifstream ifile(src_path, ios::in); 
        ofstream ofile(dest_path, ios::out); 
        if (!ifile.is_open()) {
            cout << "file not found"; 
        } 
        else { 
            ofile << ifile.rdbuf(); 
        } 
  }

  //running patch command for patch files 

    for (auto it=map_index.begin();it!=map_index.end();it++){
        string filename = it->first;
        string src_path = current_version_path+"/"+filename;
        string patchfilepath = current_version_path+"/"+filename+".patch";

        string cmd = "patch "+src_path + " " + patchfilepath;
        //patch .mygit/vno/filename .mygit/vno/filename.patch
        system(cmd.c_str()); 
    }
    
    //removing patch files from current version
    for (auto it=map_index.begin();it!=map_index.end();it++){
        string filename = it->first;
        string patchfilepath = current_version_path+"/"+filename+".patch";
        remove(patchfilepath.c_str());
    }

     //removing files from cwd which are present in current version now
    folder = opendir(current_version_path.c_str());
    string fpathlocal;
        while ( (next = readdir(folder)) != NULL )
        {
            // calculating path for each file
            fpathlocal=cwd_path;
            fpathlocal +="/";
            fpathlocal +=next->d_name;
            
            if(fpathlocal=="a.out") continue;
            //removing file
            remove(fpathlocal.c_str());
        }
        
        closedir(folder);
    
    //copying the files from current version to cwd except the index file
    for (auto it=map_index.begin();it!=map_index.end();it++){
        string filename = it->first;
        if(filename=="index.txt")continue;
        
        string src_path = current_version_path+"/"+filename;
        string current_dir_path =cwd_path;
        string dest_path = current_dir_path+"/"+filename;

        ifstream ifile(src_path, ios::in); 
        ofstream ofile(dest_path, ios::out); 
        if (!ifile.is_open()) {
            cout << "file not found"; 
        } 
        else { 
            ofile << ifile.rdbuf(); 
        } 
    }
    
    return 1;
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

map<string, string> get_map(string path)
{ 
    map<string, string> map_index;
    fstream f(path, std::ios_base::in);
    if (f.is_open())
    {
        string line;
        while (getline(f, line))
        {
            stringstream ss(line);
            string filename = "";
            string sha = "";
            ss >> filename;
            ss >> sha;
            map_index[filename] = sha;
        }
    }
    else
    {
        perror("Unable to open file information get_map function");
        exit(1);
    }
    return map_index;

}


vector<string> get_files(string path)
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
        if (file == "." || file == ".." || file == ".mygit" || file == "a.out" || file == ".vscode" || file=="mygit" || file=="status.hpp" || file=="add.hpp" || file=="commit.hpp" || file=="mygit.cpp" || file=="log.hpp" || file=="pull.hpp" || file=="push.hpp" || file=="merge.hpp" || file=="rollback.hpp" || file=="mygit" || file=="pull" || file=="push" )
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