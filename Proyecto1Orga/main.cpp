#include <iostream>
#include <fstream>
#include <cstring>
#include <String.h>
#include <cstdlib>
using namespace std;

int entradas;
int nb2=0;
int sizeA=0;
int bloqueE;
int mapaV;
char direccion[80],nombreAR[20],ndireccion[80];
char nombre2[20];
char nomE[20];
bool exi=false ;

void setTam2(int t){ //setear el numero de bloque
        nb2=t;
}
int getTam2(){ //numeros de bloques del disco
        return nb2;
}

struct MetaData{
     char nombre[20];
     int sizeDisco;
     int n_entradas;
     int size_entradas;
};
class MapaBit{
public:
        unsigned short int *arrMB;

        //constructores
        MapaBit(){
        }

        MapaBit(int tam){
                arrMB=new unsigned short int[tam];
    }
};
class FAT{
public :
        char nombre[30];
        unsigned char tipo; //Directorio o Archivo
        float size;
        int her_der;
        int izq;
        int padre;
        int libre; //Si esta libre =0, Si esta ocupado =1
        int inicio;

        FAT(){
                strcpy(nombre,"");
                her_der=-1;
                izq=-1;
                padre=-1;
                libre=0;
                inicio=-1;
        }
};
struct Data{
        char datos[1020];
        int next;
};




MetaData mdi;
MapaBit map;
FAT *fati;
//Data Da;
Data data;
class FileSystem{
        public:
                //MetaData md;
                int libre;
                int dim;
                int index;
                FAT *fL;
                FAT *fL2;
                FAT *fLD;
                FAT *fLLis;
                FAT *fLB;
                FAT *fa;
                int papa;
                int actual;
                int adi;

