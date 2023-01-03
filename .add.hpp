namespace mygitadd
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


char cwd[PATH_MAX];
string mygit_path;
string version_no_file_path;
string version_no;
string index_path;
string base_path;
int n;


vector<string> files;
map<string, string> map_index;

vector<string> new_files;
vector<string> modified_files;
vector<string> deleted_files;



string get_sha(string);
vector<string> get_files(string);
map<string, string> get_map(string);


int add_new(vector<string> &newf)
{

    for (int i = 0; i < newf.size(); i++)
    {
        string filename = newf[i];

        //copy file from cwd to current version 
        string s = cwd;
        string path_in = s + "/" + filename;
        string sha = get_sha(path_in);
        string path_out = mygit_path + version_no + "/" + filename;

        
        string copy_cmd = "cp ";
        copy_cmd = copy_cmd + path_in + " ";
        copy_cmd = copy_cmd + s + "/.mygit/base/" + filename;
        system(copy_cmd.c_str()); //files copied from cwd to base folder
        
        //copying new files to the current version folder
        ifstream fin;
        ofstream fout;

        fin.open(path_in, ios::in);
        fout.open(path_out, ios::out);
        string line;

        while (fin)
        {
            getline(fin, line);
            fout << line << endl;
        }
        fin.close();
        fout.close();

        map_index[filename] = sha;//add new file in the map
        cout<<"adding new file: "<<filename<<endl;
    }
    
    
    string main_index_path = string(cwd) + "/.mygit/main_index.txt";
    
    ofstream main_index_file(main_index_path, ios::app);
    for (int i = 0; i < newf.size(); i++)
    {
        string filename = newf[i];
        
        //in main index file add the new file name and the version no in which it came into existence
        main_index_file << filename << " " << version_no << "\n";

    }

    main_index_file.close();

    //change the index file of current version accordingly
    ofstream fout1;
    fout1.open(index_path, ios::out);
    for (auto it = map_index.begin(); it != map_index.end(); it++)
    {
        string line;
        line += it->first + " " + it->second;
        fout1 << line << endl;
    }
    fout1.close();

    return 1;
}
int add_modified(vector<string> modifiedf)
{
    for (int i = 0; i < modifiedf.size(); i++)
    {
        string filename = modifiedf[i];
        string s = cwd;
        string path_in = s + "/" + filename;
        string path_out = mygit_path + version_no + "/" + filename;
        ifstream fin(path_in);
        ofstream fout(path_out);
        //modify the file line by line
        string line;
        while (fin)
        {
            getline(fin, line);
            fout << line << endl;
        }
        fin.close();
        fout.close();

        string sha = get_sha(filename);
        map_index[filename] = sha;//change sha of the file in map
        cout<<"adding modified file: "<<filename<<endl;
    }

    //change the index file of current version accordingly
    ofstream fout1;
    fout1.open(index_path, ios::out);
    for (auto it = map_index.begin(); it != map_index.end(); it++)
    {
        string line;
        line += it->first + " " + it->second;
        fout1 << line << endl;
    }
    fout1.close();

    return 1;
}

int add_deleted(vector<string> deletedf)
{   
    //path from where the file is to be deleted i.e the current version
    string delete_in = mygit_path + version_no + "/";
    
    
    for (int i = 0; i < deletedf.size(); i++)
    {
        string str = delete_in + deletedf[i];
        map_index.erase(deletedf[i]);//delete the file from map
        remove(str.c_str());//delete file in current version
        cout<<"deleting file: "<<deletedf[i]<<endl;
    }
        
    //change the index file  of current version accordingly
    ofstream fout1;
    fout1.open(index_path, ios::out);
    for (auto it = map_index.begin(); it != map_index.end(); it++)
    {
        string line;
        line += it->first + " " + it->second;
        fout1 << line << endl;
    }
    fout1.close();
    


    return 1;
}


