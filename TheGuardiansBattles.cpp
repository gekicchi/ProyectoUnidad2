#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <stack>
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
		Map(vector<vector<int>> p) { paths = p;}
		~Map() {}
		
		void setCityNames(int num, string name) { cityNames[num] = name; }
		unordered_map<int, string> getCityNames() { return cityNames; }
		
		void AddPath(int start, int end);
		bool AreAdjacent(int start, int end);
		bool FindPath(int start, int end);
		void Print();
		
	private:
		vector<vector<int>> paths;
		unordered_map<int, string> cityNames;
};

void ReadGuardiansFile(char *file_name, Ranking *ranking, Hierarchy *hierarchy);
void ReadCitiesFile(char *file_name, Map *map);
void StartEndPath(Map map, int *start, int *end);

int main(int argc, char *argv[])
{
	int election, subElection, start, end;
	
	Ranking ranking;
	Hierarchy hierarchy;
	vector<vector<int>> list(20);
	Map map(list);
	
	ReadGuardiansFile(argv[1], &ranking, &hierarchy);
	ReadCitiesFile(argv[2], &map);
	
	ranking.Print();
	cout << endl << endl << endl;
	hierarchy.Print();
	cout << endl << endl << endl;
	map.Print();
	
	map.FindPath(0,2);
	map.FindPath(3,10);
	
	do{
		cout << "----The Guardian's Battles----" << endl;
		cout << "[1] Ver Lista de Candidatos" << endl << "[2] Ver Guardian" << endl << "[3] Conocer el Reino" << endl << "[4] Presenciar una Batalla" << endl << "[5] Salir" << endl;
		cout << "Eleccion: ";
		cin >> election;
		cout << endl;
		
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
				cout << "[1] Construir Camino\n[2] Consultar Camino";
				cout << "Eleccion: ";
				cin >> subElection;
				switch (subElection)
				{
					case 1: // construir camino
						StartEndPath(map, &start, &end);
						map.AddPath(start, end);
						break;
						
					case 2: // consultar camino
						StartEndPath(map, &start, &end);
						map.FindPath(start, end);
						break;
				}
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
	
	cout << current->Info->Level << ") " << current->Info->Name;
	if (current->Info->Level >= 90 && current->Info->Level < 100)
		cout << " (Candidato a Guardian)";
	cout << endl;
	
	PrintRanking(current->left);
}

/**** FUNCIONES PARA EL MAPA ****/
void Map::AddPath(int start, int end)
{
	if (start == end)
		return;
		
	paths[start].push_back(end);
}

bool Map::AreAdjacent(int start, int end)
{
	for (int adj : paths[start])
		if (adj == end)
			return true;
			
	return false;
}

bool Map::FindPath(int start, int end)
{
	vector<bool> visited(cityNames.size(), false);
	vector<int> parent(cityNames.size(),-1);
	vector<int> path;
	stack<int> stack;
	
	stack.push(start);
	visited[start] = true;
	
	while(!stack.empty())
	{
		int curr = stack.top();
		stack.pop();
				
		if (curr == end)
		{
			for (int i=end; i != -1; i = parent[i])
				path.push_back(i);
						
			for (int i=path.size()-1; i>-1; i--)
			{
				cout << cityNames[path[i]];
				if (i>0)
					cout << " -> ";
			}
			
			cout << endl;
			return true;
		}
				
		for (int adj : paths[curr])
		{
			if (!visited[adj])
			{
				stack.push(adj);
				visited[adj] = true;
				parent[adj] = curr;
			}
		}
	}
	
	cout << "No Existe Camino entre " << cityNames[start] << " y " << cityNames[end] << endl;
	return false;
}

void Map::Print()
{
	for (int i=0; i<cityNames.size(); i++)
	{
		cout << "[" << i << "] " << cityNames[i] << ": ";
		for (int adj : paths[i])
			cout << cityNames[adj] << "  ";
		cout << endl;
	}
}

/**** RESTO DE FUNCIONES ****/
// lee archivo de guardianes
void ReadGuardiansFile(char *file_name, Ranking *ranking, Hierarchy *hierarchy)
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
		string line;
		while (getline(cities, line))
		{
			istringstream ss(line);
			string chunk;
			vector<string> chunks;
			int cityPos, connectionPos;
			bool cityExists = false, connectionExists = false;
			
			while (getline(ss,chunk,','))
				chunks.push_back(chunk);
			
			string connection = chunks.back();
			chunks.pop_back();
			
			string city = chunks.back();
			chunks.pop_back();
			
			for (int i=0; i<map->getCityNames().size(); i++)
			{
				if (cityExists && connectionExists)
					break;
				
				if (map->getCityNames()[i] == city )
				{
					cityPos = i;
					cityExists = true;
				}
					
				if (map->getCityNames()[i] == connection)
				{
					connectionPos = i;
					connectionExists = true;
				}
			}
			
			if (!cityExists)
			{
				cityPos = map->getCityNames().size();
				map->setCityNames(map->getCityNames().size(), city);
			}
			
			if (!connectionExists)
			{
				connectionPos = map->getCityNames().size();
				map->setCityNames(map->getCityNames().size(), connection);
			}
			
			map->AddPath(cityPos, connectionPos);
		}
	}
	cities.close();
}

void StartEndPath(Map map, int *start, int *end)
{
	cout << "Inicio de Camino" << endl;
	map.Print();
	cout << endl<< "Eleccion: ";
	cin >> *start;
	
	cout << "Final de Camino" << endl;
	map.Print();
	cout << endl << "Eleccion: ";
	cin >> *end;
}
