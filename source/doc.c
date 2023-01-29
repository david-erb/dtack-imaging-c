/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




The purpose of this library is to support image processing
algorithm development.
The library itself is not considered a commercial product.

The purpose of this document is twofold:
first the organizational structure of the library,
second a reference to the data structures and function calls.

The library is loosely organized by functional units.
Each functional unit has a chapter dedicated to it.

Functional units are simply families of routines,
all of which operate on any particular data type.
In object oriented parlance, the functional units are classes.
The functions are the methods of the classes,
and the data structure they operate on our fee objects.

At the image level, the organization starts to fade from
object oriented into procedure oriented.
This manifests itself in the image class which
is a large class with very diverse methods.
There are not separate classes for different types of images.
If you consider this library to be a DSP library,
then the image is the primary input signal.

The major functional units listed in rough order of
decreasing generality:
- string operations
- error handling
- debug output
- console output
- operating system interface
- file handling
- interprocess communication
- one-dimensional operations: arithmetic, data type conversion, sorting
- major memory
- data compression
- histogram
- lookup tables
- parameter maintenance
- random numbers
- extendable stack
- configuration management
- image operations


The major operations on the image data type are:
- connected components
- data type conversion
- occurrence matrices
- two-dimensional convolution
- joint probabilities
- point operators: log, lookup,
- geometric transformations: resample
- statistical reduction: variance, sums, ranges
- arithmetic
- comparison
- synthetic image generation
- export and import
- annotation





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
