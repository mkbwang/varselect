# varselect

<!-- badges: start -->
  [![Travis build status](https://travis-ci.org/skybullbobby/varselect.svg)](https://travis-ci.com/skybullbobby/varselect)
  <!-- badges: end -->

## Introduction

This is one homework assignment from [Biostatistics 625](https://sph.umich.edu/academics/courses/course.php?courseID=BIOSTAT625) at the University of Michigan. The homework assignment is designing an R package from scratch and compare the correctness and efficiency with present packages that carries out the same functionality. 



`varselect` is a package containing a function with the same name that carries out forward variable selection for linear regression. The user can define the maximum number of variables in the subset besides feeding in the predictors and the response variable. 



## Installation

To install the package, type in the R command line.

```R
install_github("skybullbobby/varselect", build_vignettes = T)
```

You can learn how to use the package by viewing the vignette

```R
browseVignettes("varselect")
```

or type `?varselect` in the R command line.

## Benchmarking

I use benchmarking to compare the running speed of my `varselect` function and the `regsubsets` function from `leaps` package. Note that I am only comparing the speed. To compare correctness, there is already unit testing code in the package.



Because the output of my function isn't exactly the same as the output from the `regsubsets` function, I had to tweak a little bit to get the benchmarking procedure working.

```{r}
library(bench)
library(varselect)
library(leaps)

# random predictor matrix
x = matrix(sample.int(5, size = 300*50, replace = TRUE), nrow = 300, ncol = 50)

# random response vector
y = matrix(sample.int(5, size = 300, replace = TRUE), nrow = 300, ncol = 1)

# the benchmark function tailored for varselect
bench_varselect = function(mymatrixx, mymatrixy){
  myresult = varselect(mymatrixx, mymatrixy, nvmax=25)
  return (1)
}

# the benchmark function tailored for regsubsets
bench_leaps = function(mymatrixx, mymatrixy){
  myresult = regsubsets(mymatrixx, mymatrixy, nvmax=25, method="forward")
  return (1)
}

# benchmarking
result = bench::mark(bench_varselect(x, y), bench_leaps(x, y))
print(result)
plot(result)
```

The benchmarking result is disappointing. The plot shows that while the original leaps package finish in about 1ms, my function needs about 40ms. I may be able to improve my code's performance if I use RcppParallel, but there must be something algorithmically different in the leaps package code.

![benchmarkresult](https://github.com/skybullbobby/varselectbenchmark.png)

## Limitations

* If you are feeding into the `varselect` function with raw predictor matrices, please only input numeric values.
* The output of my function isn't exactly the same as the `regsubsets` function in the leaps package (I don't quite understand a number of items in the `regsubsets` function output).

## References

* This package is inspired by the [leaps](https://github.com/cran/leaps) package written by Thomas Lumley.
* The package relies on [ggplot](https://ggplot2.tidyverse.org/) and [reshape](https://cran.r-project.org/web/packages/reshape2/index.html) to provide the plots and [RcppArmadillo](https://cran.r-project.org/web/packages/RcppArmadillo/index.html) to finish the C++ codes.

