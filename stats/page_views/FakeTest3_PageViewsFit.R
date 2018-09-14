#!/usr/bin/env Rscript

page_views_V1 <- read.csv("./Output/FakeTest3_V1_fit.csv")
attach(page_views_V1)
dflin <- data.frame(v=v, lU=log(U))
linm <- lm(lU~v, data = dflin)
U0_start <- exp(summary(linm)$coefficients[1,1])
pv_start <- 1/(exp(-summary(linm)$coefficients[2,1]) - 1)
dfnonlin <- data.frame(v=v, U=U, w = 1/U)
expdecm <- nls(U ~ U0*(1+1/pv)^(-v), data = dfnonlin, weights = w, start = list(U0 = U0_start, pv = pv_start))
write.csv(summary(expdecm)$coefficients, "./Output/_summaryFakeTest3_V1.csv")
detach(page_views_V1)


page_views_V2 <- read.csv("./Output/FakeTest3_V2_fit.csv")
attach(page_views_V2)
dflin <- data.frame(v=v, lU=log(U))
linm <- lm(lU~v, data = dflin)
U0_start <- exp(summary(linm)$coefficients[1,1])
pv_start <- 1/(exp(-summary(linm)$coefficients[2,1]) - 1)
dfnonlin <- data.frame(v=v, U=U, w = 1/U)
expdecm <- nls(U ~ U0*(1+1/pv)^(-v), data = dfnonlin, weights = w, start = list(U0 = U0_start, pv = pv_start))
write.csv(summary(expdecm)$coefficients, "./Output/_summaryFakeTest3_V2.csv")
detach(page_views_V2)


page_views_V3 <- read.csv("./Output/FakeTest3_V3_fit.csv")
attach(page_views_V3)
dflin <- data.frame(v=v, lU=log(U))
linm <- lm(lU~v, data = dflin)
U0_start <- exp(summary(linm)$coefficients[1,1])
pv_start <- 1/(exp(-summary(linm)$coefficients[2,1]) - 1)
dfnonlin <- data.frame(v=v, U=U, w = 1/U)
expdecm <- nls(U ~ U0*(1+1/pv)^(-v), data = dfnonlin, weights = w, start = list(U0 = U0_start, pv = pv_start))
write.csv(summary(expdecm)$coefficients, "./Output/_summaryFakeTest3_V3.csv")
detach(page_views_V3)

setwd(initial.dir)
