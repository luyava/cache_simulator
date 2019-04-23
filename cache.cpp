//Luis Yannicelli A87086. Memoria Cache

//Para correr:
//g++ -Wall cache.cpp -o cache
// gunzip -c mcf.trace.gz |./cache -t 16 -l 32 -a 2 -rp 0

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <bitset>
#include <ctime>
#include "parametros.h"

using namespace std;

int main(int argc, char **argv){
	clock_t cl=clock();
	string str1 ("h"), str2("-t"), str3("-l"), str4("-a"), str5("-rp");
	int temp,cache,linea,asoc,rp;
	int offset,bloque, index, tag;

	for(int i=1; i<argc; i++){
		
	  if(argc==2) {
	    if (str1.compare(argv[1]) == 0) {
	      printf("Sintaxis: -t <tamaño de cache en kB> -l <tamaño de la línea en bytes> -a <asociatividad> \n");
	    }
	    exit(0);}
			
	  if(argc <9 && argc !=2) {
	    printf("Por favor, verificar sintaxis. Para ayuda digite 'h' \n");
	    exit(0);}
			
	  if(argc == 9){ 	
	    if (str2.compare(argv[i]) == 0) { // en i esta la bandera y en i + 1 el dato
	      temp = atoi(argv[i+1]);
	      cache = temp;}
	    if (str3.compare(argv[i]) == 0) {
	      temp = atoi(argv[i+1]);
	      linea = temp;}
	    if (str4.compare(argv[i]) == 0) {
	      temp = atoi(argv[i+1]);
	      asoc = temp;}
	    if (str5.compare(argv[i]) == 0) {//rp 0:LRU,1:NRU,2:RRIP,3:RANDOM
	      temp = atoi(argv[i+1]);
	      rp = temp;}
	  }
	  
	}
	printf("------------------------------------\n");
	printf("Parametros ingresados: \n");
	printf("cache = %i \n",cache);
	printf("linea= %i \n",linea);
	printf("asoc= %i \n",asoc);
	printf("rp= %i \n",rp);

	cout<<"----------------------------------------"<<endl;


	//Llamado rutina de impresion de parametros de las cache
	cache=cache*1024;//conversion de KB a B

	Parametros p;

	offset= p.m_offset(cache);
	bloque= p.m_bloque(cache,linea);
	index= p.m_index(cache,bloque,asoc);
	tag= p.m_tag(linea,offset,index);
	
	cout <<"Parametros obtenidos: "<<endl;
	cout <<"Offset: "<<offset <<endl;
	cout <<"Bloques: "<<bloque <<endl;
	cout <<"Asociatividad: "<<asoc<<endl;
	cout <<"Index: "<<index <<endl;
	cout <<"Tag: "<<tag <<endl;
	cout <<"Cache"<<cache<<endl;
	cout <<"------------------------------------- " <<endl;

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/////////////lectura del trace y obtencion del index y tag

	
///declaracion de algunas variables
 string numeral,ls,dir,ic; //componentes de cada linea del trace
 int accesos,rnd;
 unsigned int vindex, vtag,s,sic;
 unsigned long int totalIC=0;
 unsigned int c[cache][asoc];
 long int lmiss,smiss,sthit,lhit,load_counter,store_counter;
	
while(!cin.eof()){
accesos=accesos+1;	
cin>>numeral; //recibe dato del trace
cin>>ls; //load o store
cin>>dir; //direccion
cin>>ic; //instruction counter

if(numeral.compare("")==0){ //rompe el ciclo cuando se acaban las inst
	break;}
 
s=stoi(dir,nullptr,16); //convierte la direccion de string a int
sic=stoi(ic,nullptr,16);//string a int en ic

/////////////////////inicio mascaras


s=s>>offset; //se obtiene el valor del index con sll y srl
s=s<<(offset + tag);
vindex=s>>(offset + tag);
vtag= s >> (index + tag);
 
totalIC=totalIC+sic;  

/////////////////////////////politica random

 if(ls.compare("0")==0){     //load
   load_counter=load_counter+1;
   for (int i = 0; i < asoc-1; i++){
     if(c[vindex][i]==vtag){
       lhit = lhit+1;
       break;}
   }
 }
 
 if(ls.compare("1")==0){     //store
   store_counter=store_counter+1;
   for (int i = 0; i < asoc-1; i++){
     if(c[vindex][i]==vtag){
       sthit = sthit+1;
       break;}
   }
   rnd=rand() % asoc +1;
   c[vindex][rnd]=vtag; 
 }
}

 lmiss=load_counter-lhit;
 smiss=store_counter-sthit;
 cout <<":::::::::::::::::::::::::::::::::::::::::: "<<endl;
 cout <<"Estadisticas: " <<endl;
 cout << "IC total: "<< totalIC<< endl;
 cout << "Total de accesos: "<<accesos << endl;
 cout << "Total de misses en load: "<<lmiss << endl;
 cout << "Total de hits en load: "<<lhit << endl;
 cout << "Total de misses en store: "<<smiss << endl;
 cout << "Total de hits en store: "<<sthit << endl;
 cout <<":::::::::::::::::::::::::::::::::::::::::: "<<endl;
 
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////Medicion del tiempo de ejecucion
 cout <<"Tiempo de ejecución: " << ((clock()-cl)*1000/CLOCKS_PER_SEC)*0.001 << " s" << endl;
 cout<<"Maestro, he terminado"<<endl;
 exit(0);

}

//anotaciones de tiempos:
//recorre el trace haciendo mascaras en 5.528 s
/*Parametros ingresados: (Politica Random)
cache = 16 
linea= 32 
asoc= 2 
rp= 0 
----------------------------------------
Parametros obtenidos: 
Offset: 14
Bloques: 512
Asociatividad: 2
Index: 4
Tag: 14
Cache16384
------------------------------------- 
:::::::::::::::::::::::::::::::::::::::::: 
Estadisticas: 
IC total: 20603103
Total de accesos: 6943858
Total de misses en load: 4809191
Total de hits en load: 780282
Total de misses en store: 1110023
Total de hits en store: 244362
:::::::::::::::::::::::::::::::::::::::::: 
Tiempo de ejecución: 6.14 s
*/
