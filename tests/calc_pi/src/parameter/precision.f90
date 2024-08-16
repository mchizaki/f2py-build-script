module precision
    implicit none
    integer, parameter ::  i1 = selected_int_kind(2)   !  1 byte integer
    integer, parameter ::  i2 = selected_int_kind(4)   !  2 byte integer
    integer, parameter ::  i4 = selected_int_kind(8)   !  4 byte integer
    integer, parameter ::  i8 = selected_int_kind(18)  !  8 byte integer
    integer, parameter :: i16 = selected_int_kind(37)  ! 16 byte integer
    integer, parameter ::  sp = selected_real_kind(6)  ! single    precision
    integer, parameter ::  dp = selected_real_kind(15) ! double    precision
    integer, parameter ::  qp = selected_real_kind(30) ! quadruple precision
end module precision
