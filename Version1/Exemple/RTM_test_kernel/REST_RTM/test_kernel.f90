module m_fd_iso_3d_lx4_opt

  implicit none

  private

  public :: fd_iso_3d_lx4_opt
  public :: fd_iso_3d_lx4_get_opt_bloc

  integer,parameter :: lx=4
  integer,parameter :: ly=4
  integer,parameter :: lz=4


contains

  include 'crop.f90'

  !DEC$ ATTRIBUTES FORCEINLINE :: fd_iso_3d_lx4_inner
  !DEC$ ATTRIBUTES FORCEINLINE :: fd_iso_3d_lx4_damping
  

  !===========================================================================
  subroutine fd_iso_3d_lx4_get_opt_bloc(blocx,blocy,dim_x,haspml,      &
       l2_n_shared_core,l2_cache_size,l3_n_shared_core,l3_cache_size)
    !
    integer,intent(inout) :: blocx
    integer,intent(inout) :: blocy
    integer,intent(in)    :: dim_x
    logical,intent(in)    :: haspml
    integer,intent(in)    :: l2_n_shared_core,l2_cache_size
    integer,intent(in)    :: l3_n_shared_core,l3_cache_size
    
    integer               :: available_cache,base_bloc,line_size,div
    integer               :: n_array,n_lap
    real                  :: val,cache_size
    

    if (l3_cache_size.ne.0) then
       cache_size=(l3_cache_size-65536)/l3_n_shared_core
    else
       cache_size=(l2_cache_size-65536)/l2_n_shared_core
    endif

    n_lap=3
    if (haspml) then
       n_array=5
    else
       n_array=3
    endif

    available_cache=cache_size/kind(val)

    do div=1,dim_x
       blocx=(dim_x+div-1)/div

       line_size=n_array*(blocx+2*lx+1)*(2*lz+1)
       base_bloc=n_lap*blocx

       blocy=(available_cache-base_bloc)/line_size-(2*ly+1)

       if (blocy.ge.5) exit
    enddo

  end subroutine fd_iso_3d_lx4_get_opt_bloc



  !===========================================================================
  subroutine fd_iso_3d_lx4_inner(cxmin,cxmax,j,k,                      &
       coefx,coefy,coefz,                                              &
       xmin,xmax,ymin,ymax,zmin,zmax,                                  &
       u,v,roc2,coef0,lapx,lapy,lapz)
    !
    integer					  :: valfct 
    integer,intent(in) :: cxmin,cxmax,j,k
    real,intent(in)    :: coefx(-lx:lx)
    real,intent(in)    :: coefy(-ly:ly)
    real,intent(in)    :: coefz(-lz:lz)
    integer,intent(in) :: xmin,xmax,ymin,ymax,zmin,zmax
    real,intent(inout) :: u(xmin-lx:xmax+lx,ymin-ly:ymax+ly,zmin-lz:zmax+lz)
    real,intent(in)    :: v(xmin-lx:xmax+lx,ymin-ly:ymax+ly,zmin-lz:zmax+lz)
    real,intent(in)    :: roc2(xmin:xmax,ymin:ymax,zmin:zmax)
    real,intent(in)    :: coef0
    real,intent(inout) :: lapx(cxmin:cxmax)
    real,intent(inout) :: lapy(cxmin:cxmax)
    real,intent(inout) :: lapz(cxmin:cxmax)

    integer            :: i


    do i=cxmin,cxmax
       lapx(i)=coefx(1)*(v(i+1,j,k)+v(i-1,j,k))                        &
            +coefx(2)*(v(i+2,j,k)+v(i-2,j,k))                          &
            +coefx(3)*(v(i+3,j,k)+v(i-3,j,k))                          &
            +coefx(4)*(v(i+4,j,k)+v(i-4,j,k))             
    enddo
    
    do i=cxmin,cxmax
       lapy(i)=coefy(1)*(v(i,j+1,k)+v(i,j-1,k))                        &
            +coefy(2)*(v(i,j+2,k)+v(i,j-2,k))                          &
            +coefy(3)*(v(i,j+3,k)+v(i,j-3,k))                          &
            +coefy(4)*(v(i,j+4,k)+v(i,j-4,k))             
    enddo
    
    do i=cxmin,cxmax
       lapz(i)=coefz(1)*(v(i,j,k+1)+v(i,j,k-1))                        &
            +coefz(2)*(v(i,j,k+2)+v(i,j,k-2))                          &
            +coefz(3)*(v(i,j,k+3)+v(i,j,k-3))                          &
            +coefz(4)*(v(i,j,k+4)+v(i,j,k-4))
    enddo
    
    do i=cxmin,cxmax
       !! Update the wavefield.
       u(i,j,k)=2.*v(i,j,k)-u(i,j,k)+roc2(i,j,k)                       &
            *(coef0*v(i,j,k)+lapx(i)+lapy(i)+lapz(i))
    enddo

  end subroutine fd_iso_3d_lx4_inner



  !===========================================================================
  subroutine fd_iso_3d_lx4_damping(cxmin,cxmax,cymin,cymax,k,          &
       hdx_2,hdy_2,hdz_2,coefx,coefy,coefz,                            &
       xmin,xmax,ymin,ymax,zmin,zmax,                                  &
       u,v,roc2,phi,eta,coef0,lapx,lapy,lapz)
    !
    integer					  :: valfct 
    integer,intent(in) :: cxmin,cxmax,cymin,cymax,k
    real,intent(in)    :: hdx_2,hdy_2,hdz_2
    real,intent(in)    :: coefx(-lx:lx)
    real,intent(in)    :: coefy(-ly:ly)
    real,intent(in)    :: coefz(-lz:lz)
    integer,intent(in) :: xmin,xmax,ymin,ymax,zmin,zmax
    real,intent(inout) :: phi(xmin:xmax,ymin:ymax,zmin:zmax)
    real,intent(inout) :: u(xmin-lx:xmax+lx,ymin-ly:ymax+ly,zmin-lz:zmax+lz)
    real,intent(in)    :: v(xmin-lx:xmax+lx,ymin-ly:ymax+ly,zmin-lz:zmax+lz)
    real,intent(in)    :: roc2(xmin:xmax,ymin:ymax,zmin:zmax)
    real,intent(in)    :: eta(xmin-1:xmax+1,ymin-1:ymax+1,zmin-1:zmax+1)
    real,intent(in)    :: coef0
    real,intent(inout) :: lapx(cxmin:cxmax)
    real,intent(inout) :: lapy(cxmin:cxmax)
    real,intent(inout) :: lapz(cxmin:cxmax)

    integer            :: i,j

    do j=cymin,cymax
       do i=cxmin,cxmax
          lapx(i)=coefx(1)*(v(i+1,j,k)+v(i-1,j,k))                     &
               +coefx(2)*(v(i+2,j,k)+v(i-2,j,k))                       &
               +coefx(3)*(v(i+3,j,k)+v(i-3,j,k))                       &
               +coefx(4)*(v(i+4,j,k)+v(i-4,j,k))             
       enddo

       do i=cxmin,cxmax
          lapy(i)=coefy(1)*(v(i,j+1,k)+v(i,j-1,k))                     &
               +coefy(2)*(v(i,j+2,k)+v(i,j-2,k))                       &
               +coefy(3)*(v(i,j+3,k)+v(i,j-3,k))                       &
               +coefy(4)*(v(i,j+4,k)+v(i,j-4,k))             
       enddo

       do i=cxmin,cxmax
          lapz(i)=coefz(1)*(v(i,j,k+1)+v(i,j,k-1))                     &
               +coefz(2)*(v(i,j,k+2)+v(i,j,k-2))                       &
               +coefz(3)*(v(i,j,k+3)+v(i,j,k-3))                       &
               +coefz(4)*(v(i,j,k+4)+v(i,j,k-4))
       enddo

       do i=cxmin,cxmax
          !! Update the wavefield.
          u(i,j,k)=((2.-eta(i,j,k)*eta(i,j,k)                          &
               +2.*eta(i,j,k))*v(i,j,k)                                &
               -u(i,j,k)                                               &
               +roc2(i,j,k)*                                           &
               ((coef0*v(i,j,k)+lapx(i)+lapy(i)+lapz(i))               &
               +phi(i,j,k)))/(1.+2.*eta(i,j,k))
       enddo

       do i=cxmin,cxmax
          !! Update the PML function.
          phi(i,j,k)=(phi(i,j,k)-                                      &
               ((eta(i+1,j,k)-eta(i-1,j,k))                            &
               *(v(i+1,j,k)-v(i-1,j,k))*hdx_2                          &
               +(eta(i,j+1,k)-eta(i,j-1,k))                            &
               *(v(i,j+1,k)-v(i,j-1,k))*hdy_2                          &
               +(eta(i,j,k+1)-eta(i,j,k-1))                            &
               *(v(i,j,k+1)-v(i,j,k-1))*hdz_2                          &
               ))/(1.+eta(i,j,k))
       enddo
    enddo

  end subroutine fd_iso_3d_lx4_damping



  !===========================================================================
  subroutine fd_iso_3d_lx4_opt(cxmin,cxmax,cymin,cymax,czmin,czmax,    &
       x2,x3,x4,x5,y2,y3,y4,y5,z2,z3,z4,z5,                            &
       hdx_2,hdy_2,hdz_2,coefx,coefy,coefz,                            &
       xmin,xmax,ymin,ymax,zmin,zmax,                                  &
       u,v,roc2,phi,eta,bx,by)
    !
    integer,intent(in) :: cxmin,cxmax,cymin,cymax,czmin,czmax
    integer,intent(in) :: x2,x3,x4,x5,y2,y3,y4,y5,z2,z3,z4,z5
    real,intent(in)    :: hdx_2,hdy_2,hdz_2
    real,intent(in)    :: coefx(-lx:lx)
    real,intent(in)    :: coefy(-ly:ly)
    real,intent(in)    :: coefz(-lz:lz)
    integer,intent(in) :: xmin,xmax,ymin,ymax,zmin,zmax
    real,intent(inout) :: phi(xmin:xmax,ymin:ymax,zmin:zmax)
    real,intent(inout) :: u(xmin-lx:xmax+lx,ymin-ly:ymax+ly,zmin-lz:zmax+lz)
    real,intent(in)    :: v(xmin-lx:xmax+lx,ymin-ly:ymax+ly,zmin-lz:zmax+lz)
    real,intent(in)    :: roc2(xmin:xmax,ymin:ymax,zmin:zmax)
    real,intent(in)    :: eta(xmin-1:xmax+1,ymin-1:ymax+1,zmin-1:zmax+1)
    integer,intent(in) :: bx,by

    real               :: lapx(cxmin:cxmax)
    real               :: lapy(cxmin:cxmax)
    real               :: lapz(cxmin:cxmax)

    integer            :: cx1,cx2,cx3,cx4,cx5,cx6
    integer            :: cy1,cy2,cy3,cy4,cy5,cy6
    integer            :: cz1,cz2,cz3,cz4,cz5,cz6
    integer            :: bcx1,bcx2,bcx3,bcx4,bcx5,bcx6

    integer            :: j,k
    real               :: coef0

    integer            :: imin,imax,jmin,jmax
    integer            :: i2,j2
    integer            :: blocx,blocy


    !! Sub-domain crop.
    !! ----------------
    if (cxmin<0.or.cymin<0.or.czmin<0) return

    !! X-axis.
    call crop(cx1,cx2,cx3,cx4,cx5,cx6,                                 &
         cxmin,cxmax,cxmin,x2,x3,x4,x5,cxmax)

    !! Y-axis.
    call crop(cy1,cy2,cy3,cy4,cy5,cy6,                                 &
         cymin,cymax,cymin,y2,y3,y4,y5,cymax)

    !! Z-axis.
    call crop(cz1,cz2,cz3,cz4,cz5,cz6,                                 &
         czmin,czmax,czmin,z2,z3,z4,z5,czmax)

    if ((bx.le.0).or.(by.le.0)) then
       blocx=cxmax-cxmin+1
       blocy=cymax-cymin+1
    else
       blocx=bx
       blocy=by
    endif


    coef0=coefx(0)+coefy(0)+coefz(0)


    !! FD 2-8.
    !! -------
    do j2=cymin,cymax,blocy
       jmin=j2
       jmax=jmin+blocy-1
       if (jmax>cymax) jmax=cymax
       
       do i2=cxmin,cxmax,blocx
          imin=i2
          imax=imin+blocx-1
          if (imax>cxmax) imax=cxmax

          !! Crop on X based on the X blocking factor.
          call crop(bcx1,bcx2,bcx3,bcx4,bcx5,bcx6,                     &
               imin,imax,cx1,cx2,cx3,cx4,cx5,cx6)

          do k=czmin,czmax
             if ((k>=cz3).and.(k<=cz4)) then
                do j=jmin,jmax
                   if ((j>=cy3).and.(j<=cy4)) then
                      !! Damping Left.
                      call fd_iso_3d_lx4_damping(bcx1,bcx2,j,j,k,      &
                           hdx_2,hdy_2,hdz_2,coefx,coefy,coefz,        &
                           xmin,xmax,ymin,ymax,zmin,zmax,              &
                           u,v,roc2,phi,eta,coef0,lapx,lapy,lapz)
   
                      !! Basic.
                      call fd_iso_3d_lx4_inner(bcx3,bcx4,j,k,          &
                           coefx,coefy,coefz,                          &
                           xmin,xmax,ymin,ymax,zmin,zmax,              &
                           u,v,roc2,coef0,lapx,lapy,lapz)
   
                      !! Damping Right.
                      call fd_iso_3d_lx4_damping(bcx5,bcx6,j,j,k,      &
                           hdx_2,hdy_2,hdz_2,coefx,coefy,coefz,        &
                           xmin,xmax,ymin,ymax,zmin,zmax,              &
                           u,v,roc2,phi,eta,coef0,lapx,lapy,lapz)
                   else
                      !! Damping Back & Front.
                      call fd_iso_3d_lx4_damping(imin,imax,j,j,k,      &
                           hdx_2,hdy_2,hdz_2,coefx,coefy,coefz,        &
                           xmin,xmax,ymin,ymax,zmin,zmax,              &
                           u,v,roc2,phi,eta,coef0,lapx,lapy,lapz)
                   endif
                enddo
             else
                !! Damping Top & Bottom.
                call fd_iso_3d_lx4_damping(imin,imax,jmin,jmax,k,      &
                     hdx_2,hdy_2,hdz_2,coefx,coefy,coefz,              &
                     xmin,xmax,ymin,ymax,zmin,zmax,                    &
                     u,v,roc2,phi,eta,coef0,lapx,lapy,lapz)
             endif
          enddo
       enddo
    enddo

  end subroutine fd_iso_3d_lx4_opt

