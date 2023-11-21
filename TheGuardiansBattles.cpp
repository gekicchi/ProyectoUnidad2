#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
using namespace std;

struct Guardian
{
	string Name;
	int Level;
	string Master;
	string City;
	
	Guardian(string name, int level, string master, string city) : Name(name), Level(level), Master(master), City(city) {}
};
struct GeneralGuardian
{
	Guardian *Info;
	vector<GeneralGuardian*> Apprentices;
	
	GeneralGuardian(Guardian *info) : Info(info) {}
};

struct RankedGuardian
{
	Guardian *Info;
	RankedGuardian *left;
	RankedGuardian *right;
	
	RankedGuardian(Guardian *info) : Info(info), left(nullptr), right(nullptr) {}
};

class Hierarchy
{
	public:
		Hierarchy() { root = nullptr; }
		~Hierarchy() {}
		
		void Insert(GeneralGuardian *guardian) { InsertGuardian(root, guardian); }
		void InsertGuardian(GeneralGuardian *current, GeneralGuardian *newGuardian);
		
		bool Search(int level) { return SearchGuardian(root, level); }
		bool SearchGuardian(GeneralGuardian *current, int level);
		
		void Print() { PrintHierarchy(root, 0); }
		void PrintHierarchy(GeneralGuardian *current, int depth);
		
		void Delete() {}
		void DeleteGuardian(GeneralGuardian *current, string name);
		
	private:
		GeneralGuardian *root;
};

class Ranking
{
	public:
		Ranking() { root = nullptr; }
		~Ranking() {}
		
		void Insert(RankedGuardian *guardian) { root = InsertGuardian(root, guardian); }
		RankedGuardian* InsertGuardian(RankedGuardian *current, RankedGuardian *newGuardian);
		
		bool Search(int level) { return SearchGuardian(root, level); }
		bool SearchGuardian(RankedGuardian *current, int level);
			
		void Print() { PrintRanking(root); }
		void PrintRanking(RankedGuardian *current);
		
	private:
		RankedGuardian *root;
};

class Map
{
	public:
		Map() {}
		~Map() {}
		
		void AddPath(int start, int end);
		bool AreAdjacent(int start, int end);
		bool FindPath(int start, int end);
		
	private:
		vector<vector<pair<int>>> paths;
		unordered_map<int, string> cityNames;
};

void ReadGuardiansText(char *file_name, Ranking *ranking, Hierarchy *hierarchy);

int main(int argc, char *argv[])
{
	int election;
	
	Ranking ranking;
	Hierarchy hierarchy;
	
	ReadGuardiansText(argv[1], &ranking, &hierarchy);
	
	ranking.Print();
	cout << endl << endl << endl;
	hierarchy.Print();
	
	do{
		cout << "----The Guardian's Battles----" << endl;
		cout << "[1] Ver Lista de Candidatos" << endl << "[2] Ver Guardian" << endl << "[3] Conocer el Reino" << endl << "[4] Presenciar una Batalla" << endl << "[5] Salir" << endl;
		cout << "Eleccion: ";
		cin >> election;
		
		switch(election)	
		{
			case 1: // ver lista de candidatos
				cout << "LISTA DE CANDIDATOS" << endl;
				ranking.Print();
				break;
				
			case 2: // ver guardian
				cout << "INFO DE GUARDIAN" << endl;
				break;
				
			case 3: // conocer reino
				cout << "CONOCIENDO REINO" << endl;
				break;
				
			case 4: // presenciar batalla
				cout << "PRESENCIANDO BATALLA" << endl;
				break;
		}
		cout << endl;
	} while(election != 5);
	
	return 0;
}

/**** FUNCIONES PARA LA JERARQUIA ****/
// insertar guardian
void Hierarchy::InsertGuardian(GeneralGuardian *current, GeneralGuardian *newGuardian)
{
	if (root == nullptr)
	{
		root = newGuardian;
	}
	else
	{
		if (newGuardian->Info->Master.compare(current->Info->Name) == 0)
			current->Apprentices.push_back(newGuardian);
		else
			for (GeneralGuardian* apprentice : current->Apprentices)
				InsertGuardian(apprentice, newGuardian);
	}
}

// imprimir jerarquia
void Hierarchy::PrintHierarchy(GeneralGuardian *current, int depth)
{
	for (int i=0; i<depth; i++)
		cout << "  ";
		
	cout << "|-- Nombre: " << current->Info->Name << " | Ciudad: " << current->Info->City << " | Nivel: " << current->Info->Level << endl;
	
	for (GeneralGuardian *apprentices : current->Apprentices)
		PrintHierarchy(apprentices, depth+1);
}

/**** FUNCIONES PARA EL RANKING ****/
// insertar guardian
RankedGuardian* Ranking::InsertGuardian(RankedGuardian *current, RankedGuardian *newGuardian)
{
	if (current == nullptr)
		return newGuardian;
		
	if (current->Info->Level > newGuardian->Info->Level)
		current->left = InsertGuardian(current->left, newGuardian);
	else
		current->right = InsertGuardian(current->right, newGuardian);
	
	return current;
}

// imprimir ranking
void Ranking::PrintRanking(RankedGuardian *current)
{
	if (current == nullptr)
		return;
		
	PrintRanking(current->right);
	//cout << "Nombre: " << current->Info->Name << " | Ciudad: " << current->Info->City << " | Nivel: " << current->Info->Level << endl;
	cout << current->Info->Level << ") " << current->Info->Name << endl;
	PrintRanking(current->left);
}

/**** FUNCIONES PARA EL MAPA ****/
void AddPath(int start, int end)
{
	paths[start].push_back(end);
}
bool AreAdjacent(int start, int end)
{
	for (int adj : paths[start])
		if (adj == end)
			return true;
			
	return false;
}
bool FindPath(int start, int end)
{
	
	
	return false;
}

/**** RESTO DE FUNCIONES ****/
// lee archivo de guardianes
void ReadGuardiansText(char *file_name, Ranking *ranking, Hierarchy *hierarchy)
{
	ifstream guardians(file_name);
	
	if (guardians.is_open())
	{
		string line;
		while (getline(guardians, line))
		{
			istringstream ss(line);
			string chunk;
			vector<string> chunks;
			
			while (getline(ss,chunk,','))
				chunks.push_back(chunk);
				
			string city = chunks.back();
			chunks.pop_back();
			
			string master = chunks.back();
			chunks.pop_back();
			
			int level = stoi(chunks.back());
			chunks.pop_back();
			
			string name = chunks.back();
			chunks.pop_back();
		
			Guardian *newGuardian = new Guardian(name, level, master, city);
			RankedGuardian *newRanking = new RankedGuardian(newGuardian);
			GeneralGuardian *newHierarchy = new GeneralGuardian(newGuardian);
			
			ranking->Insert(newRanking);
			hierarchy->Insert(newHierarchy);
		}
	}
	guardians.close();
}

void ReadCitiesFile(char *file_name, Map *map)
{
	ifstream cities(file_name);
	
	if (cities.is_open())
	{
		
	}
	cities.close();
}
