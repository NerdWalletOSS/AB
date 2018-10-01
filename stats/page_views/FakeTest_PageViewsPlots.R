#!/usr/bin/env Rscript
# store the current directory
# initial.dir<-getwd()
# change to the new directory
# setwd("/Users/ranjeettate/Documents/WORK/Statistics/PageViews")
# load the necessary libraries
# library(nlme)
# set the output file
# sink("FakeData_V1.out")
# load the dataset or do needful
page_views_V1 <- read.csv("./Output/FakeTest_V1.csv")

png(filename = "./Output/FakeTest_V1_hist.png")
hist(page_views_V1$pgviews)
dev.off()

png(filename = "./Output/FakeTest_V1_density.png")
plot(density(page_views_V1$pgviews), log = "y")
dev.off()

# close the output file
# sink()

# sink("FakeData_C.out")
# load the dataset or do needful
page_views_C <- read.csv("./Output/FakeTest_C.csv")

png(filename = "./Output/FakeTest_C_hist.png")
hist(page_views_C$pgviews)
dev.off()

png(filename = "./Output/FakeTest_C_density.png")
plot(density(page_views_C$pgviews), log = "y")
dev.off()

# close the output file
# sink()

# sink("FakeData_$variant.out")
# load the dataset or do needful
page_views_V1_C_conf <- read.csv("./Output/FakeTest_V1_C_conf.csv")

attach(page_views_V1_C_conf)
png(filename = "./Output/FakeTest_V1_C_conf.png")
plot(delta, confidence_level, type = "b", main = "Confidence vs. delta, FakeTest_V1_C")
dev.off()

png(filename = "./Output/FakeTest_V1_C_z.png")
plot(delta, z, type = "b", main = "z vs. delta, FakeTest_V1_C")
dev.off()

# close the output file
# sink()

# unload the libraries
# detach("package:nlme")
# change back to the original directory
setwd(initial.dir)
