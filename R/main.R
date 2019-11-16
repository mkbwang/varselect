
varselect = function(x,...){
  UseMethod("varselect",x)
}

varselect.default = function(x, y, nvmax = 8,
                             method=c("forward", "backward")){
  ncov = dim(x)[2] # number of covariates
  covariates = names(x) 
  rval = switch(1+pmatch(method[1],
                         c("forward", "backward"),
                         nomatch=0),
                stop(paste("Ambiguous or unrecognised method name :",method)),
                fselect(x, y, nvmax, ncov),
                bselect(x, y, nvmax, ncov))
}

