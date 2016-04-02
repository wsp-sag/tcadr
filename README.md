<!-- README.md is generated from README.Rmd. Please edit that file -->
tcadr
=====

[![Build Status](https://travis-ci.org/pbsag/tcadr.svg?branch=master)](https://travis-ci.org/pbsag/tcadr)

This is a library of functions that can be used to read native TransCAD files
directly into R without intermediate conversion to `.csv` or `.dbf`. Currently
the package will read and write binary data tables (including network
attributes), but I hope to extend the package to cover shape data and networks
in the future.

Installation
------------

The package is not (yet) on CRAN. The easiest way to install the latest stable
version is with

``` r
devtools::install_github("pbsag/tcadr")
```

Using the [devtools](https://github.com/hadley/devtools) library may require you
to install `RTools` and `Rccp`, among other things.

Use
---

``` r
library(tcadr)
dir("inst/extdata")
#> [1] "TransitR.bin" "TransitR.DCB"
read_tcad("inst/extdata/TransitR.bin")
#> Source: local data frame [40 x 24]
#> 
#>    Route_ID Route_Name Operator Company  Mode AM_Headway OFF_Headway
#>       (int)      (chr)    (int)   (int) (int)      (dbl)       (dbl)
#> 1         1     170 EB       NA      NA     4         60         120
#> 2         2     170 WB       NA      NA     4         60         120
#> 3         3      N2 NB       NA      NA     4         60          60
#> 4         4      N2 SB       NA      NA     4         60          60
#> 5         5      E1 EB       NA      NA     4         60          30
#> 6         6      E1 WB       NA      NA     4         60          30
#> 7         7       C EB       NA      NA     4         75          75
#> 8         8       C WB       NA      NA     4         75          75
#> 9         9       N EB       NA      NA     4         60          60
#> 10       10       N WB       NA      NA     4         60          60
#> ..      ...        ...      ...     ...   ...        ...         ...
#> Variables not shown: XferPen (dbl), AM.IWAIT (dbl), OFF_IWAIT (dbl), Time
#>   (dbl), RFacLocal (dbl), RFacPrem (dbl), FareFac (dbl), Fare (dbl),
#>   Line_No (chr), ONS.PEAK (dbl), OFFS.PEAK (dbl), ONS.OFFPK (dbl),
#>   OFFS.OFFPK (dbl), ONS.AM (dbl), OFFS.AM (dbl), ONS.PM (dbl), OFFS.PM
#>   (dbl)
```

This package is under active development, and any contribution is welcome.