                //Constructor
                FileSystem(){
                }
                void init(int valor){
                        dim=valor;
                        fL=new FAT[dim];
                        fL2=new FAT[dim];
                        fLD=new FAT[dim];
                        fLLis=new FAT[dim];
                        fLB=new FAT[dim];
                        fa=new FAT[dim];


                        papa=0;
                        actual=0;
                        libre=0;
                        adi=0;
                }
                //====================================================================
                void listarFAT(){

                        ifstream flu("Disco.dat",ios::in | ios::binary);
                        if(!flu){
                                return ;
                        }
                        flu.seekg(sizeof(MetaData)+sizeof(MapaBit),ios::beg);

                        for(int x=0;x<dim;x++)
                                flu.read(reinterpret_cast<char *>(&fL[x]), sizeof(FAT));

                        cout <<dim<< '\n';
                        for(int x=0;x<dim;x++){
                                cout << " " << fL[x].nombre << " " << fL[x].tipo << " " << fL[x].size << " " << fL[x].izq<< " " <<fL[x].her_der<< " " <<fL[x].padre<< " " << fL[x].libre << '\n';
                        }
                        flu.close();
                }
                //====================================================================
                bool hayEspacio(){
                        ifstream flu22("Disco.dat",ios::in | ios::binary);
                        flu22.seekg(sizeof(MetaData)+sizeof(MapaBit),ios::beg);

                        int hay=0;
                        for(int x=0;x<dim;x++){
                                flu22.read(reinterpret_cast<char *>(&fL2[x]), sizeof(FAT));
                                if(fL2[x].libre!=-1)
                                        hay++;
                        }
                        if(hay<=0)
                                return false;
                        else
                                return true;
                }
                //====================================================================
                int getLibre(){
                        return libre;
                }
                //====================================================================
                void crearDir(){
                        ifstream flu("Disco.dat",ios::in | ios::binary);
                        fstream fluW("Disco.dat",ios::in | ios::out | ios::binary);
                        flu.seekg(sizeof(MetaData)+sizeof(MapaBit),ios::beg);

                        for(int x=0;x<dim;x++)
                                flu.read(reinterpret_cast<char *>(&fL2[x]), sizeof(FAT));

                        char  nombre[30];
                        int hijop;

                        if(hayEspacio()){
                                int pos=libre;

                                cout << "Posicion libre "<< pos << '\n' ;
                                cout << "Padre actual "<< papa <<  '\n' ;
                                cout << "Ingrese el nombre del Directorio   ";

                                cin >> nombre;

                                libre=fL2[pos].libre;
                                strcpy(fL2[pos].nombre,nombre);
                                fL2[pos].padre=papa;
                                fL2[pos].tipo='d';

                                if(fL2[papa].izq==-1){
                                        fL2[papa].izq=pos;
                                }else{
                                        hijop=fL2[papa].izq;
                                        while(fL2[hijop].her_der!=-1){
                                                hijop=fL2[hijop].her_der;
                                        }
                                        fL2[hijop].her_der=pos;
                                }

                                //fL2[pos].her_der=-1;
                                //fL2[pos].izq=-1;
                                fL2[pos].libre=-1;

                                fL2[pos].size=0.0;


                                fluW.seekp(sizeof(MetaData)+sizeof(MapaBit),ios::beg);
                                for(int x=0;x<dim;x++)
                                        fluW.write(reinterpret_cast<char *>(&fL2[x]), sizeof(FAT));

                        }else
                                cout << "No Hay Espacio en el Disco Para Crear";

                        adi++;
                }
                //====================================================================
                void CambirDir(){
                        char nombre[30];
                        int goku;

                        ifstream fluD("Disco.dat",ios::in | ios::binary);
                        fluD.seekg(sizeof(MetaData)+sizeof(MapaBit),ios::beg);

                        for(int x=0;x<dim;x++)
                                fluD.read(reinterpret_cast<char *>(&fLD[x]), sizeof(FAT));

                        cout << "A que Directorio Deseea Cambiar  ";
                        cin >> nombre;
                        bool esta=true;

                        for(int pos=0;pos<dim;pos++){
                                 goku=strcmp(nombre,fLD[pos].nombre);

                                if(goku==0){
                                        papa=pos;
                                        actual=pos;
                                        esta=true;
                                        break;
                                }else
                                        esta=false;
                        }
                        if(!esta){
                                cout << "Nombre de Carperta Erroneo \n";
                                return;
                        }

                        cout << "Usted Esta en La carpeta " << fLD[papa].nombre << "  \n";
                        cout << "Padre actual Cambiar "<< papa <<  '\n' ;
                        cout << "Actual Cambiar "<<actual <<  '\n' ;

                }
                int fatFree(){
                        ifstream flu("Disco.dat",ios::in | ios::binary);
                        flu.seekg(sizeof(MetaData)+sizeof(MapaBit),ios::beg);

                        for(int x=0;x<dim;x++){
                                flu.read(reinterpret_cast<char *>(&fL[x]), sizeof(FAT));
                                if(fL[x].libre!=-1){
                                        return x;
                                        break;
                                }
                        }
                        return -1;
                }
                //====================================================================
                void listarContenido(){
                        cout << "Actual a listar en listar contenido " << actual << '\n';
                        ifstream fluLis("Disco.dat",ios::in | ios::binary);
                        fluLis.seekg(sizeof(MetaData)+sizeof(MapaBit),ios::beg);

                        for(int x=0;x<dim;x++)
                                fluLis.read(reinterpret_cast<char *>(&fLLis[x]), sizeof(FAT));

                        int h=fLLis[actual].izq;
                                while(h!=-1){
                                        cout << "Nombre " << fLLis[h].nombre <<" Tipo " << fLLis[h].tipo << " Size " << fLLis[h].size  << '\n';
                                        h=fLLis[h].her_der;
                                }

                }
                int getPadre(){
                        return papa;
                }
};
class Disco{
        public:
                //el disco tiene internamente
                MetaData md;
                MapaBit mb;
                MapaBit mbL;
                FAT *fat;
                FileSystem f;
                int num_bloques;
                int size_entra;
                string nombre;

                Disco (){
                }

                void crear(string nombre,int nb/*num bloques*/,int tb/*tam bloques*/){
                        num_bloques=nb;//numero de bloques
                        setTam2(num_bloques);
                        size_entra=tb;//tamaño de las entradas 1024
                        nombre=nombre;

                        //meta data================
                        md.n_entradas=num_bloques;
                        strcpy(md.nombre,nombre.c_str());
                        md.size_entradas=tb;
                        md.sizeDisco=(tb*num_bloques);//tamaño del Disco

                        //Mapa Bit=================
                        MapaBit mb(num_bloques);//normal
                        MapaBit mbL(num_bloques);//lectura

                        for(int x=0;x<num_bloques;x++)//init
                                mb.arrMB[x]=0;


                        //FAT======================
                        fat=new FAT[num_bloques];

                        ofstream flujoF("Disco.dat",ios::out | ios::binary);
                        flujoF.seekp(sizeof(MetaData)+sizeof(MapaBit),ios::beg);

                        for(int x=0;x<num_bloques;x++){
                                strcpy(fat[x].nombre,"vacio");
                                fat[x].tipo='n';
                                fat[x].size=0.0;
                                fat[x].izq=-1;
                                fat[x].her_der=-1;
                                fat[x].padre=-1;
                                fat[x].inicio=-1;

                                if(x==num_bloques-1)
                                        fat[x].libre=-1;
                                else
                                        fat[x].libre=x+1;

                                flujoF.write(reinterpret_cast<char *>(&fat[x]), sizeof(FAT));
                        }

                        //DATA======================
                        //Data data;

                        for(int x=0;x<num_bloques/*1020*/;x++){
                                data.next=-1;
                                flujoF.seekp(sizeof(MetaData)+sizeof(MapaBit)+(num_bloques*(sizeof(FAT)))+(x*sizeof(Data)),ios::beg);
                                flujoF.write(reinterpret_cast<char *>(&data), sizeof(Data));
                                //cout << x << " ";
                        }

                        //FileSystem================
                        f.init(num_bloques);

                        escribirTodo(md,mb/*,data*/);
                }
                void escribirTodo(MetaData me,MapaBit mbit/*,Data dd*/){
                         fstream flujo(md.nombre,ios::out|ios::in | ios::binary);
                         flujo.seekp(0,ios::beg);
                         flujo.write(reinterpret_cast<char *>(&me), sizeof(MetaData));

                         flujo.seekp(sizeof(MetaData),ios::beg);
                         flujo.write(reinterpret_cast<char *>(&mbit), sizeof(MapaBit));
                         flujo.close();

                }

