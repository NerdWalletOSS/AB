class pageViewsDiffC(oddsMetricsC): # or previously defined subClass
    
    def descBayesStats(self, na, ma, nb, mb): ## depends on metric instance
        # Descriptive stats for the posterior Bayesian distribution
        # calculates the mean and var_mean for the two ind. dimensions
        # For the comparison metric
        # returns the approximate parameters mean(metric) and var(mean_metric)
        mua = fns.mu_bayes(na, ma)
        mub = fns.mu_bayes(nb, mb)
        var_mua = fns.var_mu_bayes(na, ma)
        var_mub = fns.var_mu_bayes(nb, mb)
        oddsa = self.fn(mua)
        oddsb = self.fn(mub)
        mean = (oddsa - oddsb) + (var_mua/(1 - mua)**3 + var_mub/(1 - mub)**3)
        var_mean = var_mua/(1 - mua)**4 + var_mub/(1 - mub)**4
        return mean, var_mean


    def make_curve(self, pAAr, mvalue): ## depends on metric instance
        # returns array of pB values corresponding to points on the metric curve
        pBAr = np.array([self.invfn(self.fn(pa) - mvalue) for pa in pAAr])
        return pBAr


pageViewsDiffM = pageViewsDiffC('PageViewsDiff', minimum, maximum)
