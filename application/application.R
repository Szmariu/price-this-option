### Libraries
# Install the custom package
install.packages("../priceThisOption_0.1.0.tar.gz",
                 type = "source",
                 repos = NULL)

library("priceThisOption")
library(tidyverse) # %>% etc.
library(ggplot2) # nice plots
library(ggthemes) 
library(ggtech) # For the airbnb theme

theme_set(theme_airbnb_fancy())
pink = "#FF5A5F"
orange = "#FFB400"
blueGreen = "#007A87"
flesh = "#FFAA91"
purple = "#7B0051"
options( scipen = 999 ) #avoiding e10 notation

# Basic test
getGeometricAsianPrice(
  price = 75,
  spread = 5,
  vol = 0.25,
  r = 0.07,
  t = 252,
  nreps = 100000
)

### Price vs. strike
# Define the wrapper
testPrice <- function(price){
  getGeometricAsianPrice(
    price = price, # Change the price
    spread = 5, # Ceteris paribus
    vol = 0.25,
    r = 0.07,
    t = 252,
    nreps = 10000 # Note the lower nreps, so it runs faster
  )
}

# Test it
testPrice(75)

# A sqeuence of prices
price <- seq(1, 200, by = 5)

# Run the fun for every price
# Takes a few seconds
result <- sapply(price, testPrice)

# Plot the results
tibble(price, result) %>%
  ggplot(aes(x = price, y = result)) +
  geom_area(fill = orange, color = orange, alpha = 0.5) +
  geom_point(size = 2, color = orange) +
  labs(
    title = "Price of the options vs. strike price",
    subtitle = "Ceteris paribus",
    x     = "Strike price",
    y     = "Price of the options"
  )




### Volatility vs. strike
testVolatility <- function(vol){
  getGeometricAsianPrice(
    price = 75,
    spread = 5,
    vol = vol,
    r = 0.07,
    t = 252,
    nreps = 10000 
  )
}

# Test it
testVolatility(0.25)

# A sqeuence of prices
vol <- seq(0, 1, by = 0.05)

# Run the fun for every price
# Takes a few seconds
result <- sapply(vol, testVolatility)

# Plot the results
tibble(vol, result) %>%
  ggplot(aes(x = vol, y = result)) +
  geom_area(fill = orange, color = orange, alpha = 0.5) +
  geom_point(size = 2, color = orange) +
  labs(
    title = "Price of the options vs. annualized volatility",
    subtitle = "Ceteris paribus",
    x     = "Strike price",
    y     = "Price of the options"
  )
