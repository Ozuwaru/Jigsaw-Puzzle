#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

class Datos{
	public:
		struct data{
			char nombre[20];
			int puntaje=0;
		}d;
	    void registro();
	    void mostrar();
};

void Datos::registro(){
	cout<<"\nIngrese el nombre del jugador: ";
	cin.ignore();
	cin.getline(d.nombre,20);
}

void Datos::mostrar(){
	cout<<"Nombre: "<<d.nombre<<" Puntaje: "<<d.puntaje;
}

class Game{
	vector<Datos> jugadores;
	public:
		Game();
		~Game();
		void sign();
		bool login(char s[20]);
		void player_list();
		void delet_player();
};

//meter los elementos del archivo
Game::Game(){
	ifstream file("Jugadores.bin",ios::binary);
	if(file){
		Datos aux;
		while(file.read((char*)&aux.d,sizeof(Datos::data))){
			jugadores.push_back(aux);
		}
	}
	file.close();
}

//mandar las cosas pal archivo
Game::~Game(){
	ofstream file;
	file.open("Jugadores.bin",ios::binary|ios::trunc);
	for(int i=0;i<jugadores.size();i++){
		file.write((char*)&jugadores[i].d,sizeof(Datos::data));
	}
	file.close();
}

void Game::sign(){
	Datos aux;
	aux.registro();
	if(login(aux.d.nombre))
		cout<<"Ya se registro un jugador con este nombre.";
	else
		jugadores.push_back(aux);
}

//pa cuando quiera iniciar sesion con un usuario ya creado
bool Game::login(char s[20]){
	for(int i=0;i<jugadores.size();i++){
		if(strcmp(jugadores[i].d.nombre,s)==0){
			return true;
		}
	}
	return false;
}

void Game::player_list(){
	for(int i=0;i<jugadores.size();i++){
		jugadores[i].mostrar();
		cout<<endl;
	}
}

void Game::delet_player(){
	char s[20];
	cout<<"Ingrese el nombre del jugador: ";
	cin.ignore();
	cin.getline(s,20);
	int i;
	for(i=0;i<jugadores.size();i++){
		if(strcmp(jugadores[i].d.nombre,s)==0){
			jugadores.erase(jugadores.begin()+i);
			break;
		}
	}
}

int main(){
	Game j;
	int op;
	do{
		system("cls");
		cout<<"1.Registrar jugador.";
		cout<<"\n2.Iniciar sesion.";
		cout<<"\n3.Lista de jugadores.";
		cout<<"\n4.Borrar jugador.";
		cout<<"\n5.Salir.";
		cout<<"\nOp: ";
		cin>>op;
		switch(op){
			case 1:{
				j.sign();
				break;
			}
			case 2:{
				char s[20];
				cout<<"Ingrese el nombre del jugador: ";
				cin.ignore();
				cin.getline(s,20);
				if(j.login(s))
					cout<<"Entro con exito.";
				else
					cout<<"No esta registrado.";
				break;
			}
			case 3:{
				j.player_list();
				break;
			}
			case 4:{
				j.delet_player();
				break;
			}
		}
		system("pause");
	}while(op!=5);
	return 0;
}