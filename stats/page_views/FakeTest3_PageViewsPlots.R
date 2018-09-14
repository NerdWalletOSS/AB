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
page_views_V1 <- read.csv("./Output/FakeTest3_V1.csv")

png(filename = "./Output/FakeTest3_V1_hist.png")
hist(page_views_V1$pgviews)
dev.off()

png(filename = "./Output/FakeTest3_V1_density.png")
plot(density(page_views_V1$pgviews), log = "y")
dev.off()

# close the output file
# sink()

# sink("FakeData_V2.out")
# load the dataset or do needful
page_views_V2 <- read.csv("./Output/FakeTest3_V2.csv")

png(filename = "./Output/FakeTest3_V2_hist.png")
hist(page_views_V2$pgviews)
dev.off()

png(filename = "./Output/FakeTest3_V2_density.png")
plot(density(page_views_V2$pgviews), log = "y")
dev.off()

# close the output file
# sink()

# sink("FakeData_V3.out")
# load the dataset or do needful
page_views_V3 <- read.csv("./Output/FakeTest3_V3.csv")

png(filename = "./Output/FakeTest3_V3_hist.png")
hist(page_views_V3$pgviews)
dev.off()

png(filename = "./Output/FakeTest3_V3_density.png")
plot(density(page_views_V3$pgviews), log = "y")
dev.off()

# close the output file
# sink()

# sink("FakeData_$variant.out")
# load the dataset or do needful
page_views_V1_V2_conf <- read.csv("./Output/FakeTest3_V1_V2_conf.csv")

attach(page_views_V1_V2_conf)
png(filename = "./Output/FakeTest3_V1_V2_conf.png")
plot(delta, confidence_level, type = "b", main = "Confidence vs. delta, FakeTest3_V1_V2")
dev.off()

png(filename = "./Output/FakeTest3_V1_V2_z.png")
plot(delta, z, type = "b", main = "z vs. delta, FakeTest3_V1_V2")
dev.off()

# close the output file
# sink()

# sink("FakeData_$variant.out")
# load the dataset or do needful
page_views_V1_V3_conf <- read.csv("./Output/FakeTest3_V1_V3_conf.csv")

attach(page_views_V1_V3_conf)
png(filename = "./Output/FakeTest3_V1_V3_conf.png")
plot(delta, confidence_level, type = "b", main = "Confidence vs. delta, FakeTest3_V1_V3")
dev.off()

png(filename = "./Output/FakeTest3_V1_V3_z.png")
plot(delta, z, type = "b", main = "z vs. delta, FakeTest3_V1_V3")
dev.off()

# close the output file
# sink()

# sink("FakeData_$variant.out")
# load the dataset or do needful
page_views_V2_V3_conf <- read.csv("./Output/FakeTest3_V2_V3_conf.csv")

attach(page_views_V2_V3_conf)
png(filename = "./Output/FakeTest3_V2_V3_conf.png")
plot(delta, confidence_level, type = "b", main = "Confidence vs. delta, FakeTest3_V2_V3")
dev.off()

png(filename = "./Output/FakeTest3_V2_V3_z.png")
plot(delta, z, type = "b", main = "z vs. delta, FakeTest3_V2_V3")
dev.off()

# close the output file
# sink()

# unload the libraries
# detach("package:nlme")
# change back to the original directory
setwd(initial.dir)
