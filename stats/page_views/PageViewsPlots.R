#!/usr/bin/env Rscript
# load the dataset or do needful
page_views <- read.csv("./Output/FakeTest/C_data.csv")

attach(page_views)
png(filename = "./Output/FakeTest/C_hist.png")
hist(pgviews)
dev.off()

png(filename = "./Output/FakeTest/C_density.png")
plot(density(pgviews), log = "y")
dev.off()
detach(page_views)


# load the dataset or do needful
page_views <- read.csv("./Output/FakeTest/T_data.csv")

attach(page_views)
png(filename = "./Output/FakeTest/T_hist.png")
hist(pgviews)
dev.off()

png(filename = "./Output/FakeTest/T_density.png")
plot(density(pgviews), log = "y")
dev.off()
detach(page_views)


# load the dataset or do needful
page_views_conf <- read.csv("./Output/FakeTest/C_T_conf.csv")

attach(page_views_conf)
png(filename = "./Output/FakeTest/C_T_conf.png")
plot(delta, confidence_level, type = "b", main = "Confidence vs. delta, C_T")
dev.off()

png(filename = "./Output/FakeTest/C_T_z.png")
plot(delta, z, type = "b", main = "z vs. delta, C_T")
dev.off()
detach(page_views_conf)

