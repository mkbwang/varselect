#' Varselect
#' 
#' Forward selection for linear regression
#'
#' @import Rcpp ggplot2 reshape2 stats
#' @importFrom Rcpp evalCpp

#' @param x optional Design matrix, coappears with \code{y} 
#' @param fml optional formula, coappears with \code{data}
#' @param y Response vector, coappear with \code{x}
#' @param data Optional data frame. coappears with \code{fml}
#' @param nvmax Maximal number of parameters included in the subset model
#' @return A list with multiple fields. Users can apply \code{summary}, \code{coef} and \code{plot} to explore the results. 
#' @examples
#' data(Hitters)
#' mymodel = varselect(Salary~., data=Hitters, nvmax=12)
#' summary(mymodel)
#' coef(mymodel, id=6)
#' plot(mymodel)
#' @useDynLib varselect
#' 
#' @rdname varselect
#' @export varselect
varselect = function(x,...){
  UseMethod("varselect",x)
}

#' @rdname varselect
#' @export
varselect.formula = function(fml, data, nvmax = 8){
  response_var = as.character(fml)[2]
  y = data[, response_var, drop=F]
  x = model.matrix(fml, data)[,-1] # we don't want intercept
  ncov = dim(x)[2]
  covariates = colnames(x)
  x = as.matrix(x)
  y = as.matrix(y)
  
  rval = fselect(x, y, nvmax, ncov)
  
  rval$variables = as.vector(rval$variables)
  if (!is.null(covariates)){
    rval$variables = covariates[rval$variables]
  }
  class(rval) <- "varselect"
  invisible(rval)
}


#' @rdname varselect
#' @export
varselect.default = function(x, y, nvmax = 8){
  ncov = dim(x)[2] # number of covariates
  covariates = colnames(x)
  x = as.matrix(x)
  y = as.matrix(y)
  
  rval = fselect(x, y, nvmax, ncov)
  
  rval$variables = as.vector(rval$variables)
  if (!is.null(covariates)){
    rval$variables = covariates[rval$variables]
  }
  class(rval)<-"varselect"
  invisible(rval)
}
