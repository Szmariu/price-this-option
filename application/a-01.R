# cpp25
# Paweł Sakowski
# University of Warsaw
# 2020-05-05

library(tidyverse)
library(ggplot2)

# 1. remove package if it exists
remove.packages("priceThisOption")
detach("package:priceThisOption", unload = TRUE) # if it still is in memory

# 2. install package and load to memory
#install.packages("../priceThisOption_0.1.0.tar.gz",
#                 type = "source",
#                 repos = NULL)

install.packages("../priceThisOption_0.1.0.zip",
                 type = "binary",
                 repos = NULL)
# load to memory
library("priceThisOption")

getGeometricAsianPrice(
  price = 75,
  spread = 5,
  vol = 0.25,
  r = 0.07,
  t = 252,
  nreps = 10000
)


testPrice <- function(price){
  getGeometricAsianPrice(
    price = price,
    spread = 5,
    vol = 0.25,
    r = 0.07,
    t = 252,
    nreps = 10000
  )
}

testPrice(100)

price <- seq(1, 200, by = 5)
result <- sapply(price, testPrice)


# 7. same plot using ggplot2 package
tibble(price, result) %>%
  ggplot(aes(x = price, y = result, col = "red")) +
  geom_line() +
  geom_point(size = 2, shape = 21, fill = "white") +
  labs(
    title = "price of European call option vs. time  to maturity",
    x     = "time to maturity",
    y     = "price of European call option"
  )

# 8. build an R function: option price vs. number of loops
MCEuropeanCallLoops <- function(loops) {
  return(
    MCEuropeanOptionPricer(0.5, 100, 95, 0.2, 0.06, loops, 0)
  )
}

MCEuropeanCallLoops(500)
loops  <- 2 ^ (1:25)
result <- sapply(loops, MCEuropeanCallLoops)

tibble(loops, result) %>%
  ggplot(aes(x = as.factor(loops), y = result)) +
  geom_point() + 
  labs(
    x = "number of loops",
    y = "price of European call option",
    title = "price of European call option vs. number of loops") +
  theme(axis.text.x = element_text(angle = 90))

# 9. build an R function: option price vs. spot and volatility
MCEuropeanCallSpotVol <- function(spot, vol) {
  return(
    MCEuropeanOptionPricer(0.5, 100, spot, vol, 0.06, 100000, 0)
  )
}
# call function once
MCEuropeanCallSpotVol(100, 0.2)

# sequences of argument values
spot <- seq(90, 105, by = 0.5)
vol  <- c(0.001, 0.01, 0.02, 0.05, 0.1, 0.15, 0.2, 0.3, 0.5, 1)

grid <- expand.grid(spot = spot, vol = vol)
result <- mapply(MCEuropeanCallSpotVol, spot = grid$spot, vol = grid$vol)
result.df <- data.frame(grid, result)
result.df

ggplot(data = result.df,
       aes(x = spot, y = result, group = vol, colour = vol)) +
  geom_line() +
  geom_point(size = 2, shape = 21, fill = "white") +
  labs(
    title = "price of European call option vs. spot price and volatility",
    x     = "spot price",
    y     = "price of European call option"
  )


