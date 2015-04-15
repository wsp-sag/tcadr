#include <string>
#include <Rcpp.h>
#include <iostream>
#include <fstream>
using namespace Rcpp;
using namespace std;

// [[Rcpp::export]]
List get_df_from_binary(
    String bin_file,
    CharacterVector name, CharacterVector type,
    NumericVector start, NumericVector width,
    int row_length){

  // Create a list with the appropriate number of fields
  int n_fields = name.size();
  List my_list(n_fields);
  my_list.attr("names") = name;

  // Open the binary data file and make sure it exists
  std::fstream bf;
  bf.open(bin_file, ios::in | ios::binary | ios::ate);
  if(!bf.is_open()){
    throw std::range_error("could not open binary file");
  }

  int file_size = bf.tellg();
  int n_rows = file_size / row_length;



  //Rcout << bin_file;
  Rcout << "row length" << row_length << endl;
  Rcout << "number of fields" << n_fields << endl;
  Rcout << "number rows" << n_rows << endl;
  Rcout << "file size" << file_size << endl;

  //Close file
  bf.close();

  // change list to data.frame, and return
  return(my_list);
}
