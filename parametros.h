//Luis Sergio Yannicelli Vargas, A87086

//Esta libreria trata los parametros de la cache para conseguir los tamanos del offset, index y tag. Ademas imprimir en pantalla los resultados obtenidos.

#include <iostream>
using namespace std;

class Parametros{
   private:
  int cache1;
  int offset1;
  unsigned int asoc1;
  int linea1;
  int bloque1;
  int index1;
  int tag1;

   public:
  int m_offset(int cache1){
    offset1=log2(cache1);//bits del cache OFFSET
    return offset1;
  }
  int m_bloque(int cache1, int linea1){
    bloque1=cache1/linea1;
    return bloque1;
  }
  void m_asoc(int asoc1){
    cout<<asoc1<<endl;
  }
  int m_index(int cache1, int bloque1, int asoc1){
    index1=(cache1/(bloque1*asoc1));
    return index1;
  }
  int m_tag(int linea1, int offset1, int index1){
    tag1=linea1-offset1-index1;
    return tag1;
  }
  void imprimirpar(){
    cout << "Cache en bytes en l1: " << cache1 << endl;
    cout << "Número de bloques en l1: " << bloque1 << endl;
    cout << "Asociatividad en l1: " << asoc1 << endl;
    cout << "Tamaño de línea en l1: " << linea1 << endl;
    cout << "------------------------------" <<endl;
    cout << "Offset en l1: " << offset1 << endl;
    cout << "Index en l1: " << index1 << endl;
    cout << "Tag en l1: " << tag1 << endl;
    cout << "---------------------------------------"<<endl;
  }

};