end module m_fd_iso_3d_lx4_opt



!=============================================================================
!
! Compilation: ifort -cpp -DTEST_KERNEL m_fd_iso_3d_lx4_opt.f90 -o test_kernel
!
!=============================================================================
module m_test_kernel

  use m_fd_iso_3d_lx4_opt, only : fd_iso_3d_lx4_opt,fd_iso_3d_lx4_get_opt_bloc

  implicit none

  private

  type :: t_context
     !! Domain contour coordinates.
     integer :: xmin,xmax,ymin,ymax,zmin,zmax
     integer :: x1,x2,x3,x4,x5,x6
     integer :: y1,y2,y3,y4,y5,y6
     integer :: z1,z2,z3,z4,z5,z6

     !! Wavefields.
     real,dimension(:,:,:),allocatable :: u0
     real,dimension(:,:,:),allocatable :: u1

     !! PML/Taper implementation.
     real,dimension(:,:,:),allocatable :: eta,phi

     !! Velocity.
     real,dimension(:,:,:),allocatable :: roc2

     !! Stencils.
     integer                       :: lx,ly,lz
     real,dimension(:),allocatable :: coefx,coefy,coefz
     real                          :: hdx_2,hdy_2,hdz_2

     !! Hardware configuration.
     integer :: cache_bloc_x
     integer :: cache_bloc_y
  end type t_context

  public :: t_context
  public :: test_kernel_init,test_kernel_finalize
  public :: test_kernel,test_kernel_inner


