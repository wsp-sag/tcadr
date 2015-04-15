#include <string>
#include <Rcpp.h>
#include <iostream>
#include <fstream>
using namespace Rcpp;
using namespace std;


// [[Rcpp::export]]
void get_df_from_binary(
    String bin_file,
    CharacterVector name, CharacterVector type,
    NumericVector start, NumericVector width,
    int row_length){

  int number_fields = name.size();

  //Rcout << bin_file;
  Rcout << number_fields << endl;


}
