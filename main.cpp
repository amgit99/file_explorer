#include <string>
#include <stdio.h>
#include <iostream>
#include <filesystem>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <conio.h>
#include <bits/stdc++.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;

string HOME="/home/amit/";
string PWD = "/home/";
string ROOT="/";
string RESULT;
int MODE = 0;

////////////////////////STAT CONVERSION FUNCTIONS////////////////////////////
string convertToString(double num) {
    ostringstream convert;
    convert << num;
    return convert.str();
}

double roundOff(double n) {
    double d = n * 100.0;
    int i = d + 0.5;
    d = (float)i / 100.0;
    return d;
}

string convertSize(size_t size) {              
    static const char *SIZES[] = { "B", "KB", "MB", "GB" };
    int div = 0;
    size_t rem = 0;

    while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
        rem = (size % 1024);
        div++;
        size /= 1024;
    }

    double size_d = (float)size + (float)rem / 1024.0;
    string result = convertToString(roundOff(size_d)) + " " + SIZES[div];
    return result;
}

string permissions(mode_t perm){
    string modeval = "-----------";
    modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
    modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
    modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
    modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
    modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
    modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
    modeval[6] = (perm & S_IROTH) ? 'r' : '-';
    modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
    modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
    modeval[9] = (perm & S_IFDIR) ? 'y' : '-';
    modeval[10] = (perm & S_IFREG) ? 'y' : '-';
    return modeval;        
}

///////////////////////////END STAT CONVERSION FUNCTIONS/////////////////////////////

/////////////////////////////////NORMAL MODE//////////////////////////////////

stack<string> back_stac;
stack<string> ford_stac;