contains

  !===========================================================================
  subroutine test_kernel_allocate(ctx,ierr,errmsg)
    !
    type(t_context),intent(inout),target :: ctx
    integer,intent(out)          :: ierr
    character(len=*),intent(out) :: errmsg


    allocate(ctx%u0(ctx%xmin-ctx%lx:ctx%xmax+ctx%lx,                   &
         ctx%ymin-ctx%ly:ctx%ymax+ctx%ly,                              &
         ctx%zmin-ctx%lz:ctx%zmax+ctx%lz),STAT=ierr)
    if (ierr.ne.0) then
       write(errmsg,'(a)') 'Error ! Cannot allocate ctx%u0.'
       return
    endif

    allocate(ctx%u1(ctx%xmin-ctx%lx:ctx%xmax+ctx%lx,                   &
         ctx%ymin-ctx%ly:ctx%ymax+ctx%ly,                              &
         ctx%zmin-ctx%lz:ctx%zmax+ctx%lz),STAT=ierr)
    if (ierr.ne.0) then
       write(errmsg,'(a)') 'Error ! Cannot allocate ctx%u1.'
       return
    endif

    allocate(ctx%phi(ctx%xmin:ctx%xmax,                                &
         ctx%ymin:ctx%ymax,                                            &
         ctx%zmin:ctx%zmax),STAT=ierr)
    if (ierr.ne.0) then
       write(errmsg,'(a)') 'Error ! Cannot allocate ctx%phi.'
       return
    endif

    allocate(ctx%eta(ctx%xmin-1:ctx%xmax+1,                            &
         ctx%ymin-1:ctx%ymax+1,                                        &
         ctx%zmin-1:ctx%zmax+1),STAT=ierr)
    if (ierr.ne.0) then
       write(errmsg,'(a)') 'Error ! Cannot allocate ctx%eta.'
       return
    endif

    allocate(ctx%roc2(ctx%xmin:ctx%xmax,                               &
         ctx%ymin:ctx%ymax,                                            &
         ctx%zmin:ctx%zmax),STAT=ierr)
    if (ierr.ne.0) then
       write(errmsg,'(a)') 'Error ! Cannot allocate ctx%roc2.'
       return
    endif

    allocate(ctx%coefx(-ctx%lx:ctx%lx),STAT=ierr)
    if (ierr.ne.0) then
       write(errmsg,'(a)') 'Error ! Cannot allocate ctx%coefx.'
       return
    endif

    allocate(ctx%coefy(-ctx%ly:ctx%ly),STAT=ierr)
    if (ierr.ne.0) then
       write(errmsg,'(a)') 'Error ! Cannot allocate ctx%coefy.'
       return
    endif

    allocate(ctx%coefz(-ctx%lz:ctx%lz),STAT=ierr)
    if (ierr.ne.0) then
       write(errmsg,'(a)') 'Error ! Cannot allocate ctx%coefz.'
       return
    endif

  end subroutine test_kernel_allocate



  !==========================================================================
  subroutine test_kernel_deallocate(ctx,ierr,errmsg)
    !
    type(t_context),intent(inout),target :: ctx
    integer,intent(out)          :: ierr
    character(len=*),intent(out) :: errmsg


    ierr=0
    errmsg=''

    deallocate(ctx%u0)
    deallocate(ctx%u1)
    deallocate(ctx%phi)
    deallocate(ctx%eta)
    deallocate(ctx%roc2)
    deallocate(ctx%coefx)
    deallocate(ctx%coefy)
    deallocate(ctx%coefz)

  end subroutine test_kernel_deallocate



  !==========================================================================
  subroutine test_kernel_stencil(l,dx,coef,ierr,errmsg)
    !
    ! Given the half-order l of the desired stencil, this routine returns 
    ! coef which contains the standard associated FD stencil second
    ! derivative.
    !
    integer,intent(in)        :: l             ! Half order.
    real,intent(in)           :: dx            ! Space increment.
    real,intent(out)          :: coef(-l:l)    ! Full stencil.
    integer,intent(out)       :: ierr
    character(len=*),intent(out) :: errmsg

    real                      :: hcoef(0:l)    ! Half stencil.


    ierr=0
    errmsg=''

    select case(l)
    case(0)  
       hcoef=(/ 0./)
    case(1)  
       hcoef=(/ -2., 1./)
    case(2)  
       hcoef=(/ -5./2., 4./3., -1./12./)
    case(3)  
       hcoef=(/ -49./18., 3./2., -3./20.,  1./90./)
    case(4)  
       hcoef=(/ -205./72., 8./5., -1./5., 8./315., -1./560./)
    case(5)  
       hcoef=(/ -5269./1800., 5./3., -5./21., 5./126., -5./1008., 1./3150./)
    case(6)  
       hcoef=(/ -5369./1800.,12./7.,-15./56.,10./189.,                 &
            -1./112., 2./1925., 1./16632./)
    case(7)  
       hcoef=(/ -266681./88200., 7./4., -7./24., 7./108.,              &
            -7./528., 7./3300., -7./30888., 1./84084./)
    case(10) 
       hcoef=(/ -1968329./635040., 20./11., -15./44., 40./429.,        &
            -15./572., 24./3575., -5./3432., 30./119119.,              &
            -5./155584., 10./3741309., -1./9237800./)
    case default
       write(errmsg,'(a)') 'Error ! Cannot compute FD stencil : scheme order unavailable.'
       ierr=1
       return
    end select

    coef=(/hcoef(l:1:-1),hcoef/)/(dx*dx)

  end subroutine test_kernel_stencil



  !===========================================================================
  subroutine profile_init(profile,i_min,i_max,n_first,n_last,param,    &
       ierr,errmsg)
    !
    real,dimension(:),allocatable,intent(inout) :: profile
    integer,intent(in)        :: i_min,i_max
    integer,intent(in)        :: n_first,n_last
    real,intent(in)           :: param
    integer,intent(out)       :: ierr
    character(len=*),intent(out) :: errmsg

    integer                   :: first_beg,first_end
    integer                   :: last_beg,last_end
    integer                   :: i

    ierr=0
    errmsg=''

    first_beg=i_min
    first_end=n_first+i_min-1
    last_beg=i_max-n_last+1
    last_end=i_max

    !----------------------------------------------------------------------
    ! Profile used to define PML damping function.
    !
    ! NB : parameter should be set up as 
    !            3 vmax log(1000.) / (2 ntaper lambdamax)
    !      i.e.  3 fmax log(1000.) / (2 ntaper)
    !----------------------------------------------------------------------
    profile=0.

    do i=1,first_end-first_beg+1
       profile(first_end-i+1)=(real(i)/real(first_end-first_beg+1))**2*param
    enddo

    do i=1,last_end-last_beg+1
       profile(last_beg+i-1)=(real(i)/real(last_end-last_beg+1))**2*param
    enddo

  end subroutine profile_init



  !===========================================================================
  subroutine profile_extend(eta,etax,etay,etaz,im,ip,jm,jp,km,kp,scale)
    !
    real,allocatable,dimension(:,:,:),intent(inout) :: eta
    real,allocatable,dimension(:),intent(in) :: etax,etay,etaz
    integer,intent(in)        :: im,ip,jm,jp,km,kp
    real,intent(in)           :: scale

    integer                   :: i,j,k,ii


    ii=1-lbound(etax,1)

    do k=km-ii,kp+ii
       do j=jm-ii,jp+ii
          do i=im-ii,ip+ii
             eta(i,j,k)=max(etax(i),etay(j),etaz(k))*scale
          enddo
       enddo
    enddo

  end subroutine profile_extend



  !===========================================================================
  subroutine profile_extend_all(eta,etax,etay,etaz,                    &
       xmin,xmax,ymin,ymax,                                            &
       x1,x2,x5,x6,y1,y2,y3,y4,y5,y6,z1,z2,z3,z4,z5,z6,scale)
    !
    real,allocatable,dimension(:,:,:),intent(inout) :: eta
    real,allocatable,dimension(:),intent(in) :: etax,etay,etaz
    integer,intent(in)        :: xmin,xmax,ymin,ymax
    integer,intent(in)        :: x1,x2,x5,x6
    integer,intent(in)        :: y1,y2,y3,y4,y5,y6
    integer,intent(in)        :: z1,z2,z3,z4,z5,z6
    real,intent(in)           :: scale


    eta=0.
    !! Top.
    if (z1/=-1) then
       call profile_extend(eta,etax,etay,etaz,xmin,xmax,ymin,ymax,     &
            z1,z2,scale)
    endif

    !! Bottom.
    if (z5/=-5) then
       call profile_extend(eta,etax,etay,etaz,xmin,xmax,ymin,ymax,     &
            z5,z6,scale)
    endif

    !! Front.
    if ((y1/=-1).and.(z3/=-3)) then
       call profile_extend(eta,etax,etay,etaz,xmin,xmax,y1,y2,         &
            z3,z4,scale)
    endif

    !! Back.
    if ((y6/=-6).and.(z3/=-3)) then
       call profile_extend(eta,etax,etay,etaz,xmin,xmax,y5,y6,         &
            z3,z4,scale)
    endif

    !! Left.
    if ((x1/=-1).and.(y3/=-3).and.(z3/=-3))then
       call profile_extend(eta,etax,etay,etaz,x1,x2,y3,y4,             &
            z3,z4,scale)
    endif

    !! Right.
    if ((x6/=-6).and.(y3/=-3).and.(z3/=-3))then
       call profile_extend(eta,etax,etay,etaz,x5,x6,y3,y4,             &
            z3,z4,scale)
    endif

  end subroutine profile_extend_all



  !==========================================================================
  subroutine test_kernel_profile(ctx,nx,ny,nz,ndampx,ndampy,ndampz,    &
       ntaperx,ntapery,ntaperz,vel_max,lambdamax,dt_sch,ierr,errmsg)
    !
    type(t_context),intent(inout),target :: ctx
    integer,intent(in)            :: nx,ny,nz
    integer,intent(in)            :: ndampx,ndampy,ndampz
    integer,intent(in)            :: ntaperx,ntapery,ntaperz
    real,intent(in)               :: vel_max,lambdamax,dt_sch
    integer,intent(out)           :: ierr
    character(len=*),intent(out)  :: errmsg

    real,dimension(:),allocatable :: etax,etay,etaz
    real                          :: param


    ierr=0
    errmsg=''

    allocate(etax(0:nx+1),STAT=ierr)
    if (ierr.ne.0) then
       write(errmsg,'(a)') 'Error ! Cannot allocate etax.'
       ierr=-1
       return
    endif
    param=3.*vel_max*log(1000.)/(2.*ntaperx*lambdamax)
    call profile_init(etax,0,nx+1,ndampx,ndampx,param,ierr,errmsg)
    if (ierr.ne.0) return

    allocate(etay(0:ny+1),STAT=ierr)
    if (ierr.ne.0) then
       write(errmsg,'(a)') 'Error ! Cannot allocate etay.'
       ierr=-1
       return
    endif
    param=3.*vel_max*log(1000.)/(2.*ntapery*lambdamax)
    call profile_init(etay,0,ny+1,ndampy,ndampy,param,ierr,errmsg)
    if (ierr.ne.0) return

    allocate(etaz(0:nz+1),STAT=ierr)
    if (ierr.ne.0) then
       write(errmsg,'(a)') 'Error ! Cannot allocate etaz.'
       ierr=-1
       return
    endif
    param=3.*vel_max*log(1000.)/(2.*ntaperz*lambdamax)
    call profile_init(etaz,0,nz+1,ndampz,ndampz,param,ierr,errmsg)
    if (ierr.ne.0) return

    call profile_extend_all(ctx%eta,etax,etay,etaz,                    &
         ctx%xmin,ctx%xmax,ctx%ymin,ctx%ymax,                          &
         ctx%x1,ctx%x2,ctx%x5,ctx%x6,                                  &
         ctx%y1,ctx%y2,ctx%y3,ctx%y4,ctx%y5,ctx%y6,                    &
         ctx%z1,ctx%z2,ctx%z3,ctx%z4,ctx%z5,ctx%z6,                    &
         dt_sch)

    deallocate(etax,etay,etaz)

  end subroutine test_kernel_profile



  !==========================================================================
  subroutine test_kernel_init(ctx,xmin,xmax,ymin,ymax,zmin,zmax,       &
       dx,dy,dz,ndampx,ndampy,ndampz,                                  &
       ntaperx,ntapery,ntaperz,fmax,vscale,cfl_perc,ierr,errmsg)
    !
    type(t_context),intent(inout),target :: ctx
    integer,intent(in)           :: xmin,xmax,ymin,ymax,zmin,zmax
    real,intent(in)              :: dx,dy,dz ! Grid space increments
    integer,intent(in)           :: ndampx,ndampy,ndampz
    integer,intent(in)           :: ntaperx,ntapery,ntaperz
    real,intent(in)              :: fmax,vscale,cfl_perc
    integer,intent(out)          :: ierr
    character(len=*),intent(out) :: errmsg

    integer                   :: nx,ny,nz,k
    real                      :: dt_sch,courant,vel_max,lambdamax
    integer                   :: l2_cache_size,l2_n_shared_core
    logical                   :: haspml


    ierr=0
    errmsg=''

    ctx%xmin=xmin
    ctx%xmax=xmax
    ctx%ymin=ymin
    ctx%ymax=ymax
    ctx%zmin=zmin
    ctx%zmax=zmax

    ctx%lx=4
    ctx%ly=4
    ctx%lz=4

    nx=xmax-xmin+1
    ny=ymax-ymin+1
    nz=zmax-zmin+1

    ctx%x1=xmin
    ctx%x2=xmin+ndampx-1
    ctx%x3=xmin+ndampx
    ctx%x4=xmax-ndampx
    ctx%x5=xmax-ndampx+1
    ctx%x6=xmax

    ctx%y1=ymin
    ctx%y2=ymin+ndampy-1
    ctx%y3=ymin+ndampy
    ctx%y4=ymax-ndampy
    ctx%y5=ymax-ndampy+1
    ctx%y6=ymax

    ctx%z1=zmin
    ctx%z2=zmin+ndampz-1
    ctx%z3=zmin+ndampz
    ctx%z4=zmax-ndampz
    ctx%z5=zmax-ndampz+1
    ctx%z6=zmax

    l2_cache_size=3*1048576
    l2_n_shared_core=2

    if ((ndampx.ne.0).or.(ndampy.ne.0).or.(ndampz.ne.0)) then
       haspml=.true.
    else
       haspml=.false.
    endif
    call fd_iso_3d_lx4_get_opt_bloc(ctx%cache_bloc_x,ctx%cache_bloc_y, &
         nx,haspml,0,0,l2_n_shared_core,l2_cache_size)

    call test_kernel_allocate(ctx,ierr,errmsg)
    if (ierr.ne.0) return

    ctx%u0=0.
    ctx%u1=0.
    ctx%phi=0.
    ctx%roc2=0. !! Need to be filled with velocity model.
    do k=zmin,zmax
       ctx%roc2(:,:,k)=1500.
    enddo

    vel_max=maxval(ctx%roc2)
    lambdamax=vel_max*vscale/fmax


    !! FD stencils.
    !! ------------
    call test_kernel_stencil(ctx%lx,dx,ctx%coefx,ierr,errmsg)
    if (ierr.ne.0) return
    call test_kernel_stencil(ctx%ly,dy,ctx%coefy,ierr,errmsg)
    if (ierr.ne.0) return
    call test_kernel_stencil(ctx%lz,dz,ctx%coefz,ierr,errmsg)
    if (ierr.ne.0) return


    !! Compute the CFL.
    !! ----------------
    courant=0.
    do k=-ctx%lx,ctx%lx
       courant=courant+abs(ctx%coefx(k))
    enddo
    do k=-ctx%ly,ctx%ly
       courant=courant+abs(ctx%coefy(k))
    enddo
    do k=-ctx%lz,ctx%lz
       courant=courant+abs(ctx%coefz(k))
    enddo
    courant=2./sqrt(courant)


    !! Compute the time step with a safety margin (percentage of the CFL).
    !! -------------------------------------------------------------------
    dt_sch=cfl_perc*courant/vel_max


    !! Definition of the PML damping coefficients.
    !! -------------------------------------------
    call test_kernel_profile(ctx,nx,ny,nz,ndampx,ndampy,ndampz,        &
         ntaperx,ntapery,ntaperz,                  &
         vel_max,lambdamax,dt_sch,ierr,errmsg)
    if (ierr.ne.0) return


    !! Constant density.
    !! -----------------
    !! Output :
    !!   roc2 = dt^2 * c^2
    ctx%roc2=ctx%roc2*dt_sch
    ctx%roc2=ctx%roc2*ctx%roc2


    !! Definition of constants for the approximation of the gradient.
    !! --------------------------------------------------------------
    ctx%hdx_2=1./(4.*dx*dx)
    ctx%hdy_2=1./(4.*dy*dy)
    ctx%hdz_2=1./(4.*dz*dz)

  end subroutine test_kernel_init



  !===========================================================================
  subroutine test_kernel_finalize(ctx,ierr,errmsg)
    !
    type(t_context),intent(inout),target :: ctx
    integer,intent(out)          :: ierr
    character(len=*),intent(out) :: errmsg


    ierr=0
    errmsg=''

    call test_kernel_deallocate(ctx,ierr,errmsg)
    if (ierr.ne.0) return

  end subroutine test_kernel_finalize



  !===========================================================================
  subroutine test_kernel(ctx,nstep,ierr,errmsg)
    !
    type(t_context),intent(inout),target :: ctx
    integer,intent(in)            :: nstep
    integer,intent(out)           :: ierr
    character(len=*),intent(out)  :: errmsg

    integer                       :: it,npo
    real,dimension(:,:,:),pointer :: ptr_u0,ptr_u1,swap_ptr


    ierr=0
    errmsg=''

    write(6,'(a)') 'Start test optimized kernel : Finite Difference - Isotropic - 3D'
    ptr_u0=>ctx%u0 ! Time step # n-1.
    ptr_u1=>ctx%u1 ! Time step # n.

    npo=int(nstep/10)

    do it=1,nstep

       call fd_iso_3d_lx4_opt(ctx%xmin,ctx%xmax,                       &
            ctx%ymin,ctx%ymax,ctx%zmin,ctx%zmax,                       &
            ctx%x2,ctx%x3,ctx%x4,ctx%x5,ctx%y2,ctx%y3,                 &
            ctx%y4,ctx%y5,ctx%z2,ctx%z3,ctx%z4,ctx%z5,                 &
            ctx%hdx_2,ctx%hdy_2,ctx%hdz_2,                             &
            ctx%coefx,ctx%coefy,ctx%coefz,                             &
            ctx%xmin,ctx%xmax,ctx%ymin,ctx%ymax,ctx%zmin,ctx%zmax,     &
            ptr_u0,ptr_u1,ctx%roc2,                                    &
            ctx%phi,ctx%eta,ctx%cache_bloc_x,ctx%cache_bloc_y)

       swap_ptr=>ptr_u0
       ptr_u0=>ptr_u1
       ptr_u1=>swap_ptr

       if ((mod(it,npo).eq.0).or.(it.eq.1).or.(it.eq.nstep)) then
          write(6,'(a,i6)') 'test_kernel time step #',it
          call flush(6)
       endif
    enddo
    write(6,'(a)') 'End test optimized kernel : Finite Difference - Isotropic - 3D'

  end subroutine test_kernel



  !==========================================================================
  subroutine test_kernel_inner(ctx,nstep,ierr,errmsg)
    !
    type(t_context),intent(inout),target :: ctx
    integer,intent(in)            :: nstep
    integer,intent(out)           :: ierr
    character(len=*),intent(out)  :: errmsg

    integer                       :: it,npo
    real,dimension(:,:,:),pointer :: ptr_u0,ptr_u1,swap_ptr


    ierr=0
    errmsg=''

    write(6,'(a)') 'Start test optimized kernel inner : Finite Difference - Isotropic - 3D'
    ptr_u0=>ctx%u0 ! Time step # n-1.
    ptr_u1=>ctx%u1 ! Time step # n.

    npo=int(nstep/10)

    do it=1,nstep

       call fd_iso_3d_lx4_opt(ctx%x3,ctx%x4,                           &
            ctx%y3,ctx%y4,ctx%z3,ctx%z4,                               &
            ctx%x2,ctx%x3,ctx%x4,ctx%x5,ctx%y2,ctx%y3,                 &
            ctx%y4,ctx%y5,ctx%z2,ctx%z3,ctx%z4,ctx%z5,                 &
            ctx%hdx_2,ctx%hdy_2,ctx%hdz_2,                             &
            ctx%coefx,ctx%coefy,ctx%coefz,                             &
            ctx%xmin,ctx%xmax,ctx%ymin,ctx%ymax,ctx%zmin,ctx%zmax,     &
            ptr_u0,ptr_u1,ctx%roc2,                                    &
            ctx%phi,ctx%eta,ctx%cache_bloc_x,ctx%cache_bloc_y)

       swap_ptr=>ptr_u0
       ptr_u0=>ptr_u1
       ptr_u1=>swap_ptr

       if ((mod(it,npo).eq.0).or.(it.eq.1).or.(it.eq.nstep)) then
          write(6,'(a,i6)') 'test_kernel_inner time step #',it
          call flush(6)
       endif
    enddo
    write(6,'(a)') 'End test optimized kernel inner : Finite Difference - Isotropic - 3D'

  end subroutine test_kernel_inner

