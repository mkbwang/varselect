

#' @export
print.summary.varselect = function(x, ...){
  cat("The variable selection order is \n")
  cat(paste(x, collapse=" "))
}


#' @export
summary.varselect <- function(object,...){
  rval = object$variables
  class(rval) <- "summary.varselect"
  rval
}


#' @export
coef.varselect <- function(object, id, ...){
  vars = c("Intercept", object$variables[1:id])
  coefs = object$coefs[1:(id+1), id]
  names(coefs) = vars
  coefs
}


#' @export
plot.varselect <- function(object, ...){
  Cp = as.vector(object$Cp)
  AIC = as.vector(object$AIC)
  BIC = as.vector(object$BIC)
  adjR2 = as.vector(object$adjR2)
  Order = c(1:length(Cp))
  criteria = data.frame(Cp, AIC, BIC, adjR2, Order)
  criteria = melt(criteria, id.vars=c("Order"), variable.name="Criterion", value.name="Score")
  
  p <- ggplot(criteria, aes(Order, Score)) + geom_line() + geom_point(size=1.2)
  p + facet_wrap(~ Criterion, scales = 'free', nrow = 2)
}