void normal_mode(){
    RESULT="";
    DIR* dir = opendir(PWD.c_str());
    if(dir == NULL)cout<<"lol";
    struct dirent* entity;
    entity = readdir(dir); 

    vector<vector<string>> dir_data;
    while(entity != NULL){

        vector<string> dir_entry;

        struct stat file_metadata;
		stat((PWD+entity->d_name).c_str(), &file_metadata);

        dir_entry.push_back(entity->d_name);
        dir_entry.push_back(convertSize(file_metadata.st_size));
        dir_entry.push_back(permissions(file_metadata.st_mode));
        if(file_metadata.st_uid!=0)dir_entry.push_back("amit");
        else dir_entry.push_back("root");
        if(file_metadata.st_gid!=0)dir_entry.push_back("amit");
        else dir_entry.push_back("root");
        dir_entry.push_back(ctime(&file_metadata.st_mtime));
        dir_data.push_back(dir_entry);

        entity=readdir(dir);
    }
    closedir(dir);

    vector<float> spacing={0.3, 0.1, 0.1, 0.06, 0.06};
    vector<float> offset_cursor ={7, 3, 3, 2, 2};
    vector<float> offset_normal ={5, 3, 3, 2, 2};

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int upper_limit=0, lower_limit=w.ws_row;
    int key, widt=w.ws_col, heig=w.ws_row, window_size=w.ws_row-8;
    int cursor_position=0;
    system("clear");

    while(1){
    	system("clear");
        int i=upper_limit;
        while(i<upper_limit+window_size && i<dir_data.size()){
            if(cursor_position==i){
                cout.setf(ios::left);
                cout<<" ➤  ";
                for(int pr=0; pr<5; pr++){
                    cout.width((int)(widt*spacing[pr])+offset_cursor[pr]);
                    if(pr==0 && dir_data[i][pr].size()>(int)(widt*spacing[pr]+offset_cursor[pr])-5){
                        cout<<dir_data[i][pr].substr(0,(int)(widt*spacing[pr])+offset_cursor[pr]-8);
                    }
                    else cout<<dir_data[i][pr];
                }
                cout<<dir_data[i][5];
            }
            else{
                cout.setf(ios::left);
                cout<<"      ";
                for(int pr=0; pr<5; pr++){
                    cout.width((int)(widt*spacing[pr])+offset_normal[pr]);
                    if(pr==0 && dir_data[i][pr].size()>(int)(widt*spacing[pr])+offset_normal[pr]-5){
                        cout<<dir_data[i][pr].substr(0,(int)(widt*spacing[pr])+offset_normal[pr]-8);   
                    }
                    else cout<<dir_data[i][pr];
                }
                cout<<dir_data[i][5];
            }
            i++;
        }
        cout<<"\n\n<<<<<< NORMAL MODE >>>>>>"<<"    "<<"CURRENT DIRECTORY : "<<PWD<<"\n";
        cout<<"PRESS ':' FOR COMMAND MODE";

        while(1){
        	struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            if(widt != w.ws_col || heig != w.ws_row){
                heig = w.ws_row;
                widt = w.ws_col;
                if(window_size>heig-4){
                    if(cursor_position>upper_limit+heig-5){
                        window_size = heig-4;
                        lower_limit = cursor_position+1;
                        upper_limit = lower_limit-window_size;
                    }
                    window_size = heig-4;
                    lower_limit = upper_limit+window_size;
                }
                else if(window_size<=heig-4){
                    window_size = heig-4;
                    lower_limit = upper_limit+window_size;
                    if(dir_data.size()-upper_limit<window_size){
                        if(window_size-dir_data.size()+upper_limit<=upper_limit)upper_limit-=(window_size-dir_data.size()+upper_limit);
                        else upper_limit=0;
                    }
                }
                break;
            }
			if(kbhit()){  
                char c = getch();
                if(c=='B'){
                    if(cursor_position==upper_limit+window_size-1){
                        if(upper_limit+window_size==dir_data.size())break;
                        upper_limit++;
                        if(cursor_position<dir_data.size()-1)cursor_position++;
                    }
                    else if(cursor_position<dir_data.size()-1)cursor_position++;
                    break;
                }
                else if(c=='A'){  
                    if(cursor_position==upper_limit){
                        if(upper_limit==0)break;
                        upper_limit--;
                        cursor_position--;
                    }
                    else cursor_position--;
                    break;
                }
                else if(c=='\n'){
                    if(dir_data[cursor_position][2][9]=='y' && dir_data[cursor_position][2]!="rwx------y-"){
                    	while(!ford_stac.empty())ford_stac.pop();
                       
                    	if(dir_data[cursor_position][0]==".");
                    	else if(dir_data[cursor_position][0]==".."){
                            if(PWD==ROOT);
                    		else{
                                back_stac.push(PWD);
                                int bs=PWD.size()-1;
                                while(PWD[--bs]!='/');
                                PWD = PWD.substr(0,bs+1);
                                return;
                            }
                    	}
                    	else{
                    		back_stac.push(PWD);
                    		PWD = PWD+dir_data[cursor_position][0]+'/';
                            return;
                    	} 
                    }
                    else if(dir_data[cursor_position][2][10]=='y'){
                        if(!fork()) execlp("xdg-open", "xdg-open", (PWD+dir_data[cursor_position][0]).c_str(), NULL);
                    }
                }
                else if(c=='h') {
                	back_stac.push(PWD);
                    PWD=HOME;
                    return;
                }
                else if(c=='D' && back_stac.size()>0){
                    string ret = back_stac.top();
                    back_stac.pop();
                    ford_stac.push(PWD);
                    PWD = ret;
                    return;
                }
                else if(c==127){
                	back_stac.push(PWD);
            		int bs=PWD.size()-1;
            		while(PWD[--bs]!='/');
            		PWD = PWD.substr(0,bs+1);
            		return;
                }
                else if(c=='C' && ford_stac.size()>0){
                    string ret = ford_stac.top();
                    ford_stac.pop();
                    back_stac.push(PWD);
                    PWD = ret;
                    return;
                }
                else if(c==':'){
                    MODE=1;
                    return;
                }
                else if(c=='q')exit(0);
            }
        }
    }
}

