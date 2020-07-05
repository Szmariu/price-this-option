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
orange = "#FFB400"
purple = "#7B0051"

# Basic test
getGeometricAsianPrice(
  price = 75,
  spread = 5,
  vol = 0.25,
  r = 0.07,
  t = 252,
  nreps = 100000
)

### Price vs. price
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
# Warnings are normal, there are missing fonts for the theme 
tibble(price, result) %>%
  ggplot(aes(x = price, y = result)) +
  geom_area(fill = orange, color = orange, alpha = 0.5) +
  geom_point(size = 2, color = orange) +
  labs(
    title = "Price of the options vs. price of the instrument",
    subtitle = "Ceteris paribus",
    x     = "Price of the instrument",
    y     = "Price of the options"
  ) +
  theme(
    text = element_text(family = "sans"), # Remove the font warnings
    plot.title = element_text(family = "sans")
  )




### Volatility vs. price
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
    x     = "Annualized volatility",
    y     = "Price of the options"
  ) +
  theme(
    text = element_text(family = "sans"),
    plot.title = element_text(family = "sans")
  )



### Volatility and instrument price vs. price
testBoth <- function(price, vol){
  getGeometricAsianPrice(
    price = price,
    spread = 5,
    vol = vol,
    r = 0.07,
    t = 252,
    nreps = 10000 
  )
}

# Make all posible combinations
grid <- expand.grid(price = price, vol = vol)

# Run, takes more than a minute for all 840 variants
result <- mapply(testBoth, grid$price, grid$vol)

# Save as a df
result.df <- data.frame(grid, result)

# Plot the results
result.df %>% 
  ggplot(aes(x = price, y = result, colour = vol, group = vol)) +
    geom_line() +
    geom_point() + 
    scale_color_gradient(low = purple, high = orange) +
    labs(
      title = "Price of the strategy",
      subtitle = "Price of the underlying instrument and annualized volatility",
      x     = "Price of the instrument",
      y     = "Price of the strategy",
      color = 'Annualized 
volatility
      '
    ) + 
  theme(
    legend.title = element_text(size = 14),
    legend.key.size = unit(1.5, "lines"),
    legend.text = element_text(size = 14),
    text = element_text(family = "sans"),
    plot.title = element_text(family = "sans")
  )
