from build.lib.calcpi4py import leibniz_formula
import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

for i in range(2, 11):
    nmax = 10**i
    print( f'\npython: {nmax = }' )
    leibniz_formula.test_leibniz_formula( nmax )
