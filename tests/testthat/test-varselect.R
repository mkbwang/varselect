library(testthat)
library(varselect)

data(Hitters)
mymodel = varselect(Salary~., data=Hitters, nvmax = 12)

subset_10 = c("(Intercept)", "AtBat", "Hits",  "Walks", "CAtBat","CRuns", "CRBI","CWalks", "Division","PutOuts","Assists")
real_cp = c(104.281319, 50.723090, 38.693127,27.856220,21.613011,14.023870,13.607126, 7.624674, 6.158685, 5.009317, 5.874113,7.330766)

test_that("Selected the correct variables", {
  expect_setequal(names(coef(mymodel, 10)),subset_10 )
})

test_that("Regression Numerical Results", {
  expect_equal(as.vector(mymodel$Cp), real_cp, tolerance = 1e-3)
})


