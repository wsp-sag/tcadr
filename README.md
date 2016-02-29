[![Build Status](https://travis-ci.org/pbsag/tcadr.svg?branch=master)](https://travis-ci.org/pbsag/tcadr)

tcadr
==============

This is a library of functions that can be used to read native TransCAD files
directly into R without intermediate conversion to `.csv` or `.dbf`. Currently 
the package will read and write binary data tables (including network
attributes), but I hope to extend the package to cover shape data and networks
in the future.

Installation
--------------
The package is not (yet) on CRAN. The easiest way to install the latest stable
version is with

    devtools::install_github("pbsag/tcadr")
    
Using the [devtools](https://github.com/hadley/devtools) library will require you
to install RTools and Rccp, among other things. RStudio should make this mostly
automatic.

Alternatively, you may download the source code from the links on the right and
install with 

    R CMD INSTALL tcadr.zip

Use
--------------

    library(tcadr)
    ?read_tcad()
    ?write_tcad()
    

This package is under active development, and any contribution is welcome.

