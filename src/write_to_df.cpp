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

  //Rcout << bin_file;


  // change list to data.frame, and return
  return(my_list);
}
