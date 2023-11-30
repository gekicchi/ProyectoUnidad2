#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <stack>
#include <cstdlib>
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

class Map
{
	public:
		Map(vector<vector<int>> p) { paths = p;}
		~Map() {}
		
		void setCityNames(int num, string name) { cityNames[num] = name; }
		unordered_map<int, string> getCityNames() { return cityNames; }
		
		void setCityNumbers(string name, int num) { cityNumbers[name] = num; }
		unordered_map<string, int> getCityNumbers() { return cityNumbers; }
		
		void AddPath(int start, int end);
		bool AreAdjacent(int start, int end);
		bool FindPath(int start, int end);
		void Print();
		
	private:
		vector<vector<int>> paths;
		unordered_map<int, string> cityNames;
		unordered_map<string, int> cityNumbers;
};

class Hierarchy
{
	public:
		Hierarchy() { root = nullptr; }
		~Hierarchy() {}
		
		void Insert(GeneralGuardian *guardian) { InsertGuardian(root, guardian); }
		bool Search(string name) { return SearchGuardian(root, name); }
		Guardian* Get(string name) { return GetGuardian(root, name); }
		void Print() { PrintHierarchy(root, 0); }
		void Delete() {}
		
	private:
		GeneralGuardian *root;
		
		void InsertGuardian(GeneralGuardian *current, GeneralGuardian *newGuardian);
		bool SearchGuardian(GeneralGuardian *current, string name);
		Guardian* GetGuardian(GeneralGuardian *current, string name);
		void PrintHierarchy(GeneralGuardian *current, int depth);
		void DeleteGuardian(GeneralGuardian *current, string name);
};

class Ranking
{
	public:
		Ranking() { root = nullptr; }
		~Ranking() {}
		
		void Insert(RankedGuardian *guardian) { root = InsertGuardian(root, guardian); }
		bool Search(int level) { return SearchGuardian(root, level); }
		void Print() { PrintRanking(root); }
		void Reorder() { ReorderRanking(root); }
		
	private:
		RankedGuardian *root;
		
		RankedGuardian* InsertGuardian(RankedGuardian *current, RankedGuardian *newGuardian);
		bool SearchGuardian(RankedGuardian *current, int level);
		void PrintRanking(RankedGuardian *current);
		RankedGuardian* ReorderRanking(RankedGuardian *current);
};

int ReadGuardiansFile(char *file_name, Ranking *ranking, Hierarchy *hierarchy);
int ReadCitiesFile(char *file_name, Map *map);
void StartEndPath(Map map, int *start, int *end);
void SelectGuardian(Guardian **selection, Ranking list, Hierarchy *guardians);
void WatchFight(Guardian *player, Guardian *rival, Ranking *ranking);

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		cout << "Usage: " << argv[0] << " <archivo_guardianes>.txt <archivo_ciudades>.txt" << endl;
		return 1;
	}
	
	int election, subElection, start, end, position = 0;
	Guardian *playableGuardian = nullptr, *rivalGuardian = nullptr;
	Ranking ranking;
	Hierarchy hierarchy;
	vector<vector<int>> list(20);
	Map map(list);
	
	int lecturaGuardianes = ReadGuardiansFile(argv[1], &ranking, &hierarchy);
	if (lecturaGuardianes != 0)
	{
		if (lecturaGuardianes == 1)
			cout << "Separe los datos con comas ',' sin espacios entremedio" << endl;
		else if (lecturaGuardianes == 2)
			cout << "Solo se Acepta UN Guardian con 100 Puntos de Poder" << endl;
		else
			cout << "Solo se Aceptan MAXIMO TRES Guardianes con 90-99 Puntos de Poder" << endl;
			
		return 2;
	}
	
	int lecturaCiudades = ReadCitiesFile(argv[2], &map);
	if (lecturaCiudades == 1)
	{
		cout << "Separe los datos con comas ',' sin espacios entremedio" << endl;
		return 3;
	}
	
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
				if (playableGuardian != nullptr)
				{
					cout << "INFO DE GUARDIAN:" << endl;
					cout << "Nombre: "<< playableGuardian->Name << endl << "Nivel: " << playableGuardian->Level << endl << "Ciudad: " << playableGuardian->City << endl << "Maestro: " << playableGuardian->Master;
					cout << endl;
				}
				else
					cout << "Elija Guardian Para ver su Informacion..." << endl;
				break;
				
			case 3: // conocer reino
				cout << "CONOCIENDO REINO" << endl;
				cout << "[1] Construir Camino\n[2] Consultar Camino\n";
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
				cout << "GUARDIAN DE JUGADOR: " << endl;
				SelectGuardian(&playableGuardian, ranking, &hierarchy);
				
				cout << "GUARDIAN RIVAL: " << endl;
				SelectGuardian(&rivalGuardian, ranking, &hierarchy);
				
				if (playableGuardian != nullptr && rivalGuardian != nullptr)
				{
					if (map.FindPath(map.getCityNumbers()[playableGuardian->City], map.getCityNumbers()[rivalGuardian->City]))
					{
						WatchFight(playableGuardian, rivalGuardian, &ranking);
					}
					else
					{
						cout << "Imposible Combatir a Guardian" << endl;
					}
				}
				else
				{
					cout << "No se Puede Efectuar Pelea" << endl;
					cout << rivalGuardian->Name << endl;
					
					cout << playableGuardian->Name << endl;
				}
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

