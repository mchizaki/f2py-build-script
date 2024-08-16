program calc_pi_test
    use precision, only: i8
    use leibniz_formula, only: test_leibniz_formula
    implicit none

    integer(i8) i, imax, nmax

#ifdef _INTLONG_ERROR
    imax = 9_i8
#else
    imax = 10_i8
#endif

    do i = 1_i8, imax
        nmax = 10_i8 ** i
        call test_leibniz_formula( nmax = nmax )
    end do

end program calc_pi_test