import matplotlib as mpl
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import numpy as np
import os
import sys

from math import log, sqrt
from scipy.stats import beta, norm

sys.path.insert(0, os.path.abspath('..'))
from lib import utilities as u

mpl.rcParams['xtick.direction'] = 'out'
mpl.rcParams['ytick.direction'] = 'out'
x = np.linspace(0,1,101)
y = np.linspace(0,1,101)
xodds = np.linspace(0.01, 0.99, 99)
yodds = map(lambda i: i/(2-i), xodds) # odds are half
X, Y = np.meshgrid(x,y)
ONES = np.ones((101,101))

NA, MA, NB, MB = 12, 10, 12, 7
NAME = 'Contours of Likelihood function ' + str(NA) + '_'  + str(MA) + '_' + str(NA) + '_' + str(MB)

NAME_B = "2 Dimensional Probability Space (pA,pB)"
FNAME_B = "2_Dimensional_Probability_Space_pA_pB"
D_Bayes = beta.pdf(X, MA + 1, NA - MA + 1) * beta.pdf(Y, MB + 1, NB - MB + 1)
plt.figure(figsize = (10,10))
plt.axes().set_aspect('equal')
plt.tick_params(labelsize = 20)
CS = plt.contour(X, Y, D_Bayes)
plt.clabel(CS, inline = 1, fontsize = 16)
plt.title(NAME_B, fontsize = 28)
plt.xlabel("p_A", fontsize = 22)
plt.ylabel("p_B", fontsize = 22)
u.save(FNAME_B+'1')

NAME_BL = "2 Dimensional Probability Space (pA,pB)"
D_Bayes = beta.pdf(X, MA + 1, NA - MA + 1) * beta.pdf(Y, MB + 1, NB - MB + 1)
plt.figure(figsize = (10,10))
plt.axes().set_aspect('equal')
plt.tick_params(labelsize = 20)
CS = plt.contour(X, Y, D_Bayes)
plt.clabel(CS, inline = 1, fontsize = 16)
plt.plot([0.25,1], [0,0.75], 'r-', label = 'Difference = 0.25')
plt.plot([0,1], [0,0.5], 'y-', label = 'Lift = 100%')
plt.plot(xodds, yodds, 'g-', label = 'Page Views Lift  = 100%')
plt.title(NAME_BL, fontsize = 28)
plt.xlabel("p_A", fontsize = 22)
plt.ylabel("p_B", fontsize = 22)
plt.legend(loc = 'upper left', fontsize = 24)
u.save(FNAME_B+'2')

NAME_F = 'Freq_' + NAME
MUFA = 1.*MA/NA
SDFA = sqrt(MUFA * (1 - MUFA)/NA)
MUFB = 1.*MB/NB
SDFB = sqrt(MUFB * (1 - MUFB)/NB)
normA = norm(loc = MUFA, scale = SDFA)
normB = norm(loc = MUFB, scale = SDFB)
D_Norm = normA.pdf(X) * normB.pdf(Y)
plt.figure()
plt.axes().set_aspect('equal')
CS = plt.contour(X, Y, D_Norm)
plt.clabel(CS, inline = 1, fontsize = 10)
plt.title(NAME_F)
plt.xlabel("p_A")
plt.ylabel("p_B")
u.save(NAME_F)

NAME_Diff = 'Diff_' + NAME
plt.figure()
plt.axes().set_aspect('equal')
CS = plt.contour(X, Y, (D_Norm - D_Bayes))
plt.clabel(CS, inline = 1, fontsize = 10)
plt.title(NAME_Diff)
plt.xlabel("p_A")
plt.ylabel("p_B")
u.save(NAME_Diff)

NAME_L = 'Lines_' + NAME
plt.figure()
plt.axes().set_aspect('equal')
plt.plot([0.25,1], [0,0.75], 'r-', label = 'Difference = 0.25')
plt.plot([0,1], [0,0.5], 'y-', label = 'Lift = 1.0')
plt.plot(xodds, yodds, 'g-', label = 'OddsFactor = 2.0')
plt.title(NAME_L)
plt.xlabel("p_A")
plt.ylabel("p_B")
plt.legend(loc = 'upper left')
u.save(NAME_L)

NAME_Bl = 'Bayes line ' + NAME
FNAME_Bl = 'Bayes_line_' + NAME
D_Bayes = beta.pdf(X, MA + 1, NA - MA + 1) * beta.pdf(Y, MB + 1, NB - MB + 1)
plt.figure()
plt.axes().set_aspect('equal')
CS = plt.contour(X, Y, D_Bayes)
plt.clabel(CS, inline = 1, fontsize = 10)
plt.plot(xodds, yodds, 'g-', label = 'OddsFactor = 2.0')
plt.title(NAME_Bl)
plt.xlabel("p_A")
plt.ylabel("p_B")
plt.legend(loc = 'upper left')
u.save(NAME_Bl+'2')

