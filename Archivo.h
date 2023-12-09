#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>
using namespace std;

class Datos {
public:
	struct data {
		char nombre[20];
		int puntaje = 0;
	}d;
	void registro(string n, int puntaje);
};

void Datos::registro(string n, int puntaje) {
	strcpy_s(d.nombre,20,n.c_str());
	d.puntaje = puntaje;
}


class Archivo {
public:
	vector<Datos> jugadores;
	Archivo();
	void guardar() {
		//mandar las cosas pal archivo
		ofstream file;
		file.open("Jugadores.bin", ios::binary | ios::trunc);
		for (int i = 0; i < jugadores.size(); i++) {
			file.write((char*)&jugadores[i].d, sizeof(Datos::data));
		}
		file.close();
	

	}
	int addPlayer(string n, int p) {
		Datos aux;
		aux.registro(n, p);
		int posicion = 0;
		if (jugadores.size() == 0) {
			jugadores.push_back(aux);
		}
		else if(jugadores.size() > 0){

			bool primera = false;
			for (int i = 0; i < jugadores.size(); i++) {
				if (aux.d.puntaje > jugadores[i].d.puntaje) {
					Datos a = jugadores[i];
					jugadores[i] = aux;
					aux = a;
					if (!primera) {
						primera = true;
						posicion = i;
					}
				}
			}
			jugadores.push_back(aux);
		}
		return posicion;
	}

};

Archivo::Archivo() {
	ifstream file("Jugadores.bin", ios::binary);
	if (file) {
		Datos aux;
		while (file.read((char*)&aux.d, sizeof(Datos::data))) {
			jugadores.push_back(aux);
		}
	}
	file.close();
}


