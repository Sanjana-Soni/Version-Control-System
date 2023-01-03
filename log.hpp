namespace mygitlog{
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
#include <ctime>

using namespace std;

string log_path;
string get_cwd();
string currenttime();


void log_print(){

    log_path = get_cwd();
    log_path+= "/.mygit/log.txt";
    //path is goto .mygit and then log.txt file
    
    string s;
    ifstream fin ;
    fin.open(log_path,ios::in);//open log file by providing its path in read mode
    while(fin){
        getline(fin , s);//read it line by line
        cout<<s<<endl;//print it line by line
    }
    fin.close();//close the file
}
void log_write(string str){
    
    log_path = get_cwd();
    log_path+= "/.mygit/log.txt";
    //path is goto .mygit and then log.txt file

    ofstream fout;
    fout.open(log_path,ios::app); //opening file in append mode i.e. writing at the last
    string s = currenttime();//take curent time
    
    fout<<s<<" : "<<str<<endl;//store in the log file 
    fout.close();
}

string currenttime(){
    time_t current_time;
    current_time = time(NULL);
    tm *tm_local = localtime(&current_time);
    string time ="";
    string timehour =  to_string(tm_local->tm_hour);
    string timemin = to_string(tm_local->tm_min);
    string timesec = to_string(tm_local->tm_sec);
    time += timehour+":"+timemin + ":"+timesec;
    return time;//returning the current time

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