////////////////////////////////END NORMAL MODE///////////////////////////////////

//////////////////////////////COMMAND MODE FUNCTIONS/////////////////////////////////

bool copy_file(string src, string dest){          ////////// takes a /file.type and into_folder/
    if(dest[dest.size()-1]!='/')dest+="/";
    int i = src.size()-1;
    string filename="";
    while(src[i--]!='/')filename=src[i+1]+filename;
    dest+=filename;
    FILE *og, *cpy;
    og = fopen(src.c_str(), "r");
    cpy = fopen(dest.c_str(), "w");
    if(og==NULL || cpy==NULL)return 1;
    char c;
    while((c=fgetc(og))!=EOF)fputc(c,cpy);
    fclose(og);
    fclose(cpy);
    return 0;
}

bool delete_file(string src){                     ////////// takes a /file.type
    return remove(src.c_str());
}

bool re_name(string src, string new_name){        ////////// takes a /file.type and new_name.txt
    int i = src.size();
    while(src[--i]!='/');
    string file_loc="";
    while(i>=0)file_loc = src[i--]+file_loc;
    file_loc+=new_name;
    return rename(src.c_str(), file_loc.c_str());
}

bool move(string src, string dest){
    dest+="/";
    int i = src.size()-1;
    string file_name = "";
    while(src[i--]!='/')file_name=src[i+1]+file_name;
    dest+=file_name;
    return rename(src.c_str(), dest.c_str());
    
}

bool search(string dir_to_find, string file){
    DIR* dir = opendir(dir_to_find.c_str());
    if (dir == NULL)return false;
    struct dirent* entity;
    entity = readdir(dir);
    bool found=0;
    while(entity != NULL){
        string name(entity->d_name);
        if(name=="." || name=="..");
        else if(name==file)return true;
        else{
            name=dir_to_find+name;
            struct stat file_metadata;
            stat(name.c_str(), &file_metadata);
            if(S_IFDIR & file_metadata.st_mode)found=search(name+"/", file);
            if(found)return true;
            
        }
        entity=readdir(dir);
    }
    closedir(dir);
    return false;
}

bool delete_dir(string dir_to_del){               ////////// takes a /directory that is to be deleted
    DIR* dir = opendir((dir_to_del+"/").c_str());
    if (dir == NULL)cout<<"lol\n";
    struct dirent* entity;
    entity = readdir(dir);
    while(entity!=NULL){
        string name(entity->d_name);
        if(name=="." || name=="..");
        else{
            name = dir_to_del+"/"+name;
            struct stat file_metadata;
            stat(name.c_str(), &file_metadata);

            if(S_IFDIR & file_metadata.st_mode) delete_dir(name);
            else delete_file(name);
        }
        entity=readdir(dir);
    }
    return remove(dir_to_del.c_str());
}

bool make_file(string file_to_make){              ////////// takes a /file.type that doesnt exist
    FILE *fp;
    fp = fopen(file_to_make.c_str(), "a+");
    if(fp==NULL)return 1;
    fclose(fp);
    return 0;
}

bool make_dir(string dir_to_make){                ////////// takes a /folder_name that doesnt exist
    int result = mkdir(dir_to_make.c_str(), 0777);
    if(result==0)return true;
    return false;
}

bool copy_dir(string src, string dest){           ////////// takes a /folder_name and dest_folder/
    dest+="/";
    int i = src.size()-1;
    while(src[i--]!='/');
    string outer_dir=src.substr(i+2,src.size()-1);
    dest+=outer_dir;
    make_dir(dest.c_str());
    dest+="/";
    src+="/";
    DIR* dir = opendir(src.c_str());
    if (dir == NULL)return 1;
    struct dirent* entity;
    entity = readdir(dir);

    while(entity != NULL){
        string name(entity->d_name);
        if(name=="." || name=="..");
        else{
            name=src+name;
            struct stat file_metadata;
            stat(name.c_str(), &file_metadata);

            if(S_IFDIR & file_metadata.st_mode) copy_dir(name, dest);
            else copy_file(name, dest);
        }
        entity=readdir(dir);
    }
    return 0;
}

