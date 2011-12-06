
module m_fd_iso_3d_comm

  use mpi

  implicit none

  !!include 'mpif.h'

  integer              :: rank_R,rank_L,rank_F,rank_Bck,rank_T,rank_Btm

contains

!!$ Test code error 
!!$ ---------------
  subroutine test_success(comm,ierr,errmsg)

    integer,intent(in)            :: comm,ierr
    character(*),intent(in) :: errmsg

    if (ierr .ne. 0) then
       write(6,'(a)') trim(errmsg)
       call MPI_ABORT(comm,ierr,ierr)
    end if

  end subroutine test_success

!!$ Neighboring Processes Ranks
!!$ ----------------------------
  subroutine neighbors(comm_cart,rank)
 
    integer,intent(in)      :: comm_cart
    integer,intent(in)      :: rank

    integer                 :: tmp,ierr

    !-- Neighbor on the right side -
    tmp=rank
    call MPI_CART_SHIFT(comm_cart,0,1,tmp,rank_R,ierr)
    call test_success(comm_cart,ierr,'Error in neighbors : mpi_cart_shift (rank_R)')

    !-- Neighbor on the left side -
    tmp=rank
    call MPI_CART_SHIFT(comm_cart,0,-1,tmp,rank_L,ierr)
    call test_success(comm_cart,ierr,'Error in neighbors : mpi_cart_shift (rank_L)')

    !-- Neighbor in the front -
    tmp=rank
    call MPI_CART_SHIFT(comm_cart,1,-1,tmp,rank_F,ierr)
    call test_success(comm_cart,ierr,'Error in neighbors : mpi_cart_shift (rank_F)')

    !-- Neighbor at the back -
    tmp=rank
    call MPI_CART_SHIFT(comm_cart,1,1,tmp,rank_Bck,ierr)
    call test_success(comm_cart,ierr,'Error in neighbors : mpi_cart_shift (rank_Bck)')
 
    !-- Neighbor on the top -
    tmp=rank
    call MPI_CART_SHIFT(comm_cart,2,1,tmp,rank_T,ierr)
    call test_success(comm_cart,ierr,'Error in neighbors : mpi_cart_shift (rank_T)')
 
    !-- Neighbor on the bottom -
    tmp=rank
    call MPI_CART_SHIFT(comm_cart,2,-1,tmp,rank_Btm,ierr)
    call test_success(comm_cart,ierr,'Error in neighbors : mpi_cart_shift (rank_Btm)')

  end subroutine neighbors


