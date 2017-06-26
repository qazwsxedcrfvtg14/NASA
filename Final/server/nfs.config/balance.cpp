#include <algorithm>
#include <iostream>
#include <cstring>
#include <random>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef pair<int,int> pr;
#define f first
#define s second

vector<int> primary, secondary;
vector<int> _primary, _secondary;
vector<pr> machine_pairs;

vector<char*> args;
vector<string> groups, machines;
vector<string> _groups, _machines;

void init();
void add_group();
void delete_group();
void add_machine();
void delete_machine();
void help();
void show();

typedef void (*handler)();
vector<string> options = {"-i", "+g", "-g", "+m", "-m", "-s"};
vector<handler> handlers = {init, add_group, delete_group, add_machine, delete_machine, show};

vector<int> load;
vector<int> change;

bool load_cmp(int a, int b);

int my_rand()
{
	return rand()>>3;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

    for (int i = 2; i < argc; i++)
        args.push_back(argv[i]);

    bool handled = false;

    for (int i = 0; i < options.size(); i++)
        if ( argc >= 2 && string(argv[1]) == options[i] )
            handled = true, handlers[i]();

    if ( !handled ) help();
}

bool load_cmp(int a, int b)
{
    return load[a] < load[b];
}

void read_groups_and_machines()
{
    fstream file;
    string filename;
    string group, machine;

    filename = "group.txt";
    file.open(filename, fstream::in);
    while ( file>>group ) groups.push_back(group);
    file.close();

    filename = "machine.txt";
    file.open(filename, fstream::in);
    while ( file>>machine ) machines.push_back(machine);
    file.close();
	_groups = groups;
	_machines = machines;
}

void write_groups_and_machines()
{
    fstream file;
    string filename;
    string group, machine;

    filename = "group.txt";
    file.open(filename, fstream::out|fstream::trunc);
    for (auto group : groups) file<<group<<endl;
    file.close();

    filename = "machine.txt";
    file.open(filename, fstream::out|fstream::trunc);
    for (auto machine : machines) file<<machine<<endl;
    file.close();
}

int group_name_to_id(string name)
{
    for (int gi = 0; gi < groups.size(); gi++)
        if ( name == groups[gi] ) return gi;

    return -1;
}

int machine_name_to_id(string name)
{
    for (int mi = 0; mi < machines.size(); mi++)
        if ( name == machines[mi] ) return mi;

    return -1;
}

void read_list()
{
    fstream file;
    string filename = "list.txt";
    file.open(filename, fstream::in);

    string group, machine, backup;
    primary.resize(groups.size() );
    secondary.resize(groups.size() );
    load.resize(machines.size() );

    for (int gi = 0; gi < groups.size(); gi++) {
        file>>group>>machine>>backup;
        int gid = group_name_to_id(group);
        primary[gid] = machine_name_to_id(machine);
        secondary[gid] = machine_name_to_id(backup);
        load[primary[gid] ]++;
    }

    file.close();

	_primary = primary;
	_secondary = secondary;
}

void write_list()
{
    fstream file;
    string filename;

    filename = "list.txt";
    file.open(filename, fstream::trunc|fstream::out);

    for (int gi = 0; gi < groups.size(); gi++)
        file<<groups[gi]<<" "<<machines[primary[gi] ]<<" "<<machines[secondary[gi] ]<<endl;

    file.close();

	filename = "auto.nfs";
    file.open(filename, fstream::trunc|fstream::out);

    for (int gi = 0; gi < groups.size(); gi++)
        file<<groups[gi]<<" -rw,fg,soft,use-weight-only "<<machines[primary[gi] ]<<"(1):/var/nfs/"<<groups[gi]<<" "<<machines[secondary[gi] ]<<"(2):/var/nfs_backup/"<<groups[gi]<<endl;
    file.close();
}

void init()
{
    read_groups_and_machines();
    int population = groups.size();

    for (int mi = 0; mi < machines.size(); mi++) {
        int unbackuped;
        if ( population%(machines.size()-mi) ) {
            unbackuped = population/(machines.size()-mi)+1;
        } else {
            unbackuped = population/(machines.size()-mi);
        }
        population -= unbackuped;

        int backups = machines.size()-1;
        for (int mj = machines.size()-1; mj >= 0; mj--) {
            if ( mi == mj ) continue;

            int residence;
            if ( unbackuped%backups ) residence = unbackuped/backups+1;
            else residence = unbackuped/backups;
            unbackuped -= residence;
            backups--;

            for (int ri = 0; ri < residence; ri++)
                machine_pairs.push_back(pr(mi,mj));
        }
    }

    random_shuffle(machine_pairs.begin(), machine_pairs.end());

    for (int gi = 0; gi < groups.size(); gi++)
        primary.push_back(machine_pairs[gi].f),
        secondary.push_back(machine_pairs[gi].s);

    write_list();
}

void add_group()
{
    fstream file;
    string filename;

    filename = "change.txt";
    file.open(filename, fstream::app|fstream::out);

    read_groups_and_machines();
    read_list();

    bool OK = true;
    for (string group: args)
        if ( group_name_to_id(group) != -1 ) {
            cout<<"group "<<group<<" already exists!"<<endl;
            OK = false;
        }

    if ( !OK ) return;

    vector<int> pool;
    for (int mi = 0; mi < machines.size(); mi++)
        pool.push_back(mi);

    for (string group:args){
        groups.push_back(group);
        sort(pool.begin(), pool.end(), load_cmp);
        primary.push_back(pool[0]);
        load[pool[0] ]++;
		int machine = my_rand()%machines.size();
		while ( machine == pool[0] )
	    	machine = my_rand()%machines.size();
        secondary.push_back(machine);
		file<<"add "<<group<<" "<<machines[pool[0] ]<<" "<<machines[machine]<<endl;
    }

    write_groups_and_machines();
    write_list();

	file.close();
}

