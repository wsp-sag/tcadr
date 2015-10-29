tcadr
==============

This is a library of functions that can be used to read native TransCAD files
directly into R without intermediate conversion to `.csv` or `.dbf`. Currently 
the package will read binary data tables (including network attributes), but I 
hope to extend the package to cover shape data and networks in the future.

Installation
--------------
The package is not (yet) on CRAN. The easiest way to install it is with

    devtools::install_github("4Step/tcadr")
Current working version @pbsag is v0.0.2.2

    devtools::install_github("pbsag/tcadr@v0.0.2.2")
    
Using the [devtools](https://github.com/hadley/devtools) library will require you
to install RTools and Rccp, among other things. RStudio should make this mostly
automatic.

Use
--------------

    library(tcadr)
    ?read_tcad_bin()
    

This package is under active development, and any contribution is welcome.

