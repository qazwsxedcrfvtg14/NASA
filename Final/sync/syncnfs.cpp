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

        string nfslist=FileToStr("/etc/autofs.config/list.txt");

        stringstream file(nfslist);
        string user;
        struct stat sb;
        system("mkdir /mnt/nfs/sync");
        while( file>>user ){
                //cout<<user<<endl;
                string nfs1, nfs2;
                file>>nfs1>>nfs2;
                if(nfs2!=host_name)continue;
                string mnt ="/mnt/nfs/sync";
                system(("mount -t nfs "+nfs1+":var/nfs "+mnt).c_str());
                system(("rsync -a /mnt/nfs/sync/"+user+" /var/nfs_backup/").c_str());
                system(("umount "+mnt).c_str());
        }
        system("rm -rf /mnt/nfs/sync");
}