                void imprimir(){
                        ifstream map(md.nombre,ios::in | ios::binary);
                        map.seekg(0,ios::beg);
                        map.read(reinterpret_cast<char *>(&md), sizeof(MetaData));
                        cout << md.n_entradas << md.nombre << md.size_entradas << md.sizeDisco << '\n';
                        map.close();

                        ifstream map2(md.nombre,ios::in | ios::binary);
                        map2.seekg(sizeof(MetaData),ios::beg);
                        map2.read(reinterpret_cast<char *>(&mbL), sizeof(MapaBit));

                        for(int x=0;x<30;x++){
                                if(mbL.arrMB[x]==1)
                                        cout << '1';
                                else
                                        cout << '0';
                        }//===========
                }
                int bloqueVacio(){

                        ifstream set(md.nombre,ios::in | ios::binary);
                        set.seekg(sizeof(MetaData),ios::beg);
                        set.read(reinterpret_cast<char *>(&mbL), sizeof(MapaBit));

                        for(int x=0;x<num_bloques;x++){
                                if(mbL.arrMB[x]==0){
                                        return x;
                                        break;
                                }
                        }
                        return -1;

                }
                void setbitOn(int pos){
                        ifstream set(md.nombre,ios::in | ios::binary);
                        set.seekg(sizeof(MetaData),ios::beg);
                        set.read(reinterpret_cast<char *>(&mbL), sizeof(MapaBit));
                        mbL.arrMB[pos]=1;
                }

                void setbitoff(int pos){
                        ifstream set(md.nombre,ios::in | ios::binary);
                        set.seekg(sizeof(MetaData),ios::beg);
                        set.read(reinterpret_cast<char *>(&mbL), sizeof(MapaBit));
                        mbL.arrMB[pos]=0;
                }

                int numEntra(){
                        return num_bloques;
                }

};




Disco disco;

//=====================importar====================
int getLibreMapa(){
        return disco.bloqueVacio();
}
int SizeArchivo(char dirA[80]){
        fstream archi( dirA, ios::in | ios::binary );
        if (!archi){
                cout<<"No se puede abrir el archivo\n";
        }
        archi .seekg(0, ios::end);
        int size= archi.tellg();
        archi.close();
        return size;
}

