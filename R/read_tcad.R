read_tcad_bin <- function(dir, file_name){

  # Create structure for object returned to
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

  # Read each attribute in DCB from a binary file.
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
