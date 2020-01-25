#include <iostream>
#include <fstream>
#include <cstring>
#include <string.h>

using namespace std;

struct registro{

    char pos[4] = "";
    char llave[12] = "";
    char producto[13] = "";
    char fabricante[16] = "";
    char cantidad[6] = "";
    char precio[4] = "";
    char fecha[10] = "";
    char tipo[11] = "";
};


void EnumerarArchico(int value){//Fucion para enumerar el archivo lo enemuera dependiendo del parametro

    registro blaco;

    fstream Archivo("Dispersion.txt",ios::out);//Abrir el Archivo

    if(!Archivo.is_open()){//Verificacion de apertira
        cout << "UPS... Error al abrir el archivo" << endl;
    }

    for (int i =0 ;i <= value;i++) {//Ingresa Registros en blanco con un numero para marcar la posicion
        int nrr = i * (int(sizeof (struct registro)) + (int(sizeof (char)) * 2));
        itoa(i,blaco.pos,10);
        blaco.pos[3] = '|';
        Archivo.seekp(nrr);
        Archivo.write((char*) &blaco,sizeof (struct registro));
        Archivo << "\n";
    }

    Archivo.close();//Cerrar archivo
};

int GenerarPosicion(string primaria){//Funcion para generar una posicion dependiendo del parametro

    primaria = primaria + " ";
    int Numero[10];
    int aux;

    for (int i = 0;i <= primaria.size();i++) {//Llenar arreglo con los valores ascii de la cadena
        aux = (int) primaria[i];//conversion de caracter a su valor ascii
        Numero[i] = aux;//ingresar al arreglo
    }

    aux = 0;
    string conca = "";//auxuliar para suma
    int j = 0;

    while(j <= primaria.size()) {//Algoritmo de dispersion
        conca = "";//reinicio de variable
        conca = to_string(Numero[j]) + to_string(Numero[j+1]);//union de valores
        aux+= atoi(conca.c_str());//suma
        if(aux >= 20000){//verificacion para aplicar modulo
            aux = aux%20000;//modulo
            j = j+2;
        }
        else{
            j = j+2;
        }
    }
    return (aux%47);
}

void Dispersion(){

    EnumerarArchico(49);
    fstream Origen("Inventario_hashing.txt",ios::in | ios::out);//Archivo de datos
    fstream Destino("Dispersion.txt",ios::in | ios::out);//Abrir en el cual se va a guardar

    //Mover Apuntadores
    Origen.seekg(0);
    Destino.seekg(0);
    string Llave;
    string LoDemas;
    string pos;
    registro nuevoReg,regblanco,establecerBlaco;
    int num = 0;

    while (!Origen.eof()) {//Sacar Registros

        //lee el archivo y ingresa los datos a un registro
        getline(Origen,Llave,',');
        strcpy(nuevoReg.llave,Llave.c_str());
        nuevoReg.llave[11] = '|';

        getline(Origen,LoDemas,',');
        strcpy(nuevoReg.producto,LoDemas.c_str());
        nuevoReg.producto[12] = '|';

        getline(Origen,LoDemas,',');
        strcpy(nuevoReg.fabricante,LoDemas.c_str());
        nuevoReg.fabricante[15] = '|';

        getline(Origen,LoDemas,',');
        strcpy(nuevoReg.cantidad,LoDemas.c_str());
        nuevoReg.cantidad[5] = '|';

        getline(Origen,LoDemas,',');
        strcpy(nuevoReg.precio,LoDemas.c_str());
        nuevoReg.precio[4] = '|';

        getline(Origen,LoDemas,',');
        strcpy(nuevoReg.fecha,LoDemas.c_str());
        nuevoReg.fecha[9] = '|';

        getline(Origen,LoDemas,'\n');
        strcpy(nuevoReg.tipo,LoDemas.c_str());
        nuevoReg.tipo[10] = '|';

        if(Origen.eof()){
            break;
        }

        num = GenerarPosicion(nuevoReg.llave);

        int i = 0;
        bool ban = true;
        Destino.seekg(0);
        while (!Destino.eof() && ban == true) {//lee el archivo donde se guardaran los registros
            int nrr = i * (int(sizeof (struct registro)) + (int(sizeof (char)) * 2));
            Destino.seekg(nrr);
            Destino.read((char*) &regblanco,sizeof (struct registro));

            if(Destino.eof()){
                break;
            }

            //si el registro leido del archivo de salida esta en blanco ingresa el nuevo registro
            if(atoi(regblanco.pos) == num){
                if(strcmp(regblanco.llave,"") == 0){
                    strcpy(nuevoReg.pos,regblanco.pos);
                    nuevoReg.pos[3] = '|';
                    Destino.seekp(nrr);
                    Destino.write((char*) &nuevoReg,sizeof (struct registro));
                }else{//Genera un mensaje con las colsiones y el registro y posicion que la genero
                    cout << "La llave base: " << num << " generada ya esta ciendo Ocupada" << endl;
                    cout << "Para el registro: ";
                    cout << nuevoReg.llave << ",";
                    cout << nuevoReg.producto << ",";
                    cout << nuevoReg.fabricante << ",";
                    cout << nuevoReg.cantidad << ",";
                    cout << nuevoReg.precio << ",";
                    cout << nuevoReg.fecha << ",";
                    cout << nuevoReg.tipo << ",";

                    cout << endl << endl;
                }

                ban = false;
            }
            i++;
        }
        //limpia las estructuras
        nuevoReg = establecerBlaco;
        regblanco = establecerBlaco;

    }

    //Cerrar Archivos
    Origen.close();
    Destino.close();
}


int main()
{
    char op;
    do{
        cout << "Menu" << endl
             << "a) Ejecutar funcion de dispersion" << endl
             << "b) Salir" << endl;
        cin >> op;
        if(op == 'a'){
            system("cls");
            Dispersion();
        }
    }while (op != 'b');
}