bool move_dir(string src, string dest){
    copy_dir(src, dest);
    return delete_dir(src);
}

////////////////////////////END COMMAND MODE FUNCTIONS///////////////////////////////

//////////////////////////////COMMAND MODE////////////////////////////////

string parse_control(string command){
    vector<string> command_args;
    int i=0,j=0;
    while(command[++j]!=' ');
    string op_code = command.substr(i,j);
    i=++j;
    while(j<=command.size()){
        if(command[j]==' '){
            command_args.push_back(command.substr(i,j-i));
            j++;
            i=j;
        }
        else j++;
    }
    command_args.push_back(command.substr(i,command.size()-i));
    
    for(int op=0; op<(int)command_args.size(); op++){
        if(command_args[op][0]=='/');
        else if(command_args[op][0]=='~'){
            char *no_gu = new char[PATH_MAX];
            string gu = HOME+command_args[op].substr(2,command_args[op].size());
            realpath(gu.c_str(),no_gu);
            command_args[op] = string(no_gu);
            delete[] no_gu;
        }
        else {
            char *no_gu = new char[PATH_MAX];
            string gu = PWD+command_args[op];
            realpath(gu.c_str(),no_gu);
            command_args[op] = string(no_gu);
            delete[] no_gu;
        }
    }

    bool result=0;
    int end = command_args.size()-1;

    if (op_code=="copy"){
        cout<<"enters here\n";
        for(int work=0; work<end; ++work) result = result || copy_file(command_args[work], command_args[end]);
        if(!result)return "SUCCESS";
        else return "FAILED";
    }
    else if (op_code=="copy_dir"){
        for(int work=0; work<end; ++work) result = result || copy_dir(command_args[work], command_args[end]);
        if(!result)return "SUCCESS";
        else return "FAILED";
    }
    else if (op_code=="move"){
        for(int work=0; work<end; ++work) result = result || move(command_args[work], command_args[end]);
        if(!result)return "SUCCESS";
        else return "FAILED";
    }
    else if (op_code=="rename"){
        for(int work=0; work<end; ++work) result = result || re_name(command_args[work], command_args[end]);
        if(!result)return "SUCCESS";
        else return "FAILED";
    }
    else if (op_code=="create_file"){
        for(int work=0; work<end; ++work) result = result || make_file(command_args[end]+command_args[work]);
        if(!result)return "SUCCESS";
        else return "FAILED";
    }
    else if (op_code=="create_dir"){
        for(int work=0; work<end; ++work) result = result || make_dir(command_args[end]+command_args[work]);
        if(!result)return "SUCCESS";
        else return "FAILED";
    }
    else if (op_code=="delete_file"){
        for(int work=0; work<=end; ++work) result = result || delete_file(command_args[work]);
        if(!result)return "SUCCESS";
        else return "FAILED";
    }
    else if (op_code=="delete_dir"){
        for(int work=0; work<=end; ++work) result = result || delete_dir(command_args[work]);
        if(!result)return "SUCCESS";
        else return "FAILED";
    }
    else if (op_code=="search"){
        for(int work=0; work<=end; ++work){
            int lo = command_args[work].size();
            while(command_args[work][--lo]!='/')
            result = search(PWD, command_args[work].substr(lo, command_args[work].size()-lo));
        }
        if(result)return "PRESENT";
        else return "NOT PRESENT";
    }
    else if (op_code=="goto"){
        while(ford_stac.size()>0)ford_stac.pop();
        back_stac.push(PWD);
        if(command_args[0][command_args[0].size()-1]=='/')PWD = command_args[0];
        else PWD = command_args[0]+"/";
        return "SUCCESS";
    }
    else return "Invaliad Operation";
}

