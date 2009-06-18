#ifndef GUARD_run_rubik_h
#define GUARD_run_rubik_h
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

// run_rubik.h
#include <string> // string: its member functions
#include <iostream> // Controls input output and end of line
#include <vector> // To form vectors
#include <map> // To form maps, pairs of key, value
#include <set> // To form sets, pairs of key, key
#include <deque> // To form double ended queues
#include <list>  // To form lists
#include <fstream> //When dealing with input/ouput in files
#include <cstdlib> // To execute system()
#include <stdexcept> // To have exceptions (i.e. out of range)
#include <sstream> // For the stringstream
#include <algorithm> // For the max and sort function
#include <ios> // To modify the size of output, cout.width()
//#include <ginac/ginac.h>

// Type definitions
typedef char ind; // The main type used for describing a facet
typedef short int inf; // The main type used for describing a family
typedef std::vector<ind> ind_v; // Type for describing a whole permutation cycle
typedef long long int gnumeric; // Type for large integers as turns searched

// Set
typedef std::set<int> arrayint;

// Maps
typedef std::map <inf, arrayint> arrayfe;
typedef std::map <ind_v, int> arrayfamcycle;

// Structs
struct Turn_Entry{
  gnumeric omap;
  ind_v operation;
  char level; // operation.size()
  ind_v permutation; // vector
  inf family_id; // family identificator
  int permutation_id; // permutation identificator
};
struct Turn_Description{
  struct Turn_Entry entry;
  std::string omap; // operation values mapped into an integer
  std::string sequence; // operation values mapped into (+-2)XYZ string
  std::string family_id; // family of the permutation 2*3^2*12
  std::string permutation_id; // integer defining our permutation
  std::string notation; // standard permutation notation (ab)(hj3)(klw)
};
struct Turn_Class{
  std::vector<Turn_Entry> member; // members of the class
};

// Deques
typedef std::deque<Turn_Entry> arrayte;
typedef std::deque<Turn_Class> arraytc;
typedef std::deque<Turn_Entry> arraytem1;
typedef std::vector<Turn_Description> arraytd;

// Functions
inf get_family_id(const ind_v&, arrayfamcycle&);
template <typename T> std::string itos(const T&);
std::string itos(const ind &);
int ipow(const ind&, const ind_v::size_type&);
gnumeric operation_to_omap(const ind_v&);
std::string operation_to_sequence(const ind_v&);
bool compare_notation(const Turn_Description&, const Turn_Description&);
std::vector<Turn_Description> sort_by_notation(const arraytd&);
ind cublet_position(const ind&);
ind facet_orientation(const ind&);
ind_v::size_type count_if_lt(const ind_v::const_iterator&, const ind_v::const_iterator&, const ind&);
ind_v::size_type count_if_le(const ind_v::const_iterator&, const ind_v::const_iterator&, const ind&);
ind_v::size_type count_if_gt(const ind_v::const_iterator&, const ind_v::const_iterator&, const ind&);
ind_v decode_perm(const ind_v&, const std:: vector<ind>&);
ind_v::size_type count_if_lt(const ind_v::const_iterator&, const ind_v::const_iterator&, const ind&);
ind_v::size_type count_if_le(const ind_v::const_iterator&, const ind_v::const_iterator&, const ind&);
std::string perm_to_notation(ind_v);
int perm_to_map(const ind_v&);
ind_v map_to_perm(int);
std::string perm_to_type(const ind_v&);
int compose_description(const arraytd&,	arraytd&);
int write_heading(const std::string&, const bool&);
int write_description(const arrayte&, const std::string&, const bool&);
int write_description(const arraytc&, const std::string&, const bool&);
std::string format_heading();
std::string format_entry(const Turn_Description&);
int write_level(const arraytd&, const std::string&);
Turn_Entry perm_product(const Turn_Entry&, const Turn_Entry&, arrayfamcycle&);
ind_v perm_inverse(const ind_v&);
ind_v perm_cycles_swap(ind_v);
int fill_class(Turn_Class&, arrayfamcycle&);
Turn_Entry fill_identity (arrayfamcycle&);
Turn_Entry fill_generator(const ind&, arrayfamcycle&);
int get_identity(arrayte&, arrayfe&, arrayfamcycle&);
int get_identity(arraytc&, arrayfe&, arrayfamcycle&);
int get_generators (const int&, arrayte&, arrayfe&, arrayfamcycle&);
int get_generators (const int&, arraytc&, arrayfe&, arrayfamcycle&);
bool insert_if_new(const Turn_Entry&, arrayfe&);
std::string do_sequence_product(const std::string&, const std::string&);
int get_moves(const arrayte&, arrayte&, arrayte&, arrayfe&, arrayfamcycle&);
int get_moves(const arraytc&, arraytc&, arraytc&, arrayfe&, arrayfamcycle&);
int generate_rotations(const int&, const int&, const bool&, const std::string&);
int run_rubik(const std::string&, int&, const bool&, const std::string&, const bool&);

#endif
