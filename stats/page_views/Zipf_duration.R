zipf_pv <- read.csv("./Output/_1week_000_pv_Zipf.csv")
attach(zipf_pv)
png(filename = "./Output/_1week_000_pv_hist.png")
hist(num_pv)
dev.off()

png(filename = "./Output/_1week_000_pv_density.png")
plot(density(num_pv))
dev.off()

png(filename = "./Output/_1week_000_pv_density_loglog.png")
plot(density(num_pv), log = "xy")
dev.off()

png(filename = "./Output/_1week_000_pv_density_logy.png")
plot(density(num_pv), log = "y")
dev.off()

png(filename = "./Output/_1week_000_pv_density_logx.png")
plot(density(num_pv), log = "x")
dev.off()

png(filename = "./Output/_1week_000_Zipf_pv.png")
plot(rank_of_user, num_pv, type = "l", main = "Zipf(page_views per user)", log = "xy")
dev.off()

detach()


pv_counts <- read.csv("./Output/_1week_000_pv_counts.csv")
attach(pv_counts)
png(filename = "./Output/_1week_000_pv_numuuid.png")
plot(pv, num_uuid, type = "l", main = "Log(Num(Users)) vs. PageViews", log = "y")
dev.off()

png(filename = "./Output/_1week_000_pv_cumuuid.png")
plot(pv, cum_uuid, type = "l", main = "Log(Cum(Users)) vs. PageViews", log = "y")
dev.off()

png(filename = "./Output/_1week_000_pv_transprob.png")
plot(pv, trans_prob, type = "l", main = "Trans. Prob. vs. PageViews")
dev.off()

png(filename = "./Output/_1week_000_pv_transprob_logx.png")
plot(pv, trans_prob, type = "l", main = "Trans. Prob. vs. log(PageViews)", log = "x")
dev.off()

detach()


zipf_duration <- read.csv("./Output/_1week_000_tdiff_Zipf.csv")
attach(zipf_duration)
png(filename = "./Output/_1week_000_duration_hist.png")
hist(t_diff)
dev.off()

png(filename = "./Output/_1week_000_duration_density.png")
plot(density(t_diff))
dev.off()

png(filename = "./Output/_1week_000_Zipf_duration_density_loglog.png")
plot(density(t_diff), log = "xy")
dev.off()

detach()