int HayVAcio(){
        int blo=0;
        fstream archi( "Disco.dat", ios::in | ios::binary );

        archi.seekp(0,ios::beg);
        archi.read(reinterpret_cast<char *>(&mdi), sizeof(MetaData));

        archi.seekp(sizeof(MetaData),ios::beg);
        archi.read(reinterpret_cast<char *>(&map), sizeof(MetaData));


        for(int x=0; x<disco.numEntra();x++){
                if(map.arrMB[x] == 0){
                        blo++;
                }
        }
        return blo*1020;
}
void GetNombre(char nombre[80]){
        int valor=0 ;
    for(int x=0;x<80;x++){
        if(nombre[x]=='\\'){
            valor=x+1;
        }
        if(nombre[x]=='\0')
            break;
    }

    int x=0;

        do{
        nombre2[x]=nombre[valor];
        x++;
        valor++;
    }while(nombre[valor-1]!='\0');
}
void readF(){
        fati=new FAT[getTam2()];
        ifstream flu("Disco.dat",ios::in | ios::binary);
        flu.seekg(sizeof(MetaData)+sizeof(MapaBit),ios::beg);

        for(int x=0;x<getTam2();x++)
        flu.read(reinterpret_cast<char *>(&fati[x]), sizeof(FAT));
}
void verSig(){
        ifstream Ds2( "Disco.dat" , ios::in  | ios::binary);


        for(int x=0;x<30;x++){
                Ds2.seekg(sizeof(MetaData)+sizeof(MapaBit)+(mdi.n_entradas*sizeof(FAT))+x*(sizeof(Data)),ios::beg);
                Ds2.read(reinterpret_cast<char *>(&data), sizeof(Data));
                cout << data.next << "  ";
        }
        Ds2.close();

}
void Importar(char dir[80]){
        fstream Ds( "Disco.dat" , ios::in | ios::out | ios::binary);
        fstream Ai( dir , ios::in | ios::binary);
        Ds.seekp(0,ios::beg);
        Ds.read(reinterpret_cast<char *>(&mdi), sizeof(MetaData));

        if(!Ds)
                cout << "No se puede abrir el Disco ImportarMP3\n";

        if(!Ai)
                cout << "No se puede abrir Archivo en modo Lectura\n";

        int x =0;
        while(Ai && !Ai.eof()){//mientras exista i no sea el final
                Ai.seekg(x*1020, ios::beg);
                Ai.read(data.datos,1020);

                mapaV=disco.bloqueVacio();
                cout << "Bloque vacio "<<mapaV<<'\n';
                disco.setbitOn(mapaV);

                if(mapaV!=-1){//hay espacio en Disco
                        data.next=disco.bloqueVacio();
                        Ds.seekp(sizeof(MetaData)+sizeof(MapaBit)+(mdi.n_entradas*(sizeof(FAT)))+(mapaV*sizeof(Data)),ios::beg);
                        Ds.write(reinterpret_cast<char *>(&data), sizeof(Data));
                        x++;
                }else{//no hay espacio
                        cout << "Esta LLEno \n";
                }

        }//fin del While!!!:::::::::::::

        x--;
        data.next= -1;

        Ds.seekp(sizeof(MetaData)+sizeof(MapaBit)+(disco.numEntra()*(sizeof(FAT)))+(mapaV*sizeof(Data)),ios::beg);
        Ds.write(reinterpret_cast<char *>(&data), sizeof(Data));

        Ai.close();
        Ds.close();
}


//=================Exportar!!!
bool esta(char nom[20]){
        fstream Ds2( "Disco.dat" , ios::in | ios::out | ios::binary);
        Ds2.seekp(0,ios::beg);
        Ds2.read(reinterpret_cast<char *>(&mdi), sizeof(MetaData));

        readF();

        for(int x=0; x<mdi.n_entradas; x++){
                if(strcmp(fati[x].nombre,nom)==0){
                        bloqueE =fati[x].inicio;
                        return true;
                        break;
                }
        }
        return false;
}
void Exportar(int inicio){
        fstream Dise( "Disco.dat" , ios::in | ios::binary);
        fstream Ae(ndireccion, ios::out | ios::binary);

        Dise.seekp(0,ios::beg);
        Dise.read(reinterpret_cast<char *>(&mdi), sizeof(MetaData));

        int mapaV;
        if(!Dise)
                cout << "No se puede abrir el Disco\n";

        if(!Ae)
                cout << "No se puede abrir Archivo en modo Lectura\n";


        while ( inicio != -1 ){
                Dise.seekg(sizeof(MetaData)+sizeof(MapaBit)+(mdi.n_entradas*(sizeof(FAT)))+(inicio*sizeof(Data)),ios::beg);
                Dise.read( reinterpret_cast<  char * >( &data), sizeof(Data));
                inicio=data.next;
                if(inicio != -1) {

                        Ae.write(data.datos, 1020);
                }else{
                        for(int x=0; x<1020; x++)
                                Ae.write( reinterpret_cast<  char * >( &data.datos[x]), sizeof(char));

                        //=========
                           

                }
        }
        Ae.close();
        Dise.close();
}
void f5(){
        fstream flujo("Disco.dat", ios::out |ios::in | ios::binary);

        flujo.seekp(0,ios::beg);
        flujo.write(reinterpret_cast<char *>(&mdi), sizeof(MetaData));

        flujo.seekp(sizeof(MetaData),ios::beg);
        flujo.write(reinterpret_cast<char *>(&map), sizeof(MapaBit));

        flujo.seekp(sizeof(MetaData)+sizeof(MapaBit),ios::beg);
        for(int x=0;x<disco.numEntra();x++)
                flujo.write(reinterpret_cast<char *>(&fati[x]), sizeof(FAT));

        //read
         for(int x=0;x<disco.numEntra();x++){
                flujo.seekg(sizeof(MetaData)+sizeof(MapaBit)+((disco.numEntra())*sizeof(FAT))+(x*sizeof(Data)),ios::beg);

                //for(int y=0;y<1020;y++)
                //	flujo.read(reinterpret_cast<char *>(&data.datos[y]), sizeof(Data));

                 flujo.read(reinterpret_cast<char *>(&data), sizeof(Data));
                  flujo.write(reinterpret_cast<char *>(&data), sizeof(Data));
        }
        // //write
        // for(int x=0;x<disco.numEntra();x++){
        //	flujo.seekp(sizeof(MetaData)+sizeof(MapaBit)+((disco.numEntra())*sizeof(FAT))+(x*sizeof(Data)),ios::beg);
        //
        //	//for(int y=0;y<1020;y++)
        //	//	flujo.write(reinterpret_cast<char *>(&data.datos[y]), sizeof(Data));

        //	 flujo.write(reinterpret_cast<char *>(&data), sizeof(Data));
        //}

}
void existe(){
        ifstream dis("Disco.dat",ios::in |ios::binary);
        if(dis){
                exi=true;
                cout << "Ya existe el Disco \n";
        }else{
                cout << "Ingrese el numero de entradas de Data\n";
                cin >> entradas;
                ofstream dis("Disco.dat",ios::out | ios::binary);
                dis.close();
        }
}

