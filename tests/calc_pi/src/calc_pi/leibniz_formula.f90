module leibniz_formula
    !$ use omp_lib
    use precision, only: dp, i8
    use math, only: PI
    implicit none

contains

    function calc_leibniz_formula( nmax ) result( numerical_pi )
        real(dp) numerical_pi, x
        integer(i8), intent(in) :: nmax
        integer(i8) i

#ifdef _INTLONG_ERROR
        if ( nmax > 2147483647_i8 ) then
            print *, 'nmax exceeds maximum of 4 byte integer'
            stop
        end if
#endif

        x = 1
        !$OMP parallel do reduction( +: x )
        do i = 1_i8, nmax
            x = x + (-1)**i / dble( 2*i + 1 )
        end do
        !$OMP end parallel do

        numerical_pi = x * 4

    end function calc_leibniz_formula


    subroutine test_leibniz_formula( nmax )
        integer(i8), intent(in) :: nmax
        real(dp) numerical_pi, error_rel

        numerical_pi = calc_leibniz_formula( nmax )
        error_rel = abs( numerical_pi - PI ) / PI
        print *, nmax, numerical_pi, error_rel

    end subroutine test_leibniz_formula

end module leibniz_formula