end module m_test_kernel



!=============================================================================
program test

  use m_test_kernel, only : t_context,test_kernel_init,                &
                            test_kernel_finalize,test_kernel,          &
                            test_kernel_inner

  implicit none

  type(t_context)             :: ctx
  integer                     :: ierr
  character(len=1024)         :: errmsg
  integer 					  :: funcval
  integer*8 				  :: inticks, outicks
  integer*8					  :: time
  ! xmin,xmax,ymin,ymax,zmin,zmax: 1,254,1,118,1,254
  ! lx,ly,lz: 4,4,4
  ! dx,dy,dz: 20.,20.,20.
  ! dampx,dampy,dampz: 26,26,0
  ! ntaperx,ntapery,ntaperz: 3,3,6
  ! fmax: 25.0
  ! vscale: 1.0
  ! cfl_perc: 0.6
  funcval = 15;
  !call mypl_open();
  !call mypl_write(funcval);
  !call mygetticks(inticks)
  !call RestInitFortranWrapper()	
  call test_kernel_init(ctx,1,300,1,200,1,200,                         &
       20.,20.,20.,                                                    &
       26,26,26,                                                          &
       3,3,6,                                                          &
       25.0,1.0,0.6,ierr,errmsg)
  if (ierr.ne.0) then
     write(6,'(a)') errmsg
     stop
  endif

  call test_kernel(ctx,50,ierr,errmsg)

  !call test_kernel_inner(ctx,50,ierr,errmsg)

  call test_kernel_finalize(ctx,ierr,errmsg)
  if (ierr.ne.0) then
     write(6,'(a)') errmsg
     stop
  endif
  !call RestDestroyFortranWrapper()

!call mygetticks(outicks)
!call mypl_write(funcval);
!call mypl_close();
time = (outicks-inticks)
write(*,*) 'CYCLES : ', (time)

end program test
