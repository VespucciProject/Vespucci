Guidelines for Potential Contributors
============================================
Thank you for your interest in contributing to the Vespucci Project. These guidelines should help you make a valuable contribution to the project. They cover the process of contributing to Vespucci, the process of adding a spectral pre-processing method and the process of adding a spectral analysis method. By following these guidelines, we hope Vespucci can attain a higher degree of quality than other research code.

Contributing to Vespucci
========================
The issues page on GitHub includes features we would like to see added to Vespucci that we are currently not working on. If you have a contribution to make, comment on one of these issues (or start your own) and we may assign the issue to you.

If you have code to contribute to Vespucci, simply make a pull request with your changes to the VespucciProject GitHub page. The contribution should include unit tests for at least the functions added to the `Vespucci::Math` namespace. The pull request will be automatically built by our build service providers, which will execute unit tests (provided you have added them to the Test.pro project). The code will be examined for style and quality by the maintainer, and if all tests pass and the contribution is deemed within the mission of the project, your contribution will be integrated into the code base and your name added to our list of contributors. Any code contributed must compile, test, and run successfully on all three of Vespucci's target platforms.

If you have already implemented a method not found in Vespucci in MATLAB or Octave, take a look at the [syntax conversion table](http://arma.sourceforge.net/docs.html#syntax). Re-writing MATLAB code in C++ using Armadillo is fairly easy.

If you are uncomfortable with Qt, but have a meaningful math function to contribute to the library, feel free to make a contribution. The user interface can be created later. Bug fixes and code that improves performance or clarity of existing functions are also welcome.

Libraries
=========
Generally, code contributed to the Vespucci project can rely only on the following libraries:
* Qt
* Boost
* Armadillo
* mlpack

If there is a compelling reason to use a different library than the ones listed above, please discuss it with us using the issues tab before you start writing code. Any library that is to be used in Vespucci must be regularly built and tested on Windows 7 (using MSVC and GCC), Mac OS 10.7 (Using clang), and Ubuntu 14.04 LTS (or a similar GNU/Linux distro, using GCC). If the library is not regularly tested on one of these platforms, and there is compelling reason to do so, we will set-up regular testing using Travis-CI and or Appveyor. As Vesucci is distributed under the terms of the GPL, any additional library used must use a license acceptable for GPL software.

Code Style
==========
Style Guides
------------
Vespucci tries to adhere to the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). However, none of the libraries Vespucci links to follow this guide. Armadillo uses underscore_case for all names and mlpack and Qt use camelCase for all names. The following exceptions (and perhaps others) apply:
* Source files take the extension `.cpp`
* Member functions that call their member's member functions take the same style as their member's member function (e.g. if we write a function in a `QDialog` class that calls the `addGraph` member of a `QCustomPlot` object, we name the member of the `QDialog` "`addGraph()`", rather than the stylistically preferred "`AddGraph()`").

Names
-----
* Both member functions and functions that do not belong to a class are named in PascalCase, unless they are getters.
* Member variables are denoted in underscore_case, with a trailing underscore (e.g. `name_`, `spectra_`, etc).
* Variables that are not members are denoted in underscore_case.
* Setters are named in PascalCase like other functions, but are named after the variables they set (e.g. `SetName()` for the setter of the `name_` member).
* Getters are named after the member they return (e.g. the getter for `abscissa_` is named `abscissa()`). Getters that return pointers to members have `_ptr` appended to the end of their names. Where getters that return copies and getters that return references both exist, the getter that returns the reference is named with `_ref` appended.
* Every function belongs to a namespace, either the namespace of its parent class or a namespace like `Vespucci::Math` or `BinaryImport`.
* Widgets in Qt forms are named using Qt camelCase style inside .ui files. The type of the widget should be included in the name because they have to be called something. Widgets should be accessed using the ui member of the form class.
* As mentioned above, an exception exists for a function whose sole purpose is to call the member of one of the class's members.

Types
-----
Variables in Vespucci should use the following types:
* Numeric data should use armadillo types whenever possible.
* Data to be displayed to the user should use Qt types whenever possible, converting them to standard library types only when necessary.
* If a variable is expected to be unsigned, it should use an unsigned type.

Adding Processing Methods to Vespucci
=====================================
To add a processing method to Vespucci, the following must be done:
* A member function must be added to `VespucciDataset` to execute the analysis.
* If the method requires more than 5 lines of code, a function performing the method must be included in the `Vespucci::Math` namespace in the Vespucci library.
* A form class subclassed from `QDialog` must be created, or an existing dialog expanded to handle the new method.

Processing GUI Classes
----------------------
If a class already exists for performing a processing step substantially similar to the method to be added,
the existing class should be expanded by the addition of widgets to handle user input. Widgets may also simply be reused with their `QLabel`s changed. If a new form class must be created, follow the same procedure as you would for a new analysis form class, documented in the subsection "Analysis GUI Classes" of the section "Adding Analysis Methods to Vespucci".

Adding Analysis Methods to Vespucci
===========================
To add an analysis method to Vespucci, the following must be done:
* A member function must be added to `VespucciDataset` to execute the analysis. This member must take `QString name` as its first parameter.
* If a method has not yet been implemented in mlpack, a function to execute the analysis must be created in the `Vespucci::Math` namespace of the VespucciLibrary.
* A class must be created to handle data generated by the analysis, unless mlpack has already done this.
* If a method is already implemented in mlpack, the `AnalysisResults` may be constructed in the `VespucciDataset` member function.
* A form class subclassed from `QDialog` to allow the user to enter parameters.

