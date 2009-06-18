/*
 This file is part of rubik_2^3
 Copyright (C) 2008 Alejandro Lorca <alelorca@yahoo.es>
 
 rubik_2^3 is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 rubik_2^3 is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with rubik_2^3.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "run_rubik.h"
using namespace std;

ind ngen;
bool debug;
const ind mod=24;
/*
************************************************************************
*                                                                      *
* get_family_id function                                               *
*                                                                      *
************************************************************************
*/
ind get_family_id(const ind_v& p, arrayfamcycle& map_famcycle){
  const ind_v cycle=perm_cycles_swap(p);
  int map_famcycle_sz=map_famcycle.size();
  int map_famcycle_id;
  if ( map_famcycle[cycle] == 0 && ( map_famcycle_sz == 0 || cycle.size() != 0 ) ){
    map_famcycle_id=map_famcycle_sz++;
    map_famcycle[cycle]=map_famcycle_id;
  }
  else{
    map_famcycle_id=map_famcycle[cycle];
  }
  return map_famcycle_id;
}
/*
************************************************************************
*                                                                      *
* itos function                                                        *
*                                                                      *
************************************************************************
*/
template <typename T> string itos(const T& i){
// This function converts any type into a string
  stringstream s;
  s << i;
  return s.str();
}
template string itos<unsigned int>(const unsigned int &);
template string itos<int>(const int &);
template string itos<gnumeric>(const gnumeric &);