int add()
{
    mygit_path = "";
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        mygit_path = cwd;
        mygit_path += "/.mygit/";//myit path is the path to .mygit folder
    }
    else
    {
        perror("Unable to get current working directory!\n");
        exit(1);
    }

    //get files into a vector from the cwd
    files = get_files(cwd);
    n = files.size();

    //goto the current version and read the version no
    version_no_file_path = mygit_path + "version_no.txt";
    fstream vcfile(version_no_file_path, std::ios_base::in);
    vcfile >> version_no;
    
    //go to current version index file and create an map
    index_path = mygit_path + version_no + "/" + "index.txt";
    map_index = get_map(index_path);

    //comparing for new , modified and deleted files
    for (int i = 0; i < n; i++)
    {
        string sha = get_sha(files[i]);

        if (map_index.find(files[i]) == map_index.end())//file exist in vector but not in map
        {
            new_files.push_back(files[i]);
        }
        else if (map_index[files[i]] != sha)//sha of same file name dosent match
        {
            modified_files.push_back(files[i]);
        }
    }

    sort(files.begin(), files.end());
    for (auto f : map_index)//file exist in map but not in vector
    {
        if (find(files.begin(), files.end(), f.first) == files.end())
        {
            deleted_files.push_back(f.first);
        }
    }

    //display all new files
    if (new_files.size() != 0)
    {
        cout << "New:" << endl;
        for (int i = 0; i < new_files.size(); i++)
        {
            cout << new_files[i] << endl;
        }
    }
    //display all modified files
    if (modified_files.size() != 0)
    {
        cout << "Modified:" << endl;
        for (int i = 0; i < modified_files.size(); i++)
        {
            cout << modified_files[i] << endl;
        }
    }
    //display all deleted files
    if (deleted_files.size() != 0)
    {
        cout << "Deleted:" << endl;
        for (int i = 0; i < deleted_files.size(); i++)
        {
            cout << deleted_files[i] << endl;
        }
    }

    cout<<endl;
    int new_processed = 0, modified_processed = 0, deleted_processed = 0;
    if (new_files.size() != 0)
    {
        new_processed = add_new(new_files);
        if (new_processed)
            new_files.clear();
        if (new_files.size())
            cout << "new_files not cleared!" << endl;
    }
    if (modified_files.size() != 0)
    {
        modified_processed = add_modified(modified_files);
        if (modified_processed)
        {
            modified_files.clear();
        }
    }
    
    if (deleted_files.size() != 0)
    {
        deleted_processed = add_deleted(deleted_files);
        if (deleted_processed)
            deleted_files.clear();
    }

    if (new_files.size() == 0 && modified_files.size() == 0 &&  deleted_files.size() == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

string get_sha(string file_name)
{
    string s = "sha1sum " + file_name + " > temp.txt";

    char arr[255];
    strcpy(arr, s.c_str());//store string s in char array
    system(arr);//sha calculation

    FILE *filepointer = fopen("temp.txt", "r+");//open temp.txt file to read the sha
    char aux[100];
    fread(aux, sizeof(char), 100, filepointer);//reading in sha in aux array
    string input = aux;
    stringstream ss(input);
    string sha;
    ss >> sha;
    fclose(filepointer);

    remove("temp.txt");//delete temp.txt file
    
    return sha;
}

vector<string> get_files(string path)
{
    struct dirent *pointerp;
    DIR *dir = opendir(path.c_str()); //pointer to the directory whose files we wish to copy
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
        if (file != "." && file != ".." && file != ".mygit" && file != "a.out" && file != ".vscode" && file!="mygit"  && file!="status.hpp" && file!="add.hpp" && file!="commit.hpp" && file!="mygit.cpp" && file!="log.hpp" && file!="pull.hpp" && file!="push.hpp" && file!="merge.hpp" && file!="rollback.hpp" && file!="pull" && file!="push" )
        {
            v.push_back(file); //push the file in vector
        }
        
    }
    closedir(dir); //close the directory 
    return v; //return the vector
}

map<string, string> get_map(string path)//make map from index file given the path of index.txt
{
    map<string, string> index_map;
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
            index_map[filename] = sha;//filename:sha
        }
    }
    else        
    {
        perror("unable to open file in get_map function");
        exit(1);
    }
    return index_map;
}
}