void command_mode(){
    DIR* dir = opendir(PWD.c_str());
    if (dir == NULL)cout<<"lol";
    struct dirent* entity;
    entity = readdir(dir); 

    vector<vector<string>> dir_data;
    while(entity != NULL){

        vector<string> dir_entry;

        struct stat file_metadata;
        stat((PWD+entity->d_name).c_str(), &file_metadata);

        dir_entry.push_back(entity->d_name);
        dir_entry.push_back(convertSize(file_metadata.st_size));
        dir_entry.push_back(permissions(file_metadata.st_mode));
        if(file_metadata.st_uid!=0)dir_entry.push_back("amit");
        else dir_entry.push_back("root");
        if(file_metadata.st_gid!=0)dir_entry.push_back("amit");
        else dir_entry.push_back("root");
        dir_entry.push_back(ctime(&file_metadata.st_mtime));
        dir_data.push_back(dir_entry);

        entity=readdir(dir);
    }
    closedir(dir);

    vector<float> spacing={0.3, 0.1, 0.1, 0.06, 0.06};
    vector<float> offset_cursor ={7, 3, 3, 2, 2};
    vector<float> offset_normal ={5, 3, 3, 2, 2};

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int upper_limit=0, lower_limit=w.ws_row;
    int key, widt=w.ws_col, heig=w.ws_row, window_size=w.ws_row-8;
    int cursor_position=0;
    system("clear");

    string command="";
    string result;

    while(1){
        system("clear");
        int i=upper_limit;
        while(i<upper_limit+window_size && i<dir_data.size()){
            cout.setf(ios::left);
                cout<<"      ";
                for(int pr=0; pr<5; pr++){
                    cout.width((int)(widt*spacing[pr])+offset_normal[pr]);
                    if(pr==0 && dir_data[i][pr].size()>(int)(widt*spacing[pr])+offset_normal[pr]-5){
                        cout<<dir_data[i][pr].substr(0,(int)(widt*spacing[pr])+offset_normal[pr]-8);   
                    }
                    else cout<<dir_data[i][pr];
                }
            cout<<dir_data[i][5];
            i++;
        }
        cout<<"\n\n>>>>>> COMMAND MODE <<<<<<"<<"    "<<"CURRENT DIRECTORY ::: "<<PWD<<"\n";
        cout<<RESULT;
        cout<<"\n"<<command;

        while(1){
            struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            if(widt != w.ws_col || heig != w.ws_row){
                heig = w.ws_row;
                widt = w.ws_col;
                if(window_size>heig-4){
                    if(cursor_position>upper_limit+heig-5){
                        window_size = heig-4;
                        lower_limit = cursor_position+1;
                        upper_limit = lower_limit-window_size;
                    }
                    window_size = heig-4;
                    lower_limit = upper_limit+window_size;
                }
                else if(window_size<=heig-4){
                    window_size = heig-4;
                    lower_limit = upper_limit+window_size;
                    if(dir_data.size()-upper_limit<window_size){
                        if(window_size-dir_data.size()+upper_limit<=upper_limit)upper_limit-=(window_size-dir_data.size()+upper_limit);
                        else upper_limit=0;
                    }
                }
                break;
            }
            if(kbhit()){  
                char c = getch();
                if(c==127 && command.size()>0){
                    command.pop_back();
                    break;
                }
                else if(c==27){
                    MODE=0;
                    return;
                }
                else if(c=='\n'){
                    if(command=="quit") exit(0);
                    RESULT = parse_control(command);
                    return;
                }
                else{
                    command+=c;
                    break;
                }
            }
        }
    }
}

/////////////////////////////////END COMMAND MODE/////////////////////////////////////

int main(){
    while(1){
        if(!MODE)normal_mode();
        else command_mode();
    }
    return 0;
}