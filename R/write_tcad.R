#' @title Write a TransCAD binary file
#'
#' @description Write a TransCAD \code{.bin} binary data file fromt the workspace.
#'
#' @param df workspace dataframe object
#'
#' @param file_name The path and name of the binary data file, e.g.
#'   \code{data/foo.bin}.
#'
#' @details This version doesn't work well with NA's in the dataframe. 
#'
#' @return The \code{DCB} dictionary file will be writern to the same
#'   folder. Prints dcb file contents to console.Need to make it a user defined 
#'   option.
#'
write_bin <-  function(df,file_name){
    typs <- sapply(df,typeof)
    write_df_to_binary(df,file_name,typs)
  
  i <- sapply(df, is.factor) # which variables are factors?
  df[i] <- lapply(df[i], as.character) # convert them to char strings
}
