#' @title Read a TransCAD binary file
#'
#' @description Read a TransCAD \code{.bin} binary data file into the workspace.
#'
#' @param dir The path to the directory where the files are located
#' @param file_name The name of the binary file, e.g. \code{foo.bin}
#'
#' @return A list with two elements: \code{description} contains any data labels
#' on the dictionary file; \code{data} is a \code{tbl_df(data_frame)} object with
#' the table.
read_tcad_bin <- function(dir, file_name){

  # Create structure for object returned to user.
  dir <- "data"
  file_name <- "TransitR.bin"
  object <- list()

  # Get file strings for the bin file as well as the DCB file.
  base_file_name <- gsub(".bin", "", file_name)
  bin_file <- file.path(dir, file_name)
  dcb_file <- file.path(dir, paste(base_file_name, "DCB", sep = "."))

  # Read binary file attributes from DCB file

  # row 2 has byte length of row
  row_length <- as.numeric(readLines(dcb_file, 2))[2]

  dcb <- read_dcb(dcb_file)
  object[["description"]] <- data.frame(
    name = dcb$name,
    description = dcb$description
  )

  # Read each attribute in DCB from binary file.
  df <- dplyr::tbl_df(as.data.frame(
    get_df_from_binary(bin_file, dcb$name, dcb$type,
                       dcb$start, dcb$width,
                       row_length),
    stringsAsFactors = FALSE)
    )


  return(object)
}


#' @title Read the TransCAD dictionary file
#'
#' @description Read the TransCAD \code{.DCB} dictionary file. This is a helper
#' function and is not normally called by the user directly.
#'
#' @param dcb_file A character string location of a TransCAD dictionary file
#' containing information on the layout of the binary data file and its
#' attribute properties.
#'
#' @return A \code{data_frame} of the attributes including type, description,
#' and layout of the rows in the binary data.
read_dcb <- function(dcb_file){

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
