#' @title Read a TransCAD binary file
#'
#' @description This function reads a TransCAD \code{.bin} binary data file that
#' is defined by a \code{.dcb} dictionary file.
#'
#' @param dir The path to the directory where the files are located
#' @param file_name The name of the binary file, e.g. \code{foo.bin}
#' 
#' @return A list with two elements: \code{description} contains any data labels 
#' on the dictionary file; \code{data} is a \code{tbl_df(data_frame)} object with
#' the table.
read_tcad_bin <- function(dir, file_name){

  # Create structure for object returned to user.
  object <- list()

  # Get file strings for the bin file as well as the DCB file.
  base_file_name <- gsub(".bin", "", file_name)
  bin_file <- file.path(dir, file_name)
  dcb_file <- file.path(dir, paste(base_file_name, "DCB", sep = "."))

  # Read binary file attributes from DCB file
  dcb <- read_dcb(dcb_file)
  object[["description"]] <- data.frame(
    name = dcb$name,
    description = dcb$description
  )

  # Read each attribute in DCB from binary file.



  return(object)
}


read_dcb <- function(dcb_file){
  # The first line is empty, and the second says how long the records are
  row_length <- as.numeric(readLines(dcb_file, 2))[2]

  # afterwards, the dcb file is just a csv file with the attributes
  dcb <- read.csv(
    dcb_file,
    stringsAsFactors = FALSE,
    skip = 2,
    header = FALSE)

  names(dcb) <- c(
    "name", "type", "start", "width", "decimals", "d_width", "d_decimals",
    "format", "agg_method", "description", "default", "split_method", "d_name"
  )

  dcb
}
