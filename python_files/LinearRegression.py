import numpy as np
from sklearn.linear_model import LinearRegression

print("One variable linear regression")
X = np.array([[1.], [2.], [3.], [4.], [5.]])
y = [1.5, 1.7, 3.2, 4.5, 5.19]

reg = LinearRegression().fit(X, y)
print("reg score:     %f" % reg.score(X, y))
print("reg coef:      %f" % reg.coef_)
print("reg intercept: %f" % reg.intercept_)
print(reg.predict(X))
print(reg.score(X, y))

# Two variable stuff

print("")

print("Two variable linear regression")
X = np.array([[0.9, 1.3], [2.3, 2.9], [3.1, 3.7], [3.7, 4.4], [5.4, 5.8]])
y = [5.5, 10.9, 14.0, 22.3, 27.1]

reg = LinearRegression().fit(X, y)
print("reg score:     %f" % reg.score(X, y))
print("reg coef:      %s" % reg.coef_)
print("reg intercept: %s" % reg.intercept_)
print(reg.predict(X))
print(reg.score(X, y))
