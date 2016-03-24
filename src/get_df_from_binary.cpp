#include <string>
#include <Rcpp.h>
#include <iostream>
#include <fstream>
using namespace Rcpp;
using namespace std;

#define short_miss -32767
#define long_miss  -2147483647
#define flt_miss   -3.402823466e+38F
#define dbl_miss   -1.7976931348623158e+308

//' Scan a binary TransCAD File
//' 
//' This is a C++ implementation written by Amar Sarvepalli and adapted for Rcpp
//' by Greg Macfarlane.
//' 
//' @param bin_file string with the path to the .bin file.
//' @param name character vector of each variable.
//' @param type CharacterVector with the information type for each variable.
//' @param start NumericVector with the number of bytes in which the variable
//'   starts.
//' @param width NumericVector with the number of bytes given for a variable.
//' @param row_length int showing the number of bytes in a row.
//' 
//' @details All input parameter values are available in the \code{.DCB} file 
//'   read by \link{read_dcb}. This is an internal function and should not
//'   normally be used independent of \link{read_tcad_bin}.
//' 
//' @return characters in each element of the vector
//' 
// [[Rcpp::export]]
List get_df_from_binary(
    String bin_file,
    CharacterVector name, CharacterVector type,
    NumericVector start, NumericVector width,
    int row_length){

  // Create a list with the appropriate number of fields
  int n_fields = name.size();
  int line_pos;
  int n_rows;
  int file_size;
  char* memblock ;
  List my_list(n_fields);
  my_list.attr("names") = name;

  // Open the binary data file and make sure it exists
  std::fstream bf;
  bf.open(bin_file.get_cstring(), ios::in | ios::binary | ios::ate);
  if(!bf.is_open()){
    throw std::range_error("could not open binary file");
  }
  file_size = bf.tellg();
  n_rows = file_size / row_length;

  // Loop through fields
  for (int i = 0; i < n_fields; i++){

    //Integer fields
    if(type[i] == "I"){
      IntegerVector current_vec(n_rows);

      // Loop over rows in data
      for (int j = 0; j < n_rows; j++){
        // where do we read from?
        line_pos = j * row_length + start[i] - 1;
        bf.seekg(line_pos, ios::beg);

        bf.read((char*)&current_vec[j], 4);
        if(current_vec[j] == long_miss){
          current_vec[j] = NA_INTEGER;
        }
      }
      my_list[i] = current_vec;

    // Short fields (integer)
    } else if(type[i] == "S"){
      IntegerVector current_vec(n_rows);
      short x;
      
      for (int j = 0; j < n_rows; j++){
        line_pos = j * row_length + start[i] - 1;
        bf.seekg(line_pos, ios::beg);

        // bf.read((char*)&current_vec[j], 2);
        bf.read((char*)&x, 2);
        current_vec[j] = x;
        if(current_vec[j] == short_miss){
          current_vec[j] = NA_INTEGER;
        }
      }
      my_list[i] = current_vec;

    // Real fields (double)
    } else if(type[i] == "R"){
      NumericVector current_vec(n_rows);

      for (int j = 0; j < n_rows; j++){
        line_pos = j * row_length + start[i] - 1;
        bf.seekg(line_pos, ios::beg);

        bf.read((char*)&current_vec[j], 8);
        if(current_vec[j] == dbl_miss){
          current_vec[j] = NA_REAL;
        }
      }
      my_list[i] = current_vec;

    // Float fields (single)
    } else if(type[i] == "F"){
      NumericVector current_vec(n_rows);
      float x;
      
      for (int j = 0; j < n_rows; j++){
        line_pos = j * row_length + start[i] - 1;
        bf.seekg(line_pos, ios::beg);

        // bf.read((char*)&current_vec[j], 4);
        bf.read((char*)&x, 4);
        current_vec[j] = x;
        if(current_vec[j] == flt_miss){
          current_vec[j] = NA_REAL;
        }
      }
      my_list[i] = current_vec;

    // Character fields
    } else if(type[i] == "C") {
      CharacterVector current_vec(n_rows);

      // Loop over rows in data
      for (int j = 0; j < n_rows; j++){
        // where do we read from?

        memblock = new char [100];
        // clear previous values
        for(int s= 0; s < 100 ; s++){
          memblock[s] = '\0';
        }

        line_pos = j * row_length + start[i] - 1;
        bf.seekg(line_pos, ios::beg);

        bf.read(memblock, width[i]);
        current_vec[j] = memblock;
      }
      my_list[i] = current_vec;

    } else {
      my_list[i] = CharacterVector (n_rows, NA_STRING);
    }
  }


  //Close file
  bf.close();

  // change list to data.frame, and return
  return(my_list);
}
