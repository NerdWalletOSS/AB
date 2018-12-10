#!/usr/bin/env Rscript
# load the dataset or do needful
page_views <- read.csv("V1_data.csv")

attach(page_views)
png(filename = "V1_hist.png")
hist(pgviews, main = "Page Views Histogram V1")
dev.off()

png(filename = "V1_density.png")
plot(density(pgviews), log = "y", main = "log(density) V1")
dev.off()
detach(page_views)


# load the dataset or do needful
page_views <- read.csv("V2_data.csv")

attach(page_views)
png(filename = "V2_hist.png")
hist(pgviews, main = "Page Views Histogram V2")
dev.off()

png(filename = "V2_density.png")
plot(density(pgviews), log = "y", main = "log(density) V2")
dev.off()
detach(page_views)


# load the dataset or do needful
page_views <- read.csv("V3_data.csv")

attach(page_views)
png(filename = "V3_hist.png")
hist(pgviews, main = "Page Views Histogram V3")
dev.off()

png(filename = "V3_density.png")
plot(density(pgviews), log = "y", main = "log(density) V3")
dev.off()
detach(page_views)


# load the dataset or do needful
page_views_conf <- read.csv("V3_V2_conf.csv")

attach(page_views_conf)
png(filename = "V3_V2_conf.png")
plot(delta, confidence_level, type = "b", main = "Confidence vs. delta, V3_V2")
dev.off()

png(filename = "V3_V2_z.png")
plot(delta, z, type = "b", main = "z vs. delta, V3_V2")
dev.off()
detach(page_views_conf)

# load the dataset or do needful
page_views_conf <- read.csv("V3_V1_conf.csv")

attach(page_views_conf)
png(filename = "V3_V1_conf.png")
plot(delta, confidence_level, type = "b", main = "Confidence vs. delta, V3_V1")
dev.off()

png(filename = "V3_V1_z.png")
plot(delta, z, type = "b", main = "z vs. delta, V3_V1")
dev.off()
detach(page_views_conf)

# load the dataset or do needful
page_views_conf <- read.csv("V2_V1_conf.csv")

attach(page_views_conf)
png(filename = "V2_V1_conf.png")
plot(delta, confidence_level, type = "b", main = "Confidence vs. delta, V2_V1")
dev.off()

png(filename = "V2_V1_z.png")
plot(delta, z, type = "b", main = "z vs. delta, V2_V1")
dev.off()
detach(page_views_conf)