bool Hierarchy::SearchGuardian(GeneralGuardian *current, string name)
{
	if (root == nullptr || current == nullptr)
		return false;
		
	if (name.compare(current->Info->Name) == 0)
		return true;
		
	for (GeneralGuardian *guardian : current->Apprentices)
	{
		if (SearchGuardian(guardian, name))
			return true;
	}
}

Guardian* Hierarchy::GetGuardian(GeneralGuardian *current, string name)
{
	if (root == nullptr || current == nullptr)
		return nullptr;
		
	if (name.compare(current->Info->Name) == 0)
		return current->Info;
		
	for (GeneralGuardian *guardian : current->Apprentices)
		if (GetGuardian(guardian, name) != nullptr)
			return GetGuardian(guardian, name);
			
	return nullptr;
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

RankedGuardian* Ranking::ReorderRanking(RankedGuardian *current)
{
	if (current == nullptr)
		return current;
	
	if (current->left != nullptr)
	{
		if (current->Info->Level >= current->left->Info->Level)
			current->left = ReorderRanking(current->left);
		else
		{
			RankedGuardian *changing = current->right, *original = current;
			while (temp->left != nullptr)
				temp = temp->left;
				
			current->Info = temp->Info;
			current = InsertGuardian(root, current);
		}
	}
	else
	{
	}
	if (current->right != nullptr)
	{
		if (current->Info->Level <= current->right->Info->Level)
			current->right = ReorderRanking(current->right);
		else
		{
			
		}
	}
	else
	{
	}
	
	return current;
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

bool Ranking::SearchGuardian(RankedGuardian *current, int level)
{
	if (root == nullptr || current == nullptr)
		return false;
		
	if (current->Info->Level == level)
		return true;
	
	if (current->Info->Level > level)
		return SearchGuardian(current->left, level);
	if (current->Info->Level < level)
		return SearchGuardian(current->right, level);
}

// imprimir ranking
void Ranking::PrintRanking(RankedGuardian *current)
{
	if (current == nullptr)
		return;
		
	PrintRanking(current->right);
	
	cout << current->Info->Name << " [" << current->Info->Level << "]";
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
int ReadGuardiansFile(char *file_name, Ranking *ranking, Hierarchy *hierarchy)
{
	int maxLevelGuardian = 0, candidateGuardians = 0;
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
			{
				if (chunk[0] == ' ')
					return 1;
					
				chunks.push_back(chunk);
			}
				
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
			
			if (newGuardian->Level == 100)
				maxLevelGuardian++;
			if (maxLevelGuardian > 1)
				return 2;
				
			if (newGuardian->Level >= 90 && newGuardian->Level < 100)
				candidateGuardians++;
			if (candidateGuardians > 3)
				return 3;
			
			ranking->Insert(newRanking);
			hierarchy->Insert(newHierarchy);
		}
	}
	
	guardians.close();
	return 0;
}

// lee archivo de ciudades
int ReadCitiesFile(char *file_name, Map *map)
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
			{
				if (chunk[0] == ' ')
					return 1;
				
				chunks.push_back(chunk);
			}
			
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
				map->setCityNumbers(city, map->getCityNumbers().size());
			}
			
			if (!connectionExists)
			{
				connectionPos = map->getCityNames().size();
				map->setCityNames(map->getCityNames().size(), connection);
				map->setCityNumbers(connection, map->getCityNumbers().size());
			}
			
			map->AddPath(cityPos, connectionPos);
		}
	}
	
	cities.close();
	return 0;
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

void SelectGuardian(Guardian **selection, Ranking list, Hierarchy *guardians)
{
	string name;
	cout << "Lista de Guardianes: " << endl;
	list.Print();
	
	cout << "Escriba nombre del Guardian Deseado: ";
	cin >> name;
	cout << endl;
	
	if (guardians->Search(name))
	{
		*selection = guardians->Get(name);
		cout << "Guardian Seleccionado Correctamente" << endl;
	}
	else
	{
		*selection = nullptr;
		cout << "No Existe Guardian con ese Nombre" << endl;
	}
}

void WatchFight(Guardian *player, Guardian *rival, Ranking *ranking)
{
	int winCondition = rand()%10;
	
	if (winCondition < 4)
	{
		cout << player->Name << " Ha Ganado la Pelea..." << endl;
		cout << player->Name << " Ha Conseguido 3 Puntos de Poder." << endl;
		player->Level += 3;
		cout << rival->Name << " Ha Perdidio 2 Puntos de Poder." << endl;
		rival->Level -= 2;
	}
	else
	{
		cout << rival->Name << " Ha Ganado la Pelea..." << endl;
		cout << rival->Name << " Ha Conseguido 3 Puntos de Poder." << endl;
		rival->Level += 3;
		cout << player->Name << " Ha Perdidio 2 Puntos de Poder." << endl;
		player->Level -= 2;
	}
	
	//ranking->Reorder();
}
