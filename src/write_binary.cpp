#include <Rcpp.h>
using namespace Rcpp;

//' @author Amar Sarvepalli
//' @param  List or DataFrame object
//' @return int columns
//' To get number of dataframe columns
//'
// [[Rcpp::export]]
int getColumns(List df){
  // if(!dataframe.inherits("data.frame")) stop("Input must be a data frame or list")
  int ncols = df.size();
  return ncols;
}

//' To get number of dataframe rows
//'
// [[Rcpp::export]]
int getRows(List df){
  // if(!dataframe.inherits("data.frame")) stop("Input must be a data frame or list")
  List fisrtColumn(df[1]);
  int nrows = fisrtColumn.size();
  return nrows;
}

//' To get number of dataframe dimensions
//'
// [[Rcpp::export]]
NumericVector getDims(List df){
  int nrows = getRows(df);
  int ncols = getColumns(df);
  NumericVector nrowcol = NumericVector::create(nrows, ncols);
  return nrowcol;
}

//' Method to extract dataframe column names
//'
// [[Rcpp::export]]
CharacterVector getNames(List df){
  CharacterVector names(df.names());
  return names;
}


//' Method to get column width
//'
// [[Rcpp::export]]
int get_width(const std::string& s)
{
    int element_width = 0;
    return element_width = s.end() - s.begin();
}

//' Split string (splits line by ",")
// [[Rcpp::export]]
std::vector<std::string> name_split(const std::string line, char delim){
    std::string word;
    std::vector<std::string> fields;
    std::stringstream ss(line);
    while (getline(ss, word, delim)) {
        fields.push_back(word);
    }
    return(fields);
}


//' writes dcb info
//' @param List of field info
//' @param String filename
// [[Rcpp::export]]
void write_dcb(List df, String file_name){

    FILE* pFile;

    // Get dcb file name
    std::vector<std::string> file_name_noext = name_split(file_name,'.');
    std::string dcbfName = file_name_noext[0]+".DCB";
    pFile = fopen(dcbfName.c_str(),"w");

    // Get the data frame lists
    std::vector <std::string> dcb_fieldNames = as<std::vector <std::string> >(df["Name"]);
    std::vector <std::string> dcb_fieldTypes = as<std::vector <std::string> >(df["Type"]);
    IntegerVector dcb_fieldBytes = as<IntegerVector>(df["Bytes"]);
    IntegerVector dcb_position = as<IntegerVector>(df["Position"]);
    IntegerVector dcb_decimal = as<IntegerVector>(df["Decimal"]);
    IntegerVector dcb_display = as<IntegerVector>(df["Display.width"]);

    const char* blank = "";
    int total_bytes = 0;
    int ncols = dcb_fieldNames.size();

    // Count total bytes
    for (int i = 0; i < ncols; i++){
        total_bytes += dcb_fieldBytes[i];
    }

    // write to output
    for (int i = 0; i < ncols; i++){
      if (i == 0) { fprintf(pFile, "\n %d", total_bytes); }

      fprintf(pFile, "\n %s", dcb_fieldNames[i].c_str());
      fprintf(pFile, ", %s", dcb_fieldTypes[i].c_str());
      fprintf(pFile, ", %d", dcb_position[i]);
      fprintf(pFile, ", %d", dcb_fieldBytes[i]);
      fprintf(pFile, ", %d", dcb_decimal[i]);
      fprintf(pFile, ", %d", dcb_display[i]);
      fprintf(pFile, ", %s", blank);
      fprintf(pFile, ", %s", blank);
      fprintf(pFile, ", %s", blank);
      fprintf(pFile, ", %s", blank);
      fprintf(pFile, ", %s", blank);
      fprintf(pFile, ", %s", blank);
      fprintf(pFile, ", %s", blank);
    }
    fclose(pFile);
}

