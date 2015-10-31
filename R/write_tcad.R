#' Write a TransCAD binary file
#'
#' @description Write a TransCAD \code{.bin} binary data file from the workspace.
#'
#' @param df workspace dataframe object
#'
#' @param file The path and name of the binary data file, e.g.
#'   \code{data/foo.bin}.
#'
#' @details This version doesn't work well with NA's in the dataframe. 
#'
#' @return The \code{DCB} dictionary file will be written to the same
#'   folder as the \code{.bin} file. 
#'
write_tcad <- function(df, file){
  
  i <- sapply(df, is.factor) # which variables are factors?
  df[i] <- lapply(df[i], as.character) # convert them to char strings
  
  typs <- sapply(df, typeof)
  write_df_to_binary(df, file, typs)
  
}