void delete_group()
{
    fstream file;
    string filename;

    filename = "change.txt";
    file.open(filename, fstream::app|fstream::out);
    read_groups_and_machines();
    read_list();

    bool OK = true;
    for (string group: args)
        if ( group_name_to_id(group) == -1 ) {
            cout<<"group "<<group<<" doesn't exist!"<<endl;
            OK = false;
        }

    if ( !OK ) return;

    for (string group: args) {
        int gid = group_name_to_id(group);
        swap(groups[gid], groups[groups.size()-1]);
        swap(primary[gid], primary[groups.size()-1]);
        swap(secondary[gid], secondary[groups.size()-1]);
		file<<"del "<<group<<" "<<machines[primary[groups.size()-1] ]<<" "<<machines[secondary[groups.size()-1] ]<<endl;
        groups.pop_back();
    }

    write_groups_and_machines();
    write_list();
	file.close();
}


bool move_away_cmp(int a, int b)
{
	return load[primary[a] ] > load[primary[b] ];
}

int rand_by_size()
{
	int sum = 0;
	for (int gi = 0; gi < groups.size(); gi++)
		sum += load[primary[gi] ];
	
	int rd = my_rand()%sum, tmp = 0;
	for (int gi = 0; gi < groups.size(); gi++) {
		tmp += load[primary[gi] ];
		if ( tmp > rd ) return gi;
	}
}

void add_machine()
{
    fstream file;
    string filename;

    filename = "change.txt";
    file.open(filename, fstream::app|fstream::out);
    read_groups_and_machines();
    read_list();

	bool OK = true;
	for (string machine: args)
		if ( machine_name_to_id(machine) != -1 ) {
			cout<<"machine "<<machine<<" already exists!"<<endl;
			OK = false;
		}

	if ( !OK ) return;

	for (string machine: args) {
		machines.push_back(machine);

		for (int i = 0; i < groups.size()/machines.size(); i++) {
			int group = rand_by_size()%groups.size();
			while ( primary[group] == machines.size()-1 || secondary[group] == machines.size()-1 )
				group = rand_by_size()%groups.size();
			primary[group] = machines.size()-1;
			change.push_back(group);
		}

		for (int i = 0; i < groups.size()/machines.size(); i++) {
			int group = my_rand()%groups.size();
			while ( primary[group] == machines.size()-1 || secondary[group] == machines.size()-1 )
				group = my_rand()%groups.size();
			secondary[group] = machines.size()-1;
			change.push_back(group);
		}
	}

	write_groups_and_machines();
    write_list();

	sort(change.begin(), change.end());
	change.resize(unique(change.begin(), change.end())-change.begin() );

	for (auto group: change) {
		file<<"move "<<_groups[group]<<" "<<_machines[_primary[group] ]<<" "<<_machines[_secondary[group] ]<<" ";
		file<<machines[primary[group] ]<<" "<<machines[secondary[group] ]<<endl;
	}

	file.close();
}

void delete_machine()
{
    fstream file;
    string filename;

    filename = "change.txt";
    file.open(filename, fstream::app|fstream::out);
    read_groups_and_machines();
    read_list();

	bool OK = true;
	for (string machine: args)
		if ( machine_name_to_id(machine) == -1 ) {
			cout<<"machine "<<machine<<" doesn't exist!"<<endl;
			OK = false;
		}

	if ( !OK ) return;
    

	for (string machine: args) {
		int mid = machine_name_to_id(machine);

		swap(machines[mid], machines[machines.size()-1]);

		for (int gi = 0; gi < groups.size(); gi++) {
			if ( primary[gi] == mid ) {
				primary[gi] = -1;
				change.push_back(gi);
			} else if ( primary[gi] == machines.size()-1 ) {
				primary[gi] = mid;
			}

			if ( secondary[gi] == mid ) {
				secondary[gi] = -1;
				change.push_back(gi);
			} else if ( secondary[gi] == machines.size()-1 ) {
				secondary[gi] = mid;
			}
		}
	
		vector<int> pool;
    	for (int mi = 0; mi < machines.size()-1; mi++)
        	pool.push_back(mi);

		for (int gi = 0; gi < groups.size(); gi++) {
			if ( primary[gi] == -1 ) {
				sort(pool.begin(), pool.end(), load_cmp);
				if ( pool[0] == secondary[gi] ) primary[gi] = pool[1], load[pool[1] ]++;
				else primary[gi] = pool[0], load[pool[0] ]++; 
			} else if ( secondary[gi] == -1 ) {
				int machine = my_rand()%(machines.size()-1);
				while ( primary[gi] == machine )
					machine = my_rand()%(machines.size()-1);
				secondary[gi] = machine;
			}
		}

		machines.pop_back();
	}

	write_groups_and_machines();
    write_list();

	sort(change.begin(), change.end());
	change.resize(unique(change.begin(), change.end())-change.begin() );

	for (auto group: change) {
		file<<"move "<<_groups[group]<<" "<<_machines[_primary[group] ]<<" "<<_machines[_secondary[group] ]<<" ";
		file<<machines[primary[group] ]<<" "<<machines[secondary[group] ]<<endl;
	}

	file.close();
}

void show()
{
	read_groups_and_machines();
	read_list();

	for (int mi = 0; mi < machines.size(); mi++) 
		cout<<machines[mi]<<" "<<load[mi]<<endl;
}

void help()
{
	puts("The following options are available:");
	puts("-i	init, you should provide machine.txt and group.txt and the result will be in list.txt");
	puts("-g	delete a group");
	puts("+g	add a group");
	puts("-m	delete a machine");
	puts("+m	add a machine");
	puts("-s	show balance detail");
}
