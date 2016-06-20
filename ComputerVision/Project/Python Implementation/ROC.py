import matplotlib.pyplot as plt
import sys
import os

plt.figure()
tn_rate=[i/100.0 for i in range(0,105,5)]
print(len(tn_rate))
tp_rate=[0,0.16,0.26,0.34,0.42, 0.48, 0.54, 0.58, 0.62, 0.66,
         0.70, 0.74, 0.77, 0.80, 0.82, .84, 0.85, 0.86, 0.87, 0.88, 0.91]
plt.plot(tn_rate, tp_rate, label='GAB+LBP+LDA+KNN')
plt.xlabel('False Positive Rate(Specificity)')
plt.ylabel('True Positive Rate(Sensitivity)')
plt.legend(bbox_to_anchor=(1, 1), loc=1, borderaxespad=0.)
plt.show()
