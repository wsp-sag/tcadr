tcadr
==============

This is a library of functions that can be used to read native TransCAD files
directly into R without intermediate conversion to `.csv` or `.dbf`. Currently 
the package will read binary data tables (including network attributes), but I 
hope to extend the package to cover shape data and networks in the future.

Installation
--------------
In the near future, you will be able to do this. 

    devtools::install_github("gregmacfarlane/tcadr")
    
But because that's not working right now (see 
[this issue](https://github.com/gregmacfarlane/tcadr/issues/1)), you will have 
to clone it and build it yourself. To do this:

  1. Install [Rstudio](http://www.rstudio.com/) and [git](http://git-scm.com/download/win)
  2. Click `New Project > Version Control > Git`, and then:
     - Repository URL: [https://github.com/gregmacfarlane/tcadr/](https://github.com/gregmacfarlane/tcadr)
     - Project directory name: *should automatically be* `tcadr`
     - Create project as subdirectory of: *wherever*
  3. Click `OK`.
  
When the project comes up, hit the `Build` tab, and then `Build & Reload`. The
package is now installed on your system.

Use
--------------

    library(tcadr)
    ?read_tcad_bin()
    

This package is under active development, and any contribution is welcome.