!!$ Communication on boundaries 
!!$ ---------------------------
  subroutine fd_comm_boundary(comm_cart,rank,coords,     &
                              im,ip,jm,jp,km,kp,         &
                              lx,ly,lz,                  &
                              u                          )

    integer,intent(in)    :: comm_cart,rank
    integer,intent(in)    :: coords(3)
    integer,intent(in)    :: im,ip,jm,jp,km,kp
    integer,intent(in)    :: lx,ly,lz
    real,intent(inout)    :: u(im-lx:ip+lx,jm-ly:jp+ly,km-lz:kp+lz)

    integer               :: count,index,req_count,tag=0,flag,stat
    integer               :: ierr
    integer               :: status(MPI_STATUS_SIZE,12),request(12)

    !-- Intermediate buffers used for the communication	
    real                  :: neighbor_R_s(1:lx,1:jp-jm+1,1:kp-km+1) !send 
    real                  :: neighbor_R_r(1:lx,1:jp-jm+1,1:kp-km+1) !receive
    real                  :: neighbor_L_s(1:lx,1:jp-jm+1,1:kp-km+1) 
    real                  :: neighbor_L_r(1:lx,1:jp-jm+1,1:kp-km+1) 
    real                  :: neighbor_F_s(1:ip-im+1,1:ly,1:kp-km+1)
    real                  :: neighbor_F_r(1:ip-im+1,1:ly,1:kp-km+1)
    real                  :: neighbor_Bck_s(1:ip-im+1,1:ly,1:kp-km+1)
    real                  :: neighbor_Bck_r(1:ip-im+1,1:ly,1:kp-km+1)
    real                  :: neighbor_T_s(1:ip-im+1,1:jp-jm+1,1:lz)
    real                  :: neighbor_T_r(1:ip-im+1,1:jp-jm+1,1:lz)
    real                  :: neighbor_Btm_s(1:ip-im+1,1:jp-jm+1,1:lz)
    real                  :: neighbor_Btm_r(1:ip-im+1,1:jp-jm+1,1:lz)

    !-- Compute the ranks of the neighboring processes
    call neighbors(comm_cart,rank)

    req_count=0

    !-- Communication with the neighbor on the right side
    count = lx*(jp-jm+1)*(kp-km+1)
    neighbor_R_s=u(ip-lx+1:ip,jm:jp,km:kp)

    if (rank_R .ne. MPI_PROC_NULL) then 
      call MPI_IRECV(neighbor_R_r,count,MPI_REAL,rank_R,tag,comm_cart,request(1),ierr)
      call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_irecv (rank_R)')

      call MPI_ISEND(neighbor_R_s,count,MPI_REAL,rank_R,tag,comm_cart,request(2),ierr)
      call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_isend (rank_R)')

      req_count=req_count+2

    else
      request(1)=MPI_REQUEST_NULL
      request(2)=MPI_REQUEST_NULL
    endif

    !-- Communication with the neighbor on the left side
    neighbor_L_s=u(im:im+lx-1,jm:jp,km:kp)

    if (rank_L .ne. MPI_PROC_NULL) then 
      call MPI_IRECV(neighbor_L_r,count,MPI_REAL,rank_L,tag,comm_cart,request(3),ierr)
      call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_irecv (rank_L)')

      call MPI_ISEND(neighbor_L_s,count,MPI_REAL,rank_L,tag,comm_cart,request(4),ierr)
      call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_isend (rank_L)')

      req_count=req_count+2

    else
      request(3)=MPI_REQUEST_NULL
      request(4)=MPI_REQUEST_NULL
    end if
    
    !-- Communication with the neighbor in the front
    count = ly*(ip-im+1)*(kp-km+1)
    neighbor_F_s=u(im:ip,jm:jm+ly-1,km:kp)

    if (rank_F .ne. MPI_PROC_NULL) then 
      call MPI_IRECV(neighbor_F_r,count,MPI_REAL,rank_F,tag,comm_cart,request(5),ierr)
      call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_irecv (rank_F)')

      call MPI_ISEND(neighbor_F_s,count,MPI_REAL,rank_F,tag,comm_cart,request(6),ierr)
      call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_isend (rank_F)')

      req_count=req_count+2

    else
      request(5)=MPI_REQUEST_NULL
      request(6)=MPI_REQUEST_NULL
    endif

    !-- Communication with the neighbor on the back
    neighbor_Bck_s=u(im:ip,jp-ly+1:jp,km:kp)

    if (rank_Bck .ne. MPI_PROC_NULL) then 
       call MPI_IRECV(neighbor_Bck_r,count,MPI_REAL,rank_Bck,tag,comm_cart,request(7),ierr)
       call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_irecv (rank_Bck)')

       call MPI_ISEND(neighbor_Bck_s,count,MPI_REAL,rank_Bck,tag,comm_cart,request(8),ierr)
       call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_isend (rank_Bck)')

       req_count=req_count+2

    else
      request(7)=MPI_REQUEST_NULL
      request(8)=MPI_REQUEST_NULL
    end if
    
    !-- Communication with the neighbor on the top
    count = lz*(ip-im+1)*(jp-jm+1)
    neighbor_T_s=u(im:ip,jm:jp,kp-lz+1:kp)

    if (rank_T .ne. MPI_PROC_NULL) then 
       call MPI_IRECV(neighbor_T_r,count,MPI_REAL,rank_T,tag,comm_cart,request(9),ierr)
       call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_irecv (rank_T)')

       call MPI_ISEND(neighbor_T_s,count,MPI_REAL,rank_T,tag,comm_cart,request(10),ierr)
       call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_isend (rank_T)')

       req_count=req_count+2
    else
      request(9)=MPI_REQUEST_NULL
      request(10)=MPI_REQUEST_NULL
    endif

    !-- Communication with the neighbor on the bottom
    neighbor_Btm_s=u(im:ip,jm:jp,km:km+lz-1)

    if (rank_Btm .ne. MPI_PROC_NULL) then 
       call MPI_IRECV(neighbor_Btm_r,count,MPI_REAL,rank_Btm,tag,comm_cart,request(11),ierr)
       call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_irecv (rank_Btm)')

       call MPI_ISEND(neighbor_Btm_s,count,MPI_REAL,rank_Btm,tag,comm_cart,request(12),ierr)
       call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_isend (rank_Btm)')

       req_count=req_count+2
    else
      request(11)=MPI_REQUEST_NULL
      request(12)=MPI_REQUEST_NULL
    end if

    do while(req_count .ne. 0) 

      call MPI_WAITANY(12,request,index,status,ierr)
      call test_success(comm_cart,ierr,'Error in fd_comm_boundary: mpi_waitany')
      
      select case(index) 
        case(1)
          u(ip+1:ip+lx,jm:jp,km:kp)=neighbor_R_r
        case(2)

        case(3)
          u(im-lx:im-1,jm:jp,km:kp)=neighbor_L_r
        case(4)

        case(5)
          u(im:ip,jm-ly:jm-1,km:kp)=neighbor_F_r
        case(6)

        case(7)
          u(im:ip,jp+1:jp+ly,km:kp)=neighbor_Bck_r
        case(8)

        case(9)
          u(im:ip,jm:jp,kp+1:kp+lz)=neighbor_T_r
        case(10)

        case(11)
          u(im:ip,jm:jp,km-lz:km-1)=neighbor_Btm_r
        case(12)
      endselect

      req_count=req_count-1

    enddo

  end subroutine fd_comm_boundary

end module m_fd_iso_3d_comm
