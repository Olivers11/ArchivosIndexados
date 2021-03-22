#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
#define CL system("cls")
#define PA system("pause")
using namespace std;




class Persona{
	private:
		int dpi;
		char nombre[10];
		int edad;
	public:
		Persona();
		void setDatos(int, char[10], int);
		void Imprimir();		
		int getDpi();
		int getEdad();
};

Persona::Persona()
{
	this->edad = 0;
	strcpy(this->nombre, " \0");
	this->dpi = 0;
}

void Persona::setDatos(int dpi, char n[10], int e)
{
	this->edad = e;
	strcpy(this->nombre, n);
	this->dpi = dpi;
}


void Persona::Imprimir()
{
	cout << endl;
	cout << "DPI: " << this->dpi << endl;
	cout << "Nombre: " << this->nombre << endl;
	cout << "Edad: " << this->edad << endl << endl;
}

int Persona::getEdad()
{
	return this->edad;
}

int Persona::getDpi()
{
	return this->dpi;
}



class INDICE{
	public:
		int clave;
		unsigned int ubicacion;
		INDICE()
		{
			this->clave = 0;
			this->ubicacion = 0;
		}
};





void InsertarPersona(Persona persona)
{
	ofstream escritura_p; escritura_p.open("personas.bin", ios::out | ios::app | ios::binary);
	ofstream escritura_i; escritura_i.open("indices.bin", ios::out | ios::app | ios::binary);
	if(escritura_p.fail() or escritura_i.fail()){cout << "ERROR AL ABRIR LOS ARCHIVOS \n";}
	else
	{
		
		INDICE ind = INDICE();
		ind.ubicacion = escritura_p.tellp(); //obtenemos la ubicacion del puntero del archivo
		ind.clave = persona.getDpi();
		
		escritura_p.write((char*)&persona, sizeof(Persona));
		escritura_i.write((char*)&ind, sizeof(INDICE));
		
		escritura_p.close();
		escritura_i.close();
		cout << "Guardado correctamente! \n";
	}	
}



void ConsultaPersona(Persona persona)
{
	cout << "PERSONAS \n\n";
	FILE *lectura;
	lectura = fopen("personas.bin", "r+b");
	if(!lectura){cout << "Error al abrir \n";}
	else
	{
		while(fread(&persona, sizeof(Persona), 1, lectura))
		{
			persona.Imprimir();
		}
	}
	fclose(lectura);
}

template<class T>
void LeerArchivo(T &arr_indices)
{
	int ubicacion = 0;
	Persona persona;
	fstream lectura; lectura.open("empleados.bin", ios::in | ios::binary);
	if(lectura.fail()){cout << "error al abrir \n";}
	else
	{
		lectura.read((char*)&persona, sizeof(persona));
		while(!lectura.eof())
		{
			INDICE ind = INDICE();
			ind.clave = persona.getDpi();
			ind.ubicacion = ubicacion;
			lectura.read((char*)&persona, sizeof(persona));
			arr_indices.push_back(ind);
			ubicacion = ubicacion +sizeof(persona);
		}
	}
	lectura.close();
}



bool myFunction(const INDICE a, const INDICE b){
	return(a.clave < b.clave);
}

// tuve un error poniendo el arr_indices como tipo InINDICEdice y recibiendolo como parametro xd

void OrdenarIndices(vector<INDICE> &arr_indices)
{
	vector<INDICE> aux;
	aux = arr_indices;
	sort(aux.begin(), aux.end(), myFunction);
}

template<class T> 
void GrabarIndices(T &arr_indices)
{
	fstream file;
	file.open("indices.bin", ios::in | ios::binary);
	if(file.fail()){cout << "ERROR \n";}
	else
	{
		for(auto i : arr_indices)
		{
			file.write((char*)&arr_indices, sizeof(arr_indices));
		}
	}
	file.close();	
}


void ReconstruirIndices(vector<INDICE> &arr_indices)
{
	LeerArchivo(arr_indices);
	OrdenarIndices(arr_indices);
	GrabarIndices(arr_indices);	
}



int main()
{
	int registros = 0;
	vector<INDICE> arr_indices;
	int opcion;
	do
	{
		CL;
		cout << "[1] Nuevo Registro \n";
		cout << "[2] Mostrar Registros \n";
		cout << "[3] Modificar Registro \n";
		cout << "[4] Eliminar Registro \n";
		cout << "[5] Salir \n";
		cout << "opcion: ";
		cin >> opcion;
		
		if (opcion == 1)
		{
			CL;
			string n;
			int d, e;
			cout << "Nombre: "; cin >> n;
			cout << "Edad: "; cin >> e;
			cout << "Dpi: "; cin >> d;
			Persona nueva = Persona();
			nueva.setDatos(d, strdup(n.c_str()), e);
			InsertarPersona(nueva);
			ReconstruirIndices(arr_indices);
			PA;
		}
		else if (opcion == 2)
		{
			CL;
			Persona persona;
			ConsultaPersona(persona);
			PA;
		}
		else if (opcion == 3)
		{
			CL;
			fstream personas, indices;
			personas.open("personas.bin", ios::in | ios::out | ios::binary);
			indices.open("indices.bin", ios::in | ios::binary);
			if(personas.fail() or indices.fail()){ cout << "ERROR \n";}
			else
			{
				INDICE ind;
				Persona persona;
				int clave;
				cout << "ingrese DPI a buscar: "; cin >> clave;
				indices.read((char*)&ind, sizeof(ind));
				while(!indices.eof())
				{
					if(clave == ind.clave)
					{
						personas.seekg(ind.ubicacion);
						personas.read((char*)&persona, sizeof(persona));
						string n;
						int e, c;
						cout << "nuevo nombre: "; cin >> n;
						cout << "nueva edad: "; cin >> e;
						cout << "nuevo dpi: "; cin >> c;
						persona.setDatos(c, strdup(n.c_str()), e);
						personas.seekg(ind.ubicacion);
						personas.write((char*)&persona, sizeof(persona));
						cout << "MODIFICADO CORRECTAMENTE!\n";
						personas.close();
						indices.close();
					}
					indices.read((char*)&ind, sizeof(ind));
				}
				cout << "No se encontro a la persona :c \n";
				indices.close();
				personas.close();
			}
			ReconstruirIndices(arr_indices);
			PA;
		}
		else if(opcion == 4)
		{
			CL;
			cout << "metodo por realizar \n";
			PA;	
		}

	} while (opcion != 5);
}