string itos(const ind& i){
// This function converts an index (probably char) into a string
  char a = i+97;
  stringstream s;
  s << a;
  return s.str();
}
/*
************************************************************************
*                                                                      *
* vtos function                                                        *
*                                                                      *
************************************************************************
*/
string vtos(const ind_v& v){
// This function converts a vector into a string
  stringstream s;
  ind_v::const_iterator iter = v.begin();
  while ( iter != v.end() ){
    if ( iter != v.begin() ) {
      s << "," << itos(*(iter++));
    } else {
      s << itos(*(iter++));
    }
  }
  return s.str();
}
/*
************************************************************************
*                                                                      *
* ipow function                                                        *
*                                                                      *
************************************************************************
*/
int ipow(const ind& base, const ind_v::size_type& exp){
  int result = 1;
  for (ind_v::size_type i=0; i!= exp; i++){
    result *= base;
  }
  return result;
}
/*
************************************************************************
*                                                                      *
* operation_to_omap function                                           *
*                                                                      *
************************************************************************
*/
gnumeric operation_to_omap(const ind_v& v){
  // Built within lcn and ginac libraries
  gnumeric opermap=0;
  int coefficient=1;
  for ( ind_v::size_type i = 0; i != v.size(); i++ ){
    opermap += v[i]*coefficient;
    coefficient *= ngen;
  }
  return opermap;
}
/*
************************************************************************
*                                                                      *
* operation_to_name function                                           *
*                                                                      *
************************************************************************
*/
string operation_to_name(const ind_v& v){
  string name;
  if ( v.size() != 0 )
    for ( ind_v::const_reverse_iterator rit = v.rbegin();
	  rit != v.rend(); rit++){
      //    for ( ind_v::const_iterator rit = v.begin();
      //	  rit != v.end(); rit++){
      if ( *rit < 4 ){
	name+="+";
      } else if ( *rit < 7){
	name+="-";
      } else if ( *rit < 10){
	name+=":";
      }
      
      if ( *rit%3 == 1 ){
	name+="X";
      } else if ( *rit%3 == 2 ){
	name+="Y";
      } else if ( *rit%3 == 0 ){
	name+="Z";
      }
    } else {
    // The identity
    name+="E";
  }
  return name;
}
/*
************************************************************************
*                                                                      *
* compare_notation                                                     *
*                                                                      *
************************************************************************
*/
bool compare_notation(const Turn_Description& x, 
		      const Turn_Description& y){
  return x.notation < y.notation;
}
/*
************************************************************************
*                                                                      *
* sort_by_notation                                                     *
*                                                                      *
************************************************************************
*/
vector<Turn_Description> sort_by_notation(const arraytd& l){
  vector<Turn_Description> vl;
  for (arraytd::const_iterator iter = l.begin(); 
       iter != l.end(); iter++){
    vl.push_back(*iter);
  }
  sort(vl.begin(), vl.end(), compare_notation);
  return vl;
}
/*
************************************************************************
*                                                                      *
* cublet_position function                                             *
*                                                                      *
************************************************************************
*/
ind cublet_position(const ind& i){
  // Report the cublet position for a given index. positions 0 to 7
  // are the representatives

  if ( i < 8 ) { return i;
  } else if ( i == 8 || i == 16 ) { return 0; 
  } else if ( i == 14|| i == 19 ) { return 1; 
  } else if ( i == 13|| i == 23 ) { return 2; 
  } else if ( i == 9 || i == 22 ) { return 3; 
  } else if ( i == 12|| i == 20 ) { return 4;
  } else if ( i == 15|| i == 18 ) { return 5; 
  } else if ( i == 11|| i == 17 ) { return 6; 
  } else if ( i == 10|| i == 21 ) { return 7;
  }
}
/*
************************************************************************
*                                                                      *
* cublet_orientation function                                          *
*                                                                      *
************************************************************************
*/
ind facet_orientation(const ind& i){
  // Report the facet orientation for a given index. positions 0 to 2
  // are the representatives according to X,Y,Z directions
  return i/8;
}
/*
************************************************************************
*                                                                      *
* decode_perm function                                                 *
*                                                                      *
************************************************************************
*/
ind_v decode_perm(const ind_v& p, const ind_v& o){
  ind_v perm(mod), x,y,z;
  const ind x_[8] = {0,1,2,3,4,5,6,7};
  const ind y_[8] = {8,14,13,9,12,15,11,10};
  const ind z_[8] = {16,19,23,22,20,18,17,21};
  vector<ind_v > r;
  for (ind_v::size_type i = 0; i != 8; i++){
    x.push_back(x_[i]);
    y.push_back(y_[i]);
    z.push_back(z_[i]);
  }
  r.push_back(x);
  r.push_back(y);
  r.push_back(z);
  

  // Now some ordering
  for (ind_v::size_type i = 0; i != perm.size(); i++){
    // Identity by default
    perm[i] = i;
  }
  // The real thing
  for (ind_v::size_type i = 0; i != p.size(); i++){
    int oy = 1, oz = 2;
    // There is an issue here, regarding the orientation of the piece.
    // they were defined to have orientation 0 if the x facet is also
    // in a X face after the permutation. This exchanges y,z facets to
    // Z,Y faces when the permutation of the piece is odd
    // Positions 0,2,5,7 are even and 1,3,4,6 are odd w.r.t. 0
    if ( ( i == 0 || i == 2 || i == 5 || i == 7 ) && 
	 ( p[i] == 1 ||  p[i] == 3 ||  p[i] == 4 || p[i] == 6 ) ){
	oy=2; oz=1;
      } 
    else if ( ( i == 1 || i == 3 || i == 4 || i == 6 ) && 
	      ( p[i] == 0 ||  p[i] == 2 ||  p[i] == 5 || p[i] == 7 ) ){
	oy = 2; oz = 1;
    }      
    perm[x[i]] = (r[(0 +o[i])%3][p[i]])%mod;
    perm[y[i]] = (r[(oy+o[i])%3][p[i]])%mod;
    perm[z[i]] = (r[(oz+o[i])%3][p[i]])%mod;
  }
  return perm;
}
/*
************************************************************************
*                                                                      *
* count_if_lt function                                                 *
*                                                                      *
************************************************************************
*/
ind_v::size_type count_if_lt(const ind_v::const_iterator& b,
				   const ind_v::const_iterator& e,
				   const ind& value){
  ind_v::size_type count = 0;
  for (ind_v::const_iterator iter = b; iter != e; iter++){
    count += (*iter < value) ? 1 : 0;
  }
  return count;
}
/*
************************************************************************
*                                                                      *
* count_if_le function                                                 *
*                                                                      *
************************************************************************
*/
ind_v::size_type count_if_le(const ind_v::const_iterator& b,
				   const ind_v::const_iterator& e,
				   const ind& value){
  ind_v::size_type count = 0;
  for (ind_v::const_iterator iter = b; iter != e; iter++){
    count += (*iter <= value) ? 1 : 0;
  }
  return count;
}
/*
************************************************************************
*                                                                      *
* count_if_gt function                                                 *
*                                                                      *
************************************************************************
*/
ind_v::size_type count_if_gt(const ind_v::const_iterator& b,
				   const ind_v::const_iterator& e,
				   const ind& value){
  ind_v::size_type count = 0;
  for (ind_v::const_iterator iter = b; iter != e; iter++){
    count += (*iter > value) ? 1 : 0;
  }
  return count;
}
/*
************************************************************************
*                                                                      *
* perm_to_map function                                                 *
*                                                                      *
************************************************************************
*/
int perm_to_map(const ind_v& p){
  ind_v position(8), orientation(8);
  const int c[8] = {1,24,21,18,15,12,9,6};
  ind_v::size_type k[8] = {1,24,504,9072,136080,1632960,
			       14696640,88179840};
  vector<int> vcoefficient(8);
  int result = 0;
  ind_v cublets_order;
  for (ind_v::size_type i = 0; i != 8; i++){
    vcoefficient[i] = k[i];
    // The position is the cublet_position minus the amount of 
    // previous positions < cublet_position. Shift the value consistently
    const ind cp = cublet_position(p[i]);
    cublets_order.push_back(cp);
    //    cout << cublets_order.size() 
    // << "cublets ocupied:" << vtos(cublets_order) << endl;
    position[i] = cp-count_if_lt(cublets_order.begin(),
			       cublets_order.end(), cp);
    orientation[i] = facet_orientation(p[i]);
    result+=vcoefficient[i]*(position[i]*3+orientation[i]);
    //    cout << "result=" << result << endl;
  }
  return result;
}
/*
************************************************************************
*                                                                      *
* map_to_perm function                                                 *
*                                                                      *
************************************************************************
*/
ind_v map_to_perm(int result){
  ind_v position(8), orientation(8);
  //  const ind_v::size_type c[8]={1,24,21,18,15,12,9,6};
  const ind_v::size_type k[8] = {1,24,504,9072,136080,
				     1632960,14696640,88179840};
  vector<int> vcoefficient(8);
  ind_v cublets_order;
  for (ind_v::size_type i = 0; i != 8; i++){
    vcoefficient[i] = k[i];
    cublets_order.push_back(i);
  }
  
  for (int i = 7; i != -1; i--){
    int div = result/vcoefficient[i];
    position[i] = div/3;
    orientation[i] = div%3;
    result-=vcoefficient[i]*(position[i]*3+orientation[i]);
  }

  // Now it is time to rearrange the cublets order
  for (ind_v::size_type i = 0; i != 8; i++){
    const ind relative_pos = position[i];
    position[i] = cublets_order[relative_pos];
    cublets_order.erase(cublets_order.begin()+relative_pos);
  }
  return decode_perm(position,orientation);
}
/*
************************************************************************
*                                                                      *
* perm_to_fam function                                                 *
*                                                                      *
************************************************************************
*/
string perm_to_fam(const ind_v& p){
  string fam;
  const ind_v n = perm_cycles_swap(p);
  short int howmany, value = 0;
  ind_v::const_iterator iter = n.begin();
  while ( iter != n.end() ){
    value = *iter;
    howmany = count(n.begin(),n.end(),value);
    //    cout << value << " appears " << howmany << " times" <<endl;
    fam+= itos<int>(value);
    if (howmany > 1) fam += "^"+itos<int>(howmany);
    for (ind i = 0; i != howmany; i++) iter++;
    if (iter != n.end()){
      fam += "*";
    } else {
      return fam;
    }
  }
}
/*
************************************************************************
*                                                                      *
* perm_to_notation function                                            *
*                                                                      *
************************************************************************
*/
string perm_to_notation(ind_v p){
  string notation = "";
  const ind_v r = perm_inverse(p);
  for (ind_v::size_type i = 0; i != p.size(); i++){
    if ( i != p[i] ){
      notation+="(";
      ind j = i;
      while (j != p[j]){
	// Not needed with a-x notation+=itos(j) + ",";
	notation+=itos(j);
	swap(p[j],p[r[j]]);
	j = r[j];
      }
      notation+=itos(j)+")";
    }
  }
  return notation;
}
/*
************************************************************************
*                                                                      *
* write_description function                                           *
*                                                                      *
************************************************************************
*/
int write_description(const arrayte& lte, const string& filename,
		const bool& writetofile){
  ofstream file;
  if (writetofile) file.open(filename.c_str(),ofstream::app);  
  for (arrayte::const_iterator iter = lte.begin();
       iter != lte.end(); iter++){
    Turn_Description d;
    // Some code building ltd
    
    d.entry = (*iter);
    d.name = operation_to_name(iter->operation);
    d.family_id = perm_to_fam(iter->permutation);
    d.notation = perm_to_notation(iter->permutation);

    string infoline;
    try { 
      infoline = format_entry(d);
    } catch ( length_error format_error ) {
      cout << "[Error <write_description>: " <<format_error.what() 
	   << "]"<< endl;
      exit(1);
    }
    if (writetofile){
      file << infoline << endl;
    } else {
      cout << infoline << endl;
    }
  }
  if (writetofile) file.close();
  return 0;  
}
int write_description(const arraytc& ltc, const string& filename,
		const bool& writetofile){
  ofstream file;
  if (writetofile) file.open(filename.c_str(),ofstream::app);  
  for (arraytc::const_iterator iter = ltc.begin();
       iter != ltc.end(); iter++){
    Turn_Description d;
    // Some code building ltd
    
    d.entry = (iter->member)[0];
    d.name = operation_to_name((iter->member)[0].operation);
    d.family_id = perm_to_fam((iter->member)[0].permutation);
    d.notation = perm_to_notation((iter->member)[0].permutation);

    string infoline;
    try { 
      infoline = format_entry(d);
    } catch ( length_error format_error ) {
      cout << "[Error <write_description>: " <<format_error.what() 
	   << "]"<< endl;
      exit(1);
    }
    if (writetofile){
      file << infoline << endl;
    } else {
      cout << infoline << endl;
    }
  }
  if (writetofile) file.close();
  return 0;  
}
/*
************************************************************************
*                                                                      *
* format_entry function                                                *
*                                                                      *
************************************************************************
*/
string format_entry(const Turn_Description& td){
  string line;
  // We compose a formatted line for output with fields

  const string column_pad = "\t";
  //const string omap = itos((td.entry).omap); 
  // const string map = itos((td.entry).permutation_id);
  //string::size_type omap_pad_sz = 12, map_pad_sz = 11;

  // Some test before printing integers right aligned
  //if ( omap_pad_sz < omap.size() ){
  //  throw length_error("padding size for omap negative");
  //} 
  /*
  else if ( map_pad_sz < map.size() ){
    throw length_error("padding size for map negative");
  }
  */
  //const string omap_pad(omap_pad_sz-omap.size(),' ');
  //  const string map_pad(map_pad_sz-map.size(),' ');
  const string name = td.name, name_pad = td.name.size() < 8 ? "\t": "";
  const string fam = td.family_id, fam_pad = td.family_id.size() < 8 ? "\t": "";
  const string notation = td.notation;

  // Building the line
  //line = omap_pad + omap;
  //line += column_pad;
  line += name + name_pad;
  line += column_pad;
  line += fam + fam_pad;
  line += column_pad;
  //  line += map_pad + map;
  //  line += column_pad;
  line += notation; // + notation_pad;
  return line;
}
/*
************************************************************************
*                                                                      *
* do_name_product                                                      *
*                                                                      *
************************************************************************
*/
string do_name_product(const string& a, const string& b){
  return a+b;
}
/*
************************************************************************
*                                                                      *
* perm_product function                                                *
*                                                                      *
************************************************************************
*/
ind_v perm_product(const ind_v& x, const ind_v& y){
  ind_v z;
  const ind_v::size_type x_sz = x.size(), y_sz = y.size();
  if ( x_sz == y_sz ){
    z.reserve(y_sz);
    // I like more the second iteration
    //  for (ind_v::size_type i = 0; i != z.size(); i++){
    for (ind_v::const_iterator i = y.begin(); i != y.end(); i++){
      z.push_back(x[*i]);
    }
  } else {
    throw domain_error("[perm_product: Vectors with different sizes!]");
  }
  return z;
}
Turn_Entry perm_product(const Turn_Entry& x, const Turn_Entry& y, arrayfamcycle& map_famcycle){
  Turn_Entry z;
  ind_v z_operation = x.operation;
  z_operation.insert(z_operation.end(),
		     y.operation.begin(), y.operation.end());
  z.omap = operation_to_omap(z_operation);
  z.operation = z_operation;
  z.level = x.level+y.level;
  z.permutation = perm_product(x.permutation, y.permutation);
  z.family_id = get_family_id(z.permutation, map_famcycle);
  z.permutation_id = perm_to_map(z.permutation);
  return z;
}
/*
************************************************************************
*                                                                      *
* perm_inverse function                                                *
*                                                                      *
************************************************************************
*/
ind_v perm_inverse(const ind_v& x){
  ind_v y(x.size());
  for (ind_v::size_type i = 0; i != x.size(); i++){
    y[x[i]] = i;
  }
  return y;
}
/*
************************************************************************
*                                                                      *
* perm_cycles_swap function                                            *
*                                                                      *
************************************************************************
*/
ind_v perm_cycles_swap(ind_v x){
  ind_v cycles;
  for (ind_v::size_type i = 0; i != x.size(); i++){
    if ( i != x[i] ){
      ind c = 1;
      for (ind_v::size_type j = i; j != x.size(); j = x.size()){
	while ( x[j] != i ){
	  swap(x[x[j]],x[i]);
	  c++;
	}
      }
      cycles.push_back(c);
    }
  }
  sort(cycles.begin(),cycles.end());
  return cycles;
}
/*
************************************************************************
*                                                                      *
* fill_class function                                                  *
*                                                                      *
************************************************************************
*/
int fill_class(Turn_Class& c, arrayfamcycle& map_famcycle){
  Turn_Entry x=(c.member).front();
  // We make the Similarity transformations to x in order to obtain the
  // other two members
  ind_v T(mod);

  for (ind_v::size_type i = 0; i != mod; i++){
    T[i]=(mod/3+i)%mod;
  }
  for (vector<Turn_Entry>::size_type ix = 0; ix != 2; ix++){
    x.permutation=perm_product(T,perm_product(x.permutation,perm_inverse(T)));
    x.family_id=get_family_id(x.permutation, map_famcycle);
    x.permutation_id=perm_to_map(x.permutation);
    for (ind_v::size_type i = 0; i != (x.operation).size(); i++){
      if ( x.operation[i]%3 == 0 ){
	(x.operation)[i] -= 2;
      } else {
	(x.operation)[i] += 1;
      }
    }
    x.omap=operation_to_omap(x.operation);
    (c.member).push_back(x);
  }
  return 0;
}
/*
************************************************************************
*                                                                      *
* fill_identity function                                               *
*                                                                      *
************************************************************************
*/
Turn_Entry fill_identity (arrayfamcycle& map_famcycle){
  Turn_Entry e;
  int e_omap;
  ind e_level = 0;
  ind_v e_operation, e_perm(mod);
  // Setting the identity as a permutation vector
  for ( ind_v::size_type i = 0; i != e_perm.size(); i++){
    e_perm[i] = i;
  }
  
  // Assigns
  e.omap = operation_to_omap(e_operation);
  e.level = e_level;
  e.operation = e_operation;
  e.permutation = e_perm;
  e.family_id = get_family_id(e_perm, map_famcycle);
  e.permutation_id = perm_to_map(e_perm);
  
  return e;
}
/*
************************************************************************
*                                                                      *
* get_identity function                                                *
*                                                                      *
************************************************************************
*/
int get_identity (arrayte& le, arrayfe& map_fam, arrayfamcycle& map_famcycle){
  const Turn_Entry e = fill_identity(map_famcycle);
  if ( debug ) cout << "e inserted, e_perm=" << vtos(e.permutation) << endl;
  bool newturn = insert_if_new(e, map_fam);
  if ( newturn ) le.push_back(e);
  if ( debug ) cout << "size of le=" << le.size() << endl;
  return 0;
}
int get_identity (arraytc& lc, arrayfe& map_fam, arrayfamcycle& map_famcycle){
  Turn_Class eclass;
  const Turn_Entry e = fill_identity(map_famcycle);
  if ( debug ) cout << "e inserted, e_perm=" << vtos(e.permutation) << endl;
  (eclass.member).push_back(e);
  bool newturn = insert_if_new(e, map_fam);
  if ( newturn ){
    (eclass.member).push_back(e);
    // Fill the vector makes no sense for the identity
    lc.push_back(eclass);
  }
  if ( debug ) cout << "size of lc=" << lc.size() << endl;
  return 0;
}
/*
************************************************************************
*                                                                      *
* fill_generators function                                             *
*                                                                      *
************************************************************************
*/
Turn_Entry fill_generator(const ind& id, arrayfamcycle& map_famcycle){
  Turn_Entry g;
  int g_omap;
  ind g_level = 1;
  ind_v g_operation, g_perm;
  vector<ind_v::size_type > subvalue(3,0);

  // Common declarations
  g_perm.clear();
  g_operation.clear();
  g_operation.push_back(id);
  
  // Individual declarations
  // For the +X rotation
  
  for (ind_v::size_type i = 0; i != 3; i++){
    subvalue[i] = (8*((id-1)%3+i))%mod;
    //  cout << "Subvalue[" << i << "] = " << subvalue[i] << endl;
  }
  
  // Setting the generators as permutation vectors
  for (ind_v::size_type i = 0; i != mod; i++){
    g_perm.push_back(i);
  }
  g_perm[(subvalue[0]+0)%mod] = (subvalue[0]+3)%mod;
  g_perm[(subvalue[0]+1)%mod] = (subvalue[0]+0)%mod;
  g_perm[(subvalue[0]+2)%mod] = (subvalue[0]+1)%mod;
  g_perm[(subvalue[0]+3)%mod] = (subvalue[0]+2)%mod;
  
  g_perm[(subvalue[1]+ 0)%mod] = (subvalue[1]+14)%mod;
  g_perm[(subvalue[1]+11)%mod] = (subvalue[1]+0 )%mod;
  g_perm[(subvalue[1]+ 5)%mod] = (subvalue[1]+11)%mod;
  g_perm[(subvalue[1]+14)%mod] = (subvalue[1]+ 5)%mod;
  
  g_perm[(subvalue[2]+ 0)%mod] = (subvalue[2]+17)%mod;
  g_perm[(subvalue[2]+22)%mod] = (subvalue[2]+ 0)%mod;
  g_perm[(subvalue[2]+ 7)%mod] = (subvalue[2]+22)%mod;
  g_perm[(subvalue[2]+17)%mod] = (subvalue[2]+ 7)%mod;
  
  if ( id > 3 && id < 7 ){
    g_perm = perm_inverse(g_perm);
  } 
  else if ( id > 6 && id < 10) {
    g_perm = perm_product(g_perm,g_perm);
  }
  
  // Final assigns
  g.omap = operation_to_omap(g_operation);
  g.level = g_level;
  g.operation = g_operation;
  g.permutation = g_perm;
  g.family_id = get_family_id(g_perm, map_famcycle);
  g.permutation_id = perm_to_map(g_perm);

  return g;
}
/*
************************************************************************
*                                                                      *
* get_generators function                                              *
*                                                                      *
************************************************************************
*/
int get_generators (const int& rot_style, arrayte& le, arrayfe& map_fam, 
		    arrayfamcycle& map_famcycle){
  // We loop as far as the rot_style defines
  for (int id = 1; id != 3*rot_style+1; id++){
    const Turn_Entry g=fill_generator(id, map_famcycle);
    
    bool newturn = true;
    newturn = insert_if_new(g, map_fam);
    if ( newturn ) le.push_back(g);
  }
  return 0;
}
int get_generators (const int& rot_style, arraytc& lc, arrayfe& map_fam, 
		    arrayfamcycle& map_famcycle){

  for (int id = 1; id != 3*rot_style+1; id += 3){
    Turn_Class gclass;
    const Turn_Entry g = fill_generator(id, map_famcycle);

    bool newturn = true;
    newturn = insert_if_new(g, map_fam);

    // In case of using only equivalence class, we will consider
    // only [+X] as the set {+X,+Y,+Z} and so one
    if ( newturn ){
      (gclass.member).push_back(g);
      // Fill the vector with other member of the same class
      fill_class(gclass,  map_famcycle);

      // Insertion
      insert_if_new((gclass.member)[1], map_fam);
      insert_if_new((gclass.member)[2], map_fam);

      // Finally we add it to the list of this level
      lc.push_back(gclass);
    }
  }
  return 0;
}
/*
************************************************************************
*                                                                      *
* insert_if_new                                                        *
*                                                                      *
************************************************************************
*/
bool insert_if_new(const Turn_Entry& turn, arrayfe& map_fam){
  bool newturn = true;
  arrayint turn_aint;
  // Insert if it is the head of a new family
  const arrayfe::const_iterator iterfam = map_fam.find(turn.family_id);
  if ( iterfam == map_fam.end() ){
    // New family found!
    turn_aint.insert(turn.permutation_id);
    map_fam[turn.family_id] = turn_aint; // Here we could use iterfam->second
    if ( debug ) cout << "New family (" << turn.family_id 
		      << ") inserted" << endl;
  } else {
    // Look in its family for antecesor with the same map
    const arrayint::const_iterator 
      itermap = map_fam[turn.family_id].find(turn.permutation_id);
    if ( itermap == map_fam[turn.family_id].end() ){
      map_fam[turn.family_id].insert(turn.permutation_id);
      if (debug) cout << "New member (" << turn.permutation_id 
		      << ") in an old family (" << turn.family_id 
		      << ") inserted" << endl;

    } else {
      newturn = false;
    }
  }
  return newturn;
}
/*
************************************************************************
*                                                                      *
* get_moves function                                                   *
*                                                                      *
************************************************************************
*/
int get_moves(const arrayte& generators, arrayte& levelm1, 
	      arrayte& thislevel, arrayfe& map_fam, 
	      arrayfamcycle& map_famcycle){
  // We proceed to build the product of generators times last level i-1
  for ( arrayte::iterator iter1=levelm1.begin();
	iter1 != levelm1.end(); iter1=levelm1.erase(iter1) ){
     if ( debug ) cout << "[get_moves: Beginning iter1 for rotation "
		       << itos<gnumeric>(iter1->omap) << "]" << endl;
    for ( arrayte::const_iterator iter2 = generators.begin();
	  iter2 != generators.end(); ++iter2 ){
      const Turn_Entry t = perm_product(*iter1,*iter2, map_famcycle);
      
      // Insert the permutation into our map of families if it is new
      const bool newturn = insert_if_new(t, map_fam);
      // Next flag may be used for debugging purposes, it keep also
      // those generated rotations which are equal to a previous one
      const bool keepall=0;
      if ( newturn || keepall ) thislevel.push_back(t);
      
      if ( debug ){
	// Check if the turn_map generated by perm_product is correct;
	if ( map_to_perm(t.permutation_id) != t.permutation ){
	  cout << "[Error: <get_moves>: Perm->Map->Perm wrong!]" 
	       << endl;
	  exit(1);
	}
      }
    }
    // This element is no longer needed (done in the 3rd for argument)
  }
  return 0;
}
int get_moves(const arraytc& generators, arraytc& levelm1, 
	      arraytc& thislevel, arrayfe& map_fam, 
	      arrayfamcycle& map_famcycle){

  /*
    The idea is to use the array of the previous level and generators'
    classes in order to build a new level of classes. For this we need
    a filled set of generators (all members en each class).
    The resulting class is either new (all members have unique map) or
    it is equal to an existing one stored in the map_fam pair.
    What is adventageous here is the fact of having to compare only the
    representative of the class for uniqueness. This doesn't save us the
    time of storing the map and omap of the other members of the class,
    but the time of refusing insertions of members of already refused
    representatives.
    Good luck!
  */
  for ( arraytc::iterator iter1=levelm1.begin();
	iter1 != levelm1.end(); iter1=levelm1.erase(iter1) ){
    if ( debug ) cout << "[get_moves: Beginning iter1 for rotation "
		      << itos<gnumeric>(((iter1->member)[0]).omap) << "]" << endl;
    for ( arraytc::const_iterator iter2 = generators.begin();
	  iter2 != generators.end(); ++iter2 ){
      vector<Turn_Entry>::const_iterator iter3,
	iter3_begin=(iter2->member).begin(),
	iter3_end=(iter2->member).end();
      for ( iter3=iter3_begin; iter3 != iter3_end; iter3++ ){
	Turn_Class tclass;
	const Turn_Entry t = perm_product((iter1->member)[0],*iter3, map_famcycle);
	const bool newturn = insert_if_new(t, map_fam);
	if ( newturn ){
	  (tclass.member).push_back(t);
	  // Fill the vector with other members of the same class
	  fill_class(tclass, map_famcycle);
	  // Insertion
	  insert_if_new((tclass.member).back(), map_fam);
	  // Delete them, they were use-and-throw-away objects
	  (tclass.member).pop_back();
	  insert_if_new((tclass.member).back(), map_fam);
	  (tclass.member).pop_back();
	  // Finally we add it to the list of this level
	  thislevel.push_back(tclass);
	}
      }
    }
    // This element is no longer needed (done in the 3rd for argument)
  }
  return 0;
}
/*
************************************************************************
*                                                                      *
* generate_rotations function                                          *
*                                                                      *
************************************************************************
*/
int generate_rotations(const int& rot_stylecode, 
		       const int& rot_level, const bool& equivalence, 
		       const string& outputfile){
  /* 
     We are in favour of using Turn_* structs instead of Rotation_*
     list<Rotation_Entry> rot_table;
  */
  arrayfe map_fam;
  arrayfamcycle map_famcycle;
  arrayte ltegenerators, ltelevelm1;
  arraytc ltcgenerators, ltclevelm1;
  ofstream file;
  string style, level;
  const string new_what = equivalence ? " classes" : "s";
  int found = 0;
  switch (rot_stylecode){
  case 1:
    style = "s";
    break;
  case 2:
    style = "q";
    break;
  case 3:
    style = "h";
    break;
  }
  level = rot_level < 0 ? "all" : itos<int>(rot_level);

  int ilevel = 0;
  // Invariant: we have computed ilevel number of levels so far  
  while ( ilevel != rot_level+1 ){
    // This contains the present level information
    arrayte ltelevel;
    arraytc ltclevel;
    
    if ( ilevel == 0 ){
      // Firstly we define our initial configuration (the identity)
      if ( equivalence ){
	get_identity(ltclevel, map_fam, map_famcycle);
      } else {
	get_identity(ltelevel, map_fam, map_famcycle);
      }
    } else if ( ilevel == 1 ){
      // Place the generators into the lists and keep a copy
      if ( equivalence ){
	get_generators(rot_stylecode, ltclevel, map_fam, map_famcycle);
	ltcgenerators = ltclevel;
      } else {
	get_generators(rot_stylecode, ltelevel, map_fam, map_famcycle);
	ltegenerators = ltelevel;
      }
    } else {
      // Fill with moves corresponding to the given level of rotation
      if ( debug ) cout << "[generate_rotations: " 
			<< "beginning get_moves at level " 
			<< ilevel << "]" << endl;
      if ( equivalence ){
	get_moves(ltcgenerators, ltclevelm1, ltclevel, map_fam, map_famcycle);
      } else {
	get_moves(ltegenerators, ltelevelm1, ltelevel, map_fam, map_famcycle);
      }
    }
    if ( outputfile == "/dev/null" ){
      // Wow! you don't want any output, do you? I'll save you some time
    }
    else {
      // Compose the full description for humans :) and print or write
      if ( debug ) cout << "[generate_rotations: " 
			<< "beginning compose_description]" << endl;
      if ( outputfile == "" ){
	// All to screen
	if ( debug ) cout << "[generate_rotations: " 
			  << "beginning write_level]" << endl;
	if ( equivalence ){
	  write_description(ltclevel,outputfile,0);
	} else {
	  write_description(ltelevel,outputfile,0);
	}
      } else {
	if ( ilevel == 0 ) file.open(outputfile.c_str(),ofstream::out);
	// Compose the full description for humans :) and print or write
	if ( debug ) {
	  cout << "[generate_rotations: beginning write_level]" << endl;
	}
	if ( equivalence ){
	  write_description(ltclevel,outputfile,1);
	} else {
	  write_description(ltelevel,outputfile,1);
	}
      }
    }

    // Preparing the ltelevelm1 for next level
    if ( equivalence ){
      ltclevelm1.clear();
    } else {
      ltelevelm1.clear();
    }
    // A bit of info for the user
    const int new_found =  equivalence ? ltclevel.size() 
      : ltelevel.size();
    found += new_found;
    string max_level = ( rot_level < 0 ) ? "all" : itos<int>(rot_level);
    cout << "Level ";
    cout.width(2);
    cout << itos<int>(ilevel);
    cout.width();
    cout << " out of " << max_level <<" finished. ";
    cout.width(7);
    cout << itos<int>(new_found);
    cout.width();
    cout << " new turn" << new_what  << " found!" << endl;

    // Incrementing the level or finishing
    int level_size = equivalence ? ltclevel.size(): ltelevel.size();
    if ( level_size != 0 ){
      // Prepare the lt{e,c}levelm1 for next round
      if ( debug ) cout << "[generate_rotations: clearing ltlevel]" 
			<< endl;

      // Reserving doesn't increase performance
      //   ltelevelm1.reserve(ltelevel.size());

      // Let's rename the ltelevel in ltelevelm1 and destroy ltelevel
      if ( equivalence ){
	ltclevelm1.swap(ltclevel);
      } else {
	ltelevelm1.swap(ltelevel);
      }

      // List are too slow;
      //      ltelevelm1.splice(ltelevelm1.begin(),
      //		ltelevel, ltelevel.begin(), ltelevel.end());

      ilevel++;
    } else {
      // All done!!!
      cout << "All done!" << endl;
      break;
    }
  }
 cout << "-----------------------------------------------------------"
	 << endl;
 cout << "A total of " << itos<int>(found) << " turn" << new_what 
       << " were found within " << map_fam.size() 
       << " families" << endl;
  int unique_turns=0;
  for (arrayfe::const_iterator iter = map_fam.begin();
       iter != map_fam.end(); iter++){
    unique_turns += (iter->second).size();
    cout.width(7);
    cout << (iter->second).size();
    cout.width();
    cout << " turn(s) corresponding to "
	 << perm_to_fam(map_to_perm(*(iter->second).begin())) << endl;
  }
  cout << "--------------------------------------------------------"
       << endl;
  cout.width(7);
  cout << unique_turns;
  cout.width();
  cout << " turn(s) corresponding to the families altogether" 
       << endl;
  return 0;
}
/*
************************************************************************
*                                                                      *
* run_rubik function                                                   *
*                                                                      *
************************************************************************
*/
int run_rubik(const string& rot_style, int& rot_level,
	      const bool& equivalence, const string& outputfile,
	      const bool& debug_flag){
  ind rot_stylecode;
  debug=debug_flag;
  // This is our main function to begin the generate_rotations
  cout << "Goal: to build at most ";
  if ( rot_level < 0 ){
    rot_level = -2; // -1 is reserved, it does real nothing.
      cout << "all";
  } else {
    cout << rot_level;
  }
  cout << " levels of ";
  if ( rot_style == "-s" ){
    cout << "single-direction quarter turns";
    rot_stylecode = 1;
  } else if ( rot_style == "-q" ){
    cout << "quarter turns";
    rot_stylecode = 2;
  } else if ( rot_style == "-f" ){
    cout << "full turns";
    rot_stylecode = 3;
  }
  cout << endl;
  // Setting the God's variable
  ngen=rot_stylecode*3;
  return generate_rotations(rot_stylecode, rot_level, 
			    equivalence, outputfile);
}
