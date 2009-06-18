/* 
************************************************************************
*                                                                      *
*   "rubik_2^3"                                                        *
*   a c++ program to solve the Pocket 2x2x2 Rubik's cube               *
*                                                                      *
************************************************************************
*/

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

#include <iostream> // Controls input output and end of line
#include <string> // string: its member functions
#include <cctype> // To include the isdigit and other functions
// #include <vector>  // To form vectors
// #include <list>  // To form lists
// #include <stdexcept> // To have exceptions (i.e. out of range)
// #include <algorithm> // To have max and min functions
#include "run_rubik.h"
using namespace std;

/*
************************************************************************
*                                                                      *
* int main function                                                    *
*                                                                      *
************************************************************************
*/
int main(int argc, char *argv[]){
  vector<string> linetoparse;
  for ( vector<string>::size_type i=0; i!=argc; i++  ){
    linetoparse.push_back(argv[i]);
  }
  // If the word help is there, we offer help.
  const string help="--help", h="-h";
  vector<string>::const_iterator iter_begin;
  iter_begin=find(linetoparse.begin(),linetoparse.end(),help);
  if ( iter_begin == linetoparse.end() ){
    iter_begin=find(linetoparse.begin(),linetoparse.end(),h);
  }
  for (vector<string>::const_iterator iter=iter_begin; 
       iter != linetoparse.end(); iter++ ){
    // Help wanted
    cout << "usage: " << linetoparse[0]
	 << " [-s NUM || -q NUM || -f NUM]" 
	 << " [-o,--outputfile FILE || -c,--count-only]" 
	 << " [-d,--debug] [-h,--help] " << endl;
    cout << endl;
    cout << "Options calculation:" << endl;
    cout << "  -s NUM\t where NUM is the # of single-direction quarter turns" << endl;
    cout << "  -q NUM\t where NUM is the # of quarter turns" << endl;
    cout << "  -f NUM\t where NUM is the # of full turns" << endl;
    // Not yet implemented, it will change the default behaviour
    //    cout << "  -e \t\t use equivalence classes under C_3 group" << endl;
    cout << endl;
    cout << "Options I/O:" << endl;
    cout << "  -o,--outputfile FILE\t where FILE contains the turns\' description"
	 << endl;
    cout << "  -c,--count-only\t only info about new turns is displayed" << endl;
    cout << "  -d,--debug\t\t print debugging/progress information" << endl;
    cout << "  -h,--help\t\t show this help" << endl;
    return 0;
  }
  
  // Next, we try to catch the style of rotations, -s, -q or -h 
  // and its value
  const string s="-s", q="-q", f="-f";
  // We define the default
  string rot_style="-q";
  int rot_level=-2;
  // Search
  iter_begin=find(linetoparse.begin(),linetoparse.end(),s);
  if ( iter_begin == linetoparse.end() ){
    iter_begin=find(linetoparse.begin(),linetoparse.end(),q);
  }
  if ( iter_begin == linetoparse.end() ){
    iter_begin=find(linetoparse.begin(),linetoparse.end(),f);
  }
  if ( iter_begin == linetoparse.end() ){
    iter_begin=find(linetoparse.begin(),linetoparse.end(),s);
  }
  // We modify the default
  for (vector<string>::const_iterator iter=iter_begin;
       iter != linetoparse.end(); iter++ ){
    if ( iter == iter_begin ){
    rot_style=*iter;
    iter++;
    rot_level=atoi((*iter).c_str());
    break;
    }
  }

  // Using equivalence classes 
  // (if not, all permutations are computed)
  const string e="-e";
  bool equivalence=0;
  iter_begin=find(linetoparse.begin(),linetoparse.end(),e);
  if ( iter_begin != linetoparse.end() ) equivalence=1;

  
  // Debug
  const string debug="--debug", d="-d";
  bool debugging=0;
  iter_begin=find(linetoparse.begin(),linetoparse.end(),debug);
  if ( iter_begin == linetoparse.end() ){
    iter_begin=find(linetoparse.begin(),linetoparse.end(),d);
  }
  if ( iter_begin != linetoparse.end() ) debugging=1;

  // How to bring back the information, display print or file write
  const string output="--outputfile", o="-o",
    countonly="--count-only", c="-c";
  string outputfile;
  iter_begin=find(linetoparse.begin(),linetoparse.end(),output);
  if ( iter_begin == linetoparse.end() ){ 
    iter_begin=find(linetoparse.begin(),linetoparse.end(),o);
  }
  if ( iter_begin == linetoparse.end() ){ 
    iter_begin=find(linetoparse.begin(),linetoparse.end(),countonly);
  }
  if ( iter_begin == linetoparse.end() ){ 
    iter_begin=find(linetoparse.begin(),linetoparse.end(),c);
  }
  for (vector<string>::const_iterator iter=iter_begin; 
       iter != linetoparse.end(); iter++ ){
    if ( *iter == countonly || *iter == c ){
      // Count-only mode
      outputfile="/dev/null";
      break;
    } else if (*iter == output || *iter == o ) {
      iter++;
      if ( iter!=linetoparse.end() &&
	   *iter!=d && *iter!=debug &&
	   *iter!=s && *iter!=q && *iter!=f && *iter!=e ){
	outputfile=*iter;
	break;
      } else {
	// no filename was given after the flag -o, --outputfile
	outputfile="style"+rot_style+"_level-";
	outputfile+= rot_level < 0 ? "all" : itos<int>(rot_level);
	outputfile+=".dat";
	break;
      }
      iter--;
    }
  }

  // Debuggin flag for depuring errors or unexpected behaviour
  if (debugging){
  cout << "run_kubik(" << rot_style << "," << rot_level << ","
       << equivalence<< "," << outputfile << "," << debugging << ")" 
       << endl;
  }
  return run_rubik(rot_style,rot_level,equivalence,outputfile,debugging);
}
