class pageViewsLiftC(oddsMetricsC):
    def descBayesStats(self, na, ma, nb, mb):
        mua = fns.mu_bayes(na, ma)
        mub = fns.mu_bayes(nb, mb)
        var_mua = fns.var_mu_bayes(na, ma)
        var_mub = fns.var_mu_bayes(nb, mb)
        oddsa = self.fn(mua)
        oddsb = self.fn(mub)
        mean = (oddsa/oddsb - 1) + (var_mua/(oddsb * (1- mua)**3) +
                                  var_mub * oddsa/mub**3)
        var_mean = var_mua/(oddsb**2 * (1 - mua)**4) + var_mub * oddsa**2/mub**4
        return mean, var_mean

    def make_curve(self, pAAr, mvalue): ## depends on metric instance
        # returns array of pB values corresponding to points on the metric curve
        pBAr = np.array([self.invfn(self.fn(pa) / (1. + mvalue) for pa in pAAr])
        return pBAr
    

pageViewsLiftM = pageViewsLiftC('PageViewsLift')

