#include <iostream>
#include <vector>
using namespace std;

struct GeneralGuardian
{
	string Name;
	int Level;
	string Master;
	string City;
	vector<GeneralGuardian*> Apprentices;
};

struct RankedGuardian
{
	string Name;
	int Level;
	RankedGuardian *left;
	RankedGuardian *right;
};

class Hierarchy
{
	public:
		
	private:
		GeneralGuardian *root;
};

class Ranking
{
	public:
		
	private:
		RankedGuardian *root;
};

class Map
{
	public:
		
	private:
		vector<vector<int>> paths;
};

int main(int argc, char *argv[])
{
	
	
	return 0;
}
