
# Standalone testing: To test the cpp function, uncomment the following lines.
#  library(Rcpp)
#  sourceCpp('src/write_binary.cpp')
#------------------------------------------------------------------------------

#' @title Write a TransCAD binary file
#'
#' @description Write a TransCAD \code{.bin} binary data file fromt the workspace.
#'
#' @param file The path and name of the binary data file, e.g.
#'   \code{data/foo.bin}.
#' @param df_only Should the function return a `data.frame` or a list with
#' the descriptions appended? \code{default = TRUE}
#'
#' @details If any variable descriptions are available in the dictionary file,
#'   then they will be appended to the \code{attr(*, "label")} attribute (and
#'   therefore visible in RStudio).
#'
#' @return The \code{DCB} dictionary file will be writern to the same
#'   folder.
#'
write_tcad <-  function(df,file_name){
    typs <- sapply(df,typeof)
    write_binary(df,file_name,typs)
}
