import numpy as np
import pandas as pd

N = 40
#M = np.random.randint(0,2,(n,n))
#symm = M@M.T
# test for symmetry
#print(symm)
#print(symm == symm.T)

# def random_symmetric_matrix(n):
#     _R = np.random_integers.uniform(-1,1,int(n*(n-1)/2))
#     P = np.zeros((n,n))
#     P[np.triu_indices(n, 1)] = _R
#     P[np.tril_indices(n, -1)] = P.T[np.tril_indices(n, -1)]
#     return P

# print(random_symmetric_matrix(10))


from random import randint
import random

sparse = np.zeros((N,N))
for _ in range(N + int(N/2)):
    (i,j) = (randint(0,N-1),randint(0,N-1))
    x = 1
    sparse[i][j] = x
    sparse[j][i] = x

#print(sparse)
mat = np.matrix(sparse)
with open('input.txt', 'w') as f:
    for line in mat:
        np.savetxt(f, line, fmt='%.d')

# import xlrd
# df = pd.read_excel("source.xlsx", sheet_name=0)
# df = df.dropna()
# df.drop(df.columns[[0]], axis=1, inplace=True)
# df.drop(df.columns[[0]], axis=1, inplace=True)

# a = df.to_numpy()
# a = np.matrix(a)
# print(a)
# with open('input2.txt', 'w') as f:
#     for line in a:
#         np.savetxt(f, line, fmt='%.d')
