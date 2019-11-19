
varselect = function(x,...){
  UseMethod("varselect",x)
}

# by default it receives the xmatrix, ymatrix and nvmax
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

# it also receives formula and data as first two parameters
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