//'  Method to export data from dataframe to binary file
//'  This exports only 3 datatypes: Character, Integer and Double
//'  Writes the dcb file
//'  @author Amar Sarvepalli
//'  @param List or DataFrame
//'  @param List datatypes
//'  @date 10-22-2015
//'
// [[Rcpp::export]]
DataFrame write_binary(List df, String file_name, CharacterVector field_types){

  // Check if n fields match to ftypes
  if(df.size() != field_types.size()) stop("length of \"field types\" is not same as number of fields in the dataframe");
  NumericVector nrowcols = getDims(df);
  int nrows = nrowcols[0];
  int ncols= nrowcols[1];

  // dcb info fields
  List new_dcb_info;
  DataFrame new_dcb_info_df;
  CharacterVector dcb_fieldNames = getNames(df);
  CharacterVector dcb_fieldTypes(ncols);
  CharacterVector dcb_blank(ncols);
  IntegerVector dcb_fieldBytes(ncols);
  IntegerVector dcb_position(ncols);
  IntegerVector dcb_decimal(ncols);
  IntegerVector dcb_display(ncols);

  FILE* pFile;
  pFile = fopen(file_name.get_cstring(),"wb");

  // write binary data by rows
  for(int r = 0; r < nrows; r++){
    for (int c = 0; c < ncols; c++ ){

       // Double
       if (field_types[c] == "double") {
           if (r == 0){
              dcb_fieldTypes[c] = "R";
              dcb_fieldBytes[c] = 8;
              dcb_decimal[c] = 4; // write a method to check decimals
           }
           double vec_d = as<NumericVector>(df[c])[r];
           fwrite((char*)&vec_d,8,1,pFile);
       }

       // Integer
       if (field_types[c] == "integer") {
           if (r == 0){
              dcb_fieldTypes[c] = "I";
              dcb_fieldBytes[c] = 4;
              dcb_decimal[c] = 0;
           }
          int vec_i = as<IntegerVector>(df[c])[r];
          fwrite((char*)&vec_i,4,1,pFile);
       }

       // Character
       if (field_types[c] == "character") {
          std::vector <std::string> vec_c= as<std::vector <std::string> >(df[c]);
          int col_width = 0;
         // Check all elements and get field width
         if (r == 0){
             for (int i = 0; i < (int)vec_c.size(); i++){
               if(col_width < get_width(vec_c[i])){ col_width = get_width(vec_c[i]);}
             }
             dcb_fieldTypes[c] = "C";
             dcb_fieldBytes[c] = col_width;
             dcb_decimal[c] = 0;
          }

          fwrite(vec_c[r].c_str(),dcb_fieldBytes[c],1,pFile);
       }

        // write dcb header file
        if (r == 0){
            dcb_blank[c] = "";
            dcb_display[c] = dcb_fieldBytes[c] + 4;
            if (c == 0) {
                 dcb_position[c] = 1;
            } else {
                 dcb_position[c] = dcb_position[c-1] + dcb_fieldBytes[c-1];
            }

            // Create a data frame object for dcb info
            if (c == (ncols-1)) {
               List dcb_info = List::create( _["Name"]= dcb_fieldNames,
                                       _["Type"]= dcb_fieldTypes,
                                       _["Position"]= dcb_position,
                                       _["Bytes"]= dcb_fieldBytes,
                                       _["Decimal"]= dcb_decimal,
                                       _["Display.width"]= dcb_display,
                                       _["Display.decimal"]= dcb_decimal,
                                       _["Display.Format"]= dcb_blank,
                                       _["Default.Methods"]= dcb_blank,
                                       _["Field.Description"]= dcb_blank,
                                       _["Default.Value"]= dcb_blank,
                                       _["JoinSplit.Method"]= dcb_blank,
                                       _["Display.Name"]= dcb_blank
                                      );
              new_dcb_info = dcb_info;
              write_dcb(new_dcb_info, file_name);
              new_dcb_info_df = clone(dcb_info);
            }
        }

     }
  }

  fclose(pFile);
  return new_dcb_info_df;
}

