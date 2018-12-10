#!/usr/bin/env Rscript
# load the dataset or do needful
page_views <- read.csv("C_data.csv")

attach(page_views)
png(filename = "C_hist.png")
hist(pgviews, main = "Page Views Histogram C")
dev.off()

png(filename = "C_density.png")
plot(density(pgviews), log = "y", main = "log(density) C")
dev.off()
detach(page_views)


# load the dataset or do needful
page_views <- read.csv("T_data.csv")

attach(page_views)
png(filename = "T_hist.png")
hist(pgviews, main = "Page Views Histogram T")
dev.off()

png(filename = "T_density.png")
plot(density(pgviews), log = "y", main = "log(density) T")
dev.off()
detach(page_views)


# load the dataset or do needful
page_views_conf <- read.csv("T_C_conf.csv")

attach(page_views_conf)
png(filename = "T_C_conf.png")
plot(delta, confidence_level, type = "b", main = "Confidence vs. delta, T_C")
dev.off()

png(filename = "T_C_z.png")
plot(delta, z, type = "b", main = "z vs. delta, T_C")
dev.off()
detach(page_views_conf)

