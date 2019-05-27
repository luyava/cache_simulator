// this file contains methods to implement victic cache simulator

//This method takes cache parameters and gives back index
//and tag values to be used in replacement policies.
int mask(
  int offset,
  int tag,  
  int index,  
  int dir,
	int ic){      

     int s, sic;
    s=stoi(dir,nullptr,16); //convierte la direccion de string a int
    sic=stoi(ic,nullptr,16);//string a int en ic
    s=s>>offset; //se obtiene el valor del index con sll y srl
    s=s<<(offset + tag);
    vindex=s>>(offset + tag);
    vtag= s >> (index + tag);
} 


//aca voy a hacer por mientras el struct vc. REVISAR
struct victim_cache{
  unsigned int vctag[4];
  uint8_t rp_value[4];
  bool dirty[4];
  bool valid[4];
}victim;

struct victim vc[16]

int load_counter_vc=0;
int store_counter_vc=0;
bool l1lmiss=false;
bool l1smiss=false;
int hit_vc=0;
int dual_miss=0;
int i_c;
int mini_temp;
//this method manages victim cache
int victim(
 // struct victim_cache,
  struct victim vc[16],
  //aca va el struct entru
  //int &load_counter_vc,
  //int &store_counter_vc,
  int vtag,
  //int &hit_vc,
  //int &l_dual_miss,
  //int &s_dual_miss,
  //int &i_c,
  //int &mini_temp,
  //int l1lmiss,
  //int l1smiss,
  //int &l_hit_vc,
  //int &s_hit_vc)
  {
  //case load (ls=0)
  //solo se activa en miss en l1 
  if(l1lmiss){
    load_counter_vc=load_counter_vc+1;
    for(int i=0;i==3;i++){
      if(victim.vctag[i]==vtag){
        l_hit_vc=l_hit_vc+1;
        //aca hace el swap
        mini_temp=entry[idx].tag;
        entry[idx].tag=victim.vctag[i];
        victim.vctag[i]=mini_temp;
        break;
      }
      else{
        l_dual_miss=l_dual_miss+1;
        lmiss=lmiss+1;
      }
    }
  }
  //case store (ls=1)
  if(l1smiss){
    store_counter_vc=store_counter_vc+1;
    for(int i=0;i==3;i++){
      if(victim.vctag[i]==vtag){
        s_hit_vc=s_hit_vc+1;
        //aca hace el swap
        mini_temp=entry[idx].tag;
        entry[idx].tag=victim.vctag[i];
        victim.vctag[i]=mini_temp;
        break;
      }
      else{
        s_dual_miss=s_dual_miss+1;
        smiss=smiss+1;
        //aca sustituye segun LRU
        mini_temp=entry[idx].tag;
        entry[idx].tag=vtag;
        //aca sust. segun politica de remplazo
        victim.vctag[i_rp]=mini_temp;
      }
    }
  }
}