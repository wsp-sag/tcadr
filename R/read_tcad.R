#' @title Read a TransCAD binary file
#'
#' @description Read a TransCAD \code{.bin} binary data file into the workspace.
#'
#' @param file The path and name of the binary data file, e.g.
#'   \code{data/foo.bin}. The \code{DCB} dictionary file should be in the same
#'   folder.
#' @param df_only Should the function return a `data.frame` or a list with
#' the descriptions appended? \code{default = TRUE}
#' 
#' @details If any variable descriptions are available in the dictionary file, 
#'   then they will be appended to the \code{attr(*, "label")} attribute (and 
#'   therefore visible in RStudio).
#'
#' @return If \code{df_only = TRUE}, a \code{tbl_df(data.frame)} object 
#'   implementation of the TransCAD data table. If \code{FALSE}, a list with two
#'   elements: \code{description} contains any data labels on the dictionary
#'   file; \code{df} is the data frame.
#'
#' @importFrom data.table setattr
#' @importFrom dplyr data_frame mutate_each_ tbl_df
#'
read_tcad <- function(file, df_only = TRUE){


  # Get file string for the DCB file.
  dcb_file <- gsub(".bin", ".DCB", file)

  # Read binary file attributes from DCB file
  row_length <- as.numeric(
    as.numeric(gsub("[^0-9]", "", readLines(dcb_file, 2)[2]))
  )

  dcb <- read_dcb(dcb_file)

  # Read each attribute in DCB from binary file.
  df <- dplyr::tbl_df(as.data.frame(
    get_df_from_binary(path.expand(file), 
                       dcb$name, dcb$type,
                       dcb$start, dcb$width,
                       row_length),
    stringsAsFactors = FALSE)
    )

  # strip white space from character strings
  character_vars <- names(df)[sapply(df, is.character)]
  df <- dplyr::mutate_each_(df, dplyr::funs(trim), character_vars)
  
  # add labels to data frame if they exist.
  if(any(!is.na(dcb$description))){
    for (i in seq_along(df))  
      data.table::setattr(df[[i]], name = 'label', value = dcb$description[i])
  }


  if(df_only){
    return(df)
  } else {
    object <- list()

    object[["description"]] <- data.frame(
      name = dcb$name,
      description = dcb$description
    )

    object[["df"]] <- df
  }

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

  # TransCAD can have up to the following items.
  dcb_names <- c(
    "name", "type", "start", "width", "decimals", "d_width", "d_decimals",
    "format", "agg_method", "description", "default", "split_method", "d_name"
  )
  names(dcb) <- dcb_names[1:length(names(dcb))]

  dcb
}

#' Trim all duplicate spaces from a character string.
#' 
#' @param x a character string.
#' @return a character string with extraneous spaces removed.
#' 
trim <- function (x) gsub("^\\s+|\\s+$", "", x)
