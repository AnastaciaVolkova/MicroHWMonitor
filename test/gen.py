import sys
import numpy as np
from numpy import random


def main():
  m = np.iinfo(np.int64).max
  a = random.randint(0, m/100, (10, 64));
 
  with open("data.txt", "w") as fid:
    for i in range(a.shape[0]):
      for j in range(a.shape[1]):
        fid.write("{0} ".format(a[i][j]))
      fid.write("\n")
      

if __name__ == "__main__":
  main()
