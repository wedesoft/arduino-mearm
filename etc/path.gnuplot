set multiplot
set xrange [0:4]
pi2 = 2*pi
cap(x) = 1-cos(pi2*x)
up(x) = x-sin(pi2*x)/pi2
dent(x) = 0.5*x**2+(cos(pi2*x)-1)/pi2**2
a(x) = x < 1 ? 0.5*cap(x) : x > 3 ? -0.5*cap(x-3) : 0
v(x) = x < 1 ? 0.5*up(x) : x > 3 ? 0.5*(1-up(x-3)) : 0.5
p(x) = x < 1 ? 0.5*dent(x) : x > 3 ? 0.5*x-0.25-0.5*dent(x-3): 0.5*x-0.25 
plot a(x), v(x), p(x)
pause -1
