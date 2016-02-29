#' Read in TransCAD assignment file
#'
#' @description TransCAD returns fairly uniform assignment results containing
#'   times, speeds, volumes, etc. by direction. This function returns the flow
#'   information as a tidy data frame
#'
#' @param file Path to assignment binary file.
#' @param field The name of the field to collect; if \code{NULL} (default), then
#'   will return all fields.
#'
#' @import dplyr tidyr
#'
#' @export
#' 
read_assignment <- function(file, field = NULL){
  a <- read_tcad(file) %>% 
    rename(linkid = ID1)
  
  # select chosen fields if any 
  if(!is.null(field)){
    a <- select(a, linkid, matches(field))
  }
  
  a %>%
    gather(field, value, -linkid)  %>%
    separate(field, c("dir", "field"), extra = "merge")
}
