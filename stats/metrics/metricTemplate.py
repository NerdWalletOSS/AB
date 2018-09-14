# Copy to another file, edit and paste into metricsClass.py
# These are the only functions that are metric dependent

class newmetricC(metricC): # or previously defined subClass
    def fn(self, x): # useful for different metrics
        return x

    def invfn(self, fx): # useful for different metrics
        return fx

    
    def descBayesStats(self, na, ma, nb, mb): ## depends on metric instance
        # Descriptive stats for the posterior Bayesian distribution
        # calculates the mean and var_mean for the two ind. dimensions
        # For the comparison metric
        # returns the approximate parameters mean(metric) and var(mean_metric)
        mua = fns.mu_bayes(na, ma)
        mub = fns.mu_bayes(nb, mb)
        var_mua = fns.var_mu_bayes(na, ma)
        var_mub = fns.var_mu_bayes(nb, mb)

        ## depends on metric instance
        mean = mua - mub 
        var_mean = var_mua + var_mub

        return mean, var_mean


    def make_curve(self, pAAr, mvalue): ## depends on metric instance
        # returns array of pB values corresponding to points on the metric curve
        pBAr = np.array([max(0.0, p) for p in [min(1.0, pb - mvalue)
                                               for pb in pAAr]])
        return pBAr


    def return_from_metric(self, mvAr): # depends on metric
        ## min return as a function of metric threshold
        return mvAr

    def metric_from_return(self, ret): # depends on metric
        # inverse function of return_from_metric
        return ret
    
newMetricM = newmetricC('NewMetric', minimum, maximum)
