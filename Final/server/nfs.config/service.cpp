#include <algorithm>
#include <iostream>
#include <cstring>
#include <random>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <sstream>
#include <unistd.h>
using namespace std;

typedef pair<int,int> pr;
#define f first
#define s second
struct stat st = {0};
string FileToStr(const char *fil){
    fstream fin;
    fin.open(fil,ios::in|ios::binary);
    char c;
    string ss;
    while(fin.get(c))
        ss+=c;
    fin.close();
    return ss;
    }
int main(int argc, char *argv[]){
    string host_name;
    if(argc>=2)
        host_name=argv[1];
    else
        exit(1);

    string change="";
    while(true){
        string chg=FileToStr("/etc/autofs.config/change.txt");
        if(change==chg){
            sleep(20);
            continue;
            }
        change=chg;
        //fstream file;
        //string filename = "/etc/autofs.config/change.txt";
        //file.open(filename, fstream::in);
        {
        stringstream file(change);
        string op;
        struct stat sb;
        while(file>>op){
            if(op=="add"){
                string group,m1,m2;
                file>>group>>m1>>m2;
                if(m1==host_name){
                    string dir="/var/nfs/";
                    dir+=group;
                    if(!(stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
                        system(("mkdir -m 777 "+dir).c_str());
                        }
                    }
                if(m2==host_name){
                    string dir="/var/nfs_backup/";
                    dir+=group;
                    if(!(stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
                        system(("mkdir -m 777 "+dir).c_str());
                        }
                    }
                }
            else if(op=="del"){
                string group,m1,m2;
                file>>group>>m1>>m2;
                if(m1==host_name){
                    string dir="/var/nfs/";
                    dir+=group;
                    if((stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
                        system(("rm -rf "+dir).c_str());
                        }
                    }
                if(m2==host_name){
                    string dir="/var/nfs_backup/";
                    dir+=group;
                    if((stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
                        system(("rm -rf "+dir).c_str());
                        }
                    }
                }
            else if(op=="move"){
                string group,m1,m2,n1,n2;
                file>>group>>m1>>m2>>n1>>n2;
                if(n1==host_name&&n1!=m1){
                    string dir="/var/nfs/";
                    dir+=group;
                    if(!(stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
                        string mnt="/mnt/nfs/";
                        mnt+=m1;
                        system("mkdir /mnt/nfs");
                        system(("mkdir "+mnt).c_str());
                        system(("rm -rf "+dir).c_str());
                        system(("mkdir -m 777 "+dir).c_str());
                        system(("mount -t nfs "+m1+":/var/nfs "+mnt).c_str());
                        system(("cp -rfp "+mnt+"/"+group+"/. "+dir).c_str());
                        system(("rm -rf "+mnt+"/"+group).c_str());
                        system(("umount "+mnt).c_str());
                        }
                    }
                if(n2==host_name&&n2!=m2){
                    string dir="/var/nfs_backup/";
                    dir+=group;
                    if(!(stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
                        string mnt="/mnt/nfs/";
                        mnt+=m2;
                        system("mkdir /mnt/nfs");
                        system(("mkdir "+mnt).c_str());
                        system(("rm -rf "+dir).c_str());
                        system(("mkdir -m 777 "+dir).c_str());
                        system(("mount -t nfs "+m2+":/var/nfs_backup "+mnt).c_str());
                        system(("cp -rfp "+mnt+"/"+group+"/. "+dir).c_str());
                        system(("rm -rf "+mnt+"/"+group).c_str());
                        system(("umount "+mnt).c_str());
                        }
                    }

                }
            else{
                throw "undefined op!";
                }
            }
        }
        system("systemctl reload autofs");
        {
        stringstream file(change);
        string op;
        struct stat sb;
        while(file>>op){
            if(op=="add"){
                string group,m1,m2;
                file>>group>>m1>>m2;
                }
            else if(op=="del"){
                string group,m1,m2;
                file>>group>>m1>>m2;
                system(("umount -l /home/nfs/"+group).c_str());
                }
            else if(op=="move"){
                string group,m1,m2,n1,n2;
                file>>group>>m1>>m2>>n1>>n2;
                system(("umount -l /home/nfs/"+group).c_str());
                }
            else{
                throw "undefined op!";
                }
            }
        }
        //system("systemctl restart autofs");
        }
    //file.close();
    }