Analysis GUI Classes
--------------------
GUI classes to handle the input of parameters from the user must have the following:
* A constructor which takes the current `QModelIndex` from dataset tree view and obtains a `QSharedPointer<VespucciDataset>` to the dataset the analysis is to be performed on, and calls `findChild` on the required `QWidget` members.
* A member called `data_` or `dataset_` which contains a `QSharedPointer<VespucciDataset>` corresponding to the active dataset.
* Pointers to the appropriate `QWidget`s that interact with the user.
* Correct names for the widgets. A `QWidget` that is called "thingWidget" in the .ui file should have a pointer named `thing_widget_` in the class. Widgets are named in the conventional Qt style within forms, but in Google-esque style within the C++ classes. The base type of the widget must be included in the name (e.g. `name_line_edit_` for the `QLineEdit` object that takes string representing a name from a user).


`VespucciDataset` member functions
-----------------------------------
Member functions to perform an analysis must do the following:
* Take the name of the object to display to the user and use as a key in `analysis_results_`.
* Perform the analysis through a class designed to handle the analysis (either bespoke or included from mlpack)
* Add a `QSharedPointeR<AnalysisResults>` object to `analysis_results_` map containing the matrices generated by the analysis. This is obtained from the `GetData()` method of the analysis handler or is initialized in the `VespucciDataset` member in the case of analysis methods implemented by mlpack.

Classes to Handle Analysis Data
-------------------------------
A `VespucciDataset` contains all analysis methods that may be called on it. Each analysis has a helper object which takes the data as a reference from the dataset. Helper objects must implement the following members:
* A constructor which takes the name of the result and relevant metadata
* Private members of `arma::mat` type which store the results of the analysis. It is customary to use the member `results_` when a matrix is returned from an analysis function, and to name these members the same as the parameters of the analysis function (remembering to add the trailing underscore used for members in Vespucci).
* A method called `Apply()` to which is passed `spectra_` and perhaps `abscissa_`, along with the parameters of the analysis that are taken in the `VespucciDataset` analysis member function. This function calls the functions from the Vespucci library that are required for the analysis.
* A method called `GetData()`, which heap-allocates an `AnalysisResults` object in a `QSharedPointer` and calls the  `AppendResult()` method of the `AnalysisResults` object to add matrices. Each matrix should have name that constitutes a unique key, and if necessary, column headings should be provided in a `QStringList` for at least the first 15 columns of each matrix.

Analysis Functions in the `Vespucci::Math` Namespace
----------------------------------------------------
Analysis methods must be implemented in either mlpack or armadillo, or in the `Vespucci::Math` namespace. A few style rules apply to this namespace that do not apply to Vespucci in general:
* All matrices on which operations are to be performed are to be taken as constant references `const arma::mat&`. If the matrix itself is to be modified, the function should return a copy or include a copy as a non-`const` reference parameter.
* The `using` directive should not to be used so as to avoid confusion between functions in the `std` and `arma` namespaces.
* To ease wrapping with other languages, Qt classes are to be avoided. The equivalent C++ standard library class should be used instead (e.g. `std::string` instead of `QString`). This is in contrast to the Vespucci GUI program, where Qt types are preferred.
* Armadillo, Boost, and the standard library are the only libraries that may be used. This is intended to make the code readable by users who are only familiar with languages like MATLAB.
* Unit tests must be written using the Boost unit test framework.
* Functions that check for success must have return values of type `bool`.
* Each analysis that operates on single spectra must include a function that takes a single spectrum and a function that takes a column-major matrix of spectra. The function that takes a matrix will have the same name as the function that takes a vector, but with `Mat` appended to the end of the function name (e.g. `QuantifyPeak` and `QuantifyPeakMat`, where `QuantifyPeak` returns a `arma::rowvec` and `QuantifyPeakMat` returns an `arma::mat`).
* If a matrix is expected to contain only one column, the `arma::vec` type should be used. If a matrix is expected to contain only one row, `arma::rowvec` type should be used.
* If a value is expected to be unsigned, use `arma::uword` for integers and `unsigned double` for floating-point numbers.
* Any function that can throw an exception should be inside of a `try/catch` block. The `catch` block must write the function call that threw the exception to `stdout` and throw the same exception again.
* A function returning a matrix with more than one column for each spectrum should include these matrices in an `arma::field<arma::mat>` type.
* Each function should be defined in a file with the same name as the header it is declared in and each type of analysis should include its own header and source file.
* The use of C++11 features is highly recommended when they reduce the complexity of the code.

Writing Tests
=============
All methods in the Vespucci library are unit tested to ensure code quality and reproducibility of results. The project located in the Test folder is used to run all unit tests on math functions. Example datasets are provided, including real-world and generated spectra. Unit tests written for functions in the Vespucci library should use the Boost unit test framework. Tests written for Qt classes should use QtTest. Some methods, such as Vertex Components Analysis are untestable as they produce different results each time they are run on the same data. These functions should only be tested for the validity of their output, not for the values.
