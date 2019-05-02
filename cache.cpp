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
	int temp,cache,linea,asoc1,rp;
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
	      asoc1 = temp;}
	    if (str5.compare(argv[i]) == 0) {//rp 0:LRU,1:NRU,2:RRIP,3:RANDOM
	      temp = atoi(argv[i+1]);
	      rp = temp;}
	  }
	  
	}
	printf("------------------------------------\n");
	printf("Parametros ingresados: \n");
	printf("cache = %i \n",cache);
	printf("linea= %i \n",linea);
	printf("asoc1= %i \n",asoc1);
	printf("rp= %i \n",rp);

	cout<<"----------------------------------------"<<endl;


	//Llamado rutina de impresion de parametros de las cache
	cache=cache*1024;//conversion de KB a B
	 int const asoc=asoc1;

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
	cout <<"Cache: "<<cache<<endl;
	cout <<"------------------------------------- " <<endl;

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/////////////lectura del trace y obtencion del index y tag

	
///declaracion de algunas variables
 string numeral,ls,dir,ic; //componentes de cada linea del trace
 int accesos,rnd,hit_flag,hit, prueba;
 unsigned int vindex, vtag,s,sic,vswap;
 unsigned long int totalIC=0;
 unsigned int c[cache][asoc];
 long int lmiss,smiss,sthit,lhit,load_counter,store_counter;
 
 struct entry{
  unsigned int stag[4];
  uint8_t old[4];
  bool dirty[4];
  bool valid[4]; 
}line;

struct entry cch[cache];
 ///////////////////////////////////////////////////////////

 ///////selector de politica de reemplazo
 
 if(rp==0){ //inicia politica random
   cout<<"Politica de reemplazo Random"<<endl;
 
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

 }//fin politica random
 /////////////////////////////////////////////////////////////////////////////////////
 else if(rp==1){ //politica LRU
   cout<<"Politica de reemplazo LRU"<<endl;
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
     hit_flag=0; ///bandera para metodo de hits
     /////////////////////inicio mascaras


     s=s>>offset; //se obtiene el valor del index con sll y srl
     s=s<<(offset + tag);
     vindex=s>>(offset + tag);
     vtag= s >> (index + tag);
 
     totalIC=totalIC+sic;  

     /////////////////////////////politica lru

     if(ls.compare("0")==0){     //load
       load_counter=load_counter+1;
       struct entry line=cch[vindex];
       for (int i = 0; i < asoc-1; i++){
	 if(line.stag[i]==vtag){
	   lhit = lhit+1;
	   break;}
       }
       /*si hay un miss en load se llamara el dato segun protocolo */
     }
 
     if(ls.compare("1")==0){     //store
       store_counter=store_counter+1;
       hit=0;
       hit_flag=0;
       struct entry line=cch[vindex];
       for (int i = 0; i < asoc-1; i++){
	 hit=hit+1;//es igual i para debug
	 if(line.stag[i]==vtag){//hit en store
	   sthit = sthit+1;
	   hit_flag=1;
	   line.old[i]=line.old[i]-2;//rejuvence el del hit
	   for(int k=0;k==(asoc-1);k++){//este hace que los que no son hit envejezcan
	     line.old[k]=line.old[k]+1;
	   }//fin for
	   if(line.old[i]<0){line.old[i]=0;}//asegura que no haya valores negativos
	   break;}
       }
       if(hit_flag==0){//inicio store en miss
	 int maximo=0;
	 int tracker=0;
	 for (int j=0; j==(asoc-1);j++){//detecta el mas viejo
	   if(line.old[j]>maximo){
	     maximo=line.old[j];
	     tracker=j;
	       }
	 }//fin for detector de viejos
	 line.stag[tracker]=vtag;
       }//fin store en miss
     }//fin store
 }//fin cin del trace
   
 }//fin politica LRU
 //////////////////////////////////////////////////////////////////////////////////////
 
 else if(rp==2){ //politica RRIP
   cout<<"Politica de reemplazo RRIP"<<endl;

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
     hit_flag=0; ///bandera para metodo de hits
     /////////////////////inicio mascaras


     s=s>>offset; //se obtiene el valor del index con sll y srl
     s=s<<(offset + tag);
     vindex=s>>(offset + tag);
     vtag= s >> (index + tag);
 
     totalIC=totalIC+sic;  

     /////////////////////////////politica rrip

     if(ls.compare("0")==0){     //load rrip
       load_counter=load_counter+1;
       struct entry line=cch[vindex];
       for (int i = 0; i < asoc-1; i++){
	 if(line.stag[i]==vtag){
	   lhit = lhit+1;
	   break;}
       }
       /*si hay un miss en load se llamara el dato segun protocolo */
     }
 
     if(ls.compare("1")==0){     //store
       store_counter=store_counter+1;
       hit=0;
       hit_flag=0;
       struct entry line=cch[vindex];
       for (int i = 0; i < asoc-1; i++){
	 hit=hit+1;//es igual i para debug
	 if(line.stag[i]==vtag){//hit en store
	   sthit = sthit+1;
	   hit_flag=1;
	   if(line.old[i]<asoc-2){
	     line.old[i]=0;}//si hay 2 hit o mas lo rejuvence a 0
	   else{line.old[i]=asoc-1;}//rejuvence el del hit
	   break;}
       }
       if(hit_flag==0){//inicio store en miss
	 int maximo=0;
	 int tracker=0;
	 for (int j=0; j==(asoc-1);j++){//detecta el mas viejo
	   if(line.old[j]>maximo){
	     maximo=line.old[j];
	     tracker=j;
	       }
	 }//fin for detector de viejos
	 line.stag[tracker]=vtag;//guarda el dato 
	 line.valid[tracker]=true;
	 int h=0;
	 for(int k=0;k==asoc-1;k++){//detector de valid
	   if(line.valid[k]==false){
	     line.old[k]=3;
	   }
	   else {
	     h=h+1;
	   }//fin else
	 }//fin for detector de valid
	 if(h==asoc-1){
	   for(int k=0;k==asoc-1;k++){//vejez
	     line.old[k]=line.old[k]+1;
	   }//fin for de vejez
	   line.old[tracker]=line.old[tracker]-1; //esto es para que ese bit sea coherente
	 }//fin if
       }//fin store en miss
     }//fin store
 }//fin cin del trace
 
 }//fin politica RRIP
 //////////////////////////////////////////////////////////////////////////////////////
 
 else{
   cout<<"Politica de reemplazo no disponible"<<endl;
   exit(0);
 }
 ///////////////////////////////////////////////////////////////////////////////////////


 ////inicio estadisticas
 
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
 //cout <<"Miss rate en load: "<<(lmiss/load_counter)<<endl;
 //cout <<"Miss rate en store: "<<(smiss/store_counter)<<endl;
 cout <<"Misses totales: "<<(lmiss+smiss)<<endl;
 cout <<"Hits totales: "<<(lhit+sthit)<<endl;
 cout <<":::::::::::::::::::::::::::::::::::::::::: "<<endl;
 
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////Medicion del tiempo de ejecucion
 cout <<"Tiempo de ejecución: " << ((clock()-cl)*1000/CLOCKS_PER_SEC)*0.001 << " s" << endl;
 cout<<"Maestro, he terminado"<<endl;
 exit(0);

}//fin del programa


