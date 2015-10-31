#include <Rcpp.h>
using namespace Rcpp;

#define short_miss -32767
#define long_miss  -2147483647
#define flt_miss   -3.402823466e+38F
#define dbl_miss   -1.7976931348623158e+30

//' Get the number of columns in a data frame
//' 
//' @author Amar Sarvepalli
//' @param df a data frame
//' @return int Number of colums
//'
int getColumns(List df){
  // if(!dataframe.inherits("data.frame")) stop("Input must be a data frame or list")
  int ncols = df.size();
  return ncols;
}

//' Get the number of rows in a data frame
//' 
//' @param df DataFrame object
//' @return nrows number of rows
//'
int getRows(List df){
  // if(!dataframe.inherits("data.frame")) stop("Input must be a data frame or list")
  List fisrtColumn(df[1]);
  int nrows = fisrtColumn.size();
  return nrows;
}

//' Get the dimensions of a data frame
//' 
//' @param df DataFrame object
//' @details To get number of dataframe dimensions
//' @return Array with the number of rows and columns (r x c)
//'
NumericVector getDims(List df){
  int nrows = getRows(df);
  int ncols = getColumns(df);
  NumericVector nrowcol = NumericVector::create(nrows, ncols);
  return nrowcol;
}

//' Get the names of the field headers
//' 
//' @param df DataFrame object
//' @return names field names
//'
CharacterVector getNames(List df){
  CharacterVector names(df.names());
  return names;
}

//' Get the field width in bytes
//' 
//' @param s character field
//' @return element_width width of the column
//'
int get_width(const std::string& s)
{
    int element_width = 0;
    return element_width = s.end() - s.begin();
}

//' @title parse line by comma deliminated
//' @name name_split
//' @details splits line by ","
//' @param line string line separated by delimiter
//' @param delim delimter can be anything
//' @return fields elements of string line
//'
std::vector<std::string> name_split(const std::string line, char delim){
    std::string word;
    std::vector<std::string> fields;
    std::stringstream ss(line);
    while (getline(ss, word, delim)) {
        fields.push_back(word);
    }
    return(fields);
}

//' Write a TransCAD Dictionary File
//' 
//' @param df Data frame
//' @param file Name and path to the \code{.DCB} file.
//'
// [[Rcpp::export]]
void write_dcb(List df, String file){

    FILE* pFile;

    // Get dcb file name
    std::vector<std::string> file_noext = name_split(file,'.');
    std::string dcbfName = file_noext[0]+".DCB";
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
      if (i == 0) { fprintf(pFile, "\n%d\n", total_bytes); }

      fprintf(pFile, "%s", dcb_fieldNames[i].c_str());
      fprintf(pFile, ",%s", dcb_fieldTypes[i].c_str());
      fprintf(pFile, ",%d", dcb_position[i]);
      fprintf(pFile, ",%d", dcb_fieldBytes[i]);
      fprintf(pFile, ",%d", dcb_decimal[i]);
      fprintf(pFile, ",%d", dcb_display[i]);
      fprintf(pFile, ",%s", blank);
      fprintf(pFile, ",%s", blank);
      fprintf(pFile, ",%s", blank);
      fprintf(pFile, ",%s", blank);
      fprintf(pFile, ",%s", blank);
      fprintf(pFile, ",%s", blank);
      fprintf(pFile, ",%s\n", blank);
    }
    fclose(pFile);
}

//' Write a binary TransCAD file from an R data frame.
//'
//' This is a C++ implementation written by Amar Sarvepalli and adapted for
//' Rcpp by Greg Macfarlane.
//' @param df List of variables (or a \code{data.frame}) that need to be
//'   written out to TransCAD.
//' @param file A character string giving the output file name and path.
//' @param field_types A character vector giving the data types of each column.
//'
//' @details Currently this function only exports only 3 datatypes: Character,
//'   Integer and Double.
//'
// [[Rcpp::export]]
void write_df_to_binary(
  List df,
  String file,
  CharacterVector field_types){

  // Check if n fields match to ftypes
  if(df.size() != field_types.size()){
    throw std::range_error( "length of \"field types\" is not same as number of fields in the dataframe");
  }
    
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
  pFile = fopen(file.get_cstring(),"wb");

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
              write_dcb(new_dcb_info, file);
              new_dcb_info_df = clone(dcb_info);
            }
        }

     }
  }

  fclose(pFile);
}