int main(){
        int op;

        do{
                cout <<"Menu Disco Simulado\n\n";
                cout <<"1.-Definir disco\n";
                cout <<"2.-Nuevo Directorio\n";
                cout <<"3.-Cambiar el Directorio\n";
                cout <<"4.-Lista de contenidos\n";
                cout <<"5.-Import MP3\n";
                cout <<"6.-Export MP3\n";
                cout <<"7.-Salir\n";
                cin >>op;

                switch (op){
                        case 1://========================
                                existe();
                                if(!exi){
                                        disco.crear("Disco.dat",entradas,1024);
                                        //disco.imprimir();
                                }else
                                        cout << "Ya existe un Disco Definido\n";
                        break;

                        case 2://========================
                                disco.f.crearDir();
                        break;

                        case 3://========================
                                disco.f.CambirDir();
                        break;
                        case 4://========================
                                cout << "Listar Contenido \n\n";
                                disco.f.listarContenido();
                        break;
                        case 5:
                                if(getLibreMapa()!=-1){
                                        //cout<< "hay espacio ";
                                        for(int x=0;x<disco.numEntra();x++){
                                                readF();
                                                if(fati[x].libre!=-1){
                                                        cout<<"Direccion del Archivo:  ";
                                                        cin>>direccion;

                                                        fstream ar(direccion, ios::in | ios::binary );
                                                        if (!ar){
                                                                cout<<"No se puede abrir el archivo.\n   ";
                                                                break;
                                                        }
                                                        ar.close();
                                                        GetNombre(direccion);
                                                        if(SizeArchivo(direccion)<HayVAcio()){
                                                                int val=getLibreMapa();
                                                                fati[x].inicio=val;
                                                                Importar(direccion);
                                                                strcpy(fati[x].nombre,nombre2);

                                                                fati[x].tipo='a';
                                                                fati[x].size= SizeArchivo(direccion);
                                                                int p=disco.f.getPadre();
                                                                fati[x].padre=p;
                                                                fati[p].izq=-1;


                                                                if((p!= -1) && (fati[p].izq == -1)){
                                                                        fati[p].izq = x;
                                                                }
                                                                fati[x].libre= -1;
                                                                cout << "ante de F5 \n\n";
                                                                verSig();
                                                                f5();
                                                                cout << "Despues de F5 \n\n";
                                                                verSig();
                                                                cout << "YA \n";
                                                                break;
                                                                system("pause");


                                                        }{
                                                                cout<<"No hay suficiente espacio en el DISCO";
                                                                break;
                                                        }

                                                }

                                        }
                                }else{
                                        cout << "Disco Lleno ";
                                }
                                cout << "\n\n\n\n";
                                cout <<"Siguientes despues de Importar ";
                                verSig();
                        break;
                        case 6:

                                cout<<"Nombre del Archivo";
                                cin>>nomE;
                                bool esta2 = esta(nomE);


                                cout << "Primer Bloque de la Data " << bloqueE <<'\n';
                                if(esta2){
                                        cout<<"Ingrese carpeta destino: ";
                                        cin>>ndireccion;
                                        Exportar(bloqueE);
                                }else{
                                        cout<<"No existe este Archivo ";
                                }


                        break;
                }
        }while(op!=7);
        return 0;
}
