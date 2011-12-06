module m_test_kernel

  use parser
  use m_var_kernel
  use m_fd_iso_3d_lx4_opt, only : fd_iso_3d_lx4_opt,fd_iso_3d_lx4_get_opt_bloc
  use m_fd_iso_3d_comm, only : fd_comm_boundary

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

      !! Source
      real,dimension(:),allocatable :: source
      integer                       :: ixs,iys,izs
      logical                       :: has_source

      !! Hardware configuration.
      integer :: cache_bloc_x
      integer :: cache_bloc_y
      integer :: cache_bloc_z
  end type t_context

  public :: t_context
  public :: test_kernel_init,test_kernel_finalize
  public :: test_kernel_output
  public :: test_kernel


contains


  !===========================================================================
  subroutine test_kernel_allocate(ctx,ierr,errmsg)
    !
    type(t_context),intent(inout),target :: ctx
    integer,intent(out)         :: ierr
    character(len=*),intent(out):: errmsg


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

    if (ctx%has_source) then
       allocate(ctx%source(nstep),STAT=ierr)
       if (ierr.ne.0) then
          write(errmsg,'(a)') 'Error ! Cannot allocate ctx%source.'
          return
       endif
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

    if (ctx%has_source) then
        deallocate(ctx%source)
    endif

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



  !=============================================================================
  subroutine source_check(ctx)
    !
    type(t_context),intent(inout),target   :: ctx
    
    ctx%has_source = .false.

    if ((ctx%xmin .le. ctx%ixs) .and. (ctx%ixs .le. ctx%xmax) .and. &
        (ctx%ymin .le. ctx%iys) .and. (ctx%iys .le. ctx%ymax) .and. &
        (ctx%zmin .le. ctx%izs) .and. (ctx%izs .le. ctx%zmax)) then

        ctx%has_source = .true.
    
    endif
  
  end subroutine source_check



  !=============================================================================
  subroutine test_kernel_source(nt,fmax,dt,source)

    implicit none    
    integer,intent(in):: nt
    real,intent(in)   :: fmax,dt
    real,intent(inout):: source(nt)
    integer           :: it,k
    real              :: sigma,tau,pi,t,scale

    sigma=0.6*fmax
    tau=1.0
    scale=8.0
    pi=3.1415927

    source=0.
    do it=1,nt
       t=dt*real(it-1)
       source(it)=-2*scale*sigma                           &
                  *(sigma-2*sigma*scale*(sigma*t-tau)**2)  &
                  *exp(-scale*(sigma*t-tau)**2)
    enddo

  end subroutine test_kernel_source



  !==========================================================================
  subroutine test_kernel_param(ctx,parfile)
  
    character(len=256),intent(in) :: parfile
    type(t_context),intent(inout),target :: ctx

    call FromFile(trim(parfile),"xmin",xmin); call FromFile(trim(parfile),"xmax",xmax)
    call FromFile(trim(parfile),"ymin",ymin); call FromFile(trim(parfile),"ymax",ymax)
    call FromFile(trim(parfile),"zmin",zmin); call FromFile(trim(parfile),"zmax",zmax)

    call FromFile(trim(parfile),"ixs",ctx%ixs)
    call FromFile(trim(parfile),"iys",ctx%iys)
    call FromFile(trim(parfile),"izs",ctx%izs)

    call FromFile(trim(parfile),"dx",dx)
    call FromFile(trim(parfile),"dy",dy)
    call FromFile(trim(parfile),"dz",dz)

    call FromFile(trim(parfile),"ndampx",ndampx)
    call FromFile(trim(parfile),"ndampy",ndampy)
    call FromFile(trim(parfile),"ndampz",ndampz)

    call FromFile(trim(parfile),"ntaperx",ntaperx)
    call FromFile(trim(parfile),"ntapery",ntapery)
    call FromFile(trim(parfile),"ntaperz",ntaperz)
  
    call FromFile(trim(parfile),"fmax",fmax)
    call FromFile(trim(parfile),"vscale",vscale)
    call FromFile(trim(parfile),"cfl_perc",cfl_perc)

    call FromFile(trim(parfile),"auto_compute_blocsize",auto_compute_blocsize)

    call FromFile(trim(parfile),"l2_cache_size",l2_cache_size)
    call FromFile(trim(parfile),"l2_n_shared_core",l2_n_shared_core)
    call FromFile(trim(parfile),"l3_cache_size",l3_cache_size)
    call FromFile(trim(parfile),"l3_n_shared_core",l3_n_shared_core)

    call FromFile(trim(parfile),"cache_bloc_x",ctx%cache_bloc_x)
    call FromFile(trim(parfile),"cache_bloc_y",ctx%cache_bloc_y)
    call FromFile(trim(parfile),"cache_bloc_z",ctx%cache_bloc_z)

    call FromFile(trim(parfile),"nstep",nstep)

    call FromFile(trim(parfile),"dim1",dims(1))
    call FromFile(trim(parfile),"dim2",dims(2))
    call FromFile(trim(parfile),"dim3",dims(3))

  end subroutine test_kernel_param



  !==========================================================================
  subroutine test_kernel_bloc(ctx,auto_compute_blocsize,dim_x,haspml,   &
                              l2_n_shared_core,l2_cache_size,           &
                              l3_n_shared_core,l3_cache_size            )
    !

    type(t_context),intent(inout),target :: ctx
    integer,intent(in)                   :: auto_compute_blocsize
    integer,intent(in)                   :: dim_x
    logical,intent(in)                   :: haspml
    integer,intent(in)                   :: l2_n_shared_core,l2_cache_size
    integer,intent(in)                   :: l3_n_shared_core,l3_cache_size

    if (auto_compute_blocsize.eq.1) then

        call fd_iso_3d_lx4_get_opt_bloc(ctx%cache_bloc_x,ctx%cache_bloc_y,          &
        dim_x,haspml,l2_n_shared_core,l2_cache_size,l3_n_shared_core,l3_cache_size)

    else 

        if (ctx%cache_bloc_x.le.0) ctx%cache_bloc_x = ctx%xmax-ctx%xmin+1
        if (ctx%cache_bloc_y.le.0) ctx%cache_bloc_y = ctx%ymax-ctx%ymin+1
        if (ctx%cache_bloc_z.le.0) ctx%cache_bloc_z = ctx%zmax-ctx%zmin+1

        if (ctx%cache_bloc_x.gt.(ctx%xmax-ctx%xmin+1)) ctx%cache_bloc_x = ctx%xmax-ctx%xmin+1
        if (ctx%cache_bloc_y.gt.(ctx%ymax-ctx%ymin+1)) ctx%cache_bloc_y = ctx%ymax-ctx%ymin+1
        if (ctx%cache_bloc_z.gt.(ctx%zmax-ctx%zmin+1)) ctx%cache_bloc_z = ctx%zmax-ctx%zmin+1

    endif

  end subroutine test_kernel_bloc



  !==========================================================================
  subroutine dim_indices(xmin,xmax,             &
                        x1,x2,x3,x4,x5,x6,      &
                        px1,px2,px3,px4,px5,px6 )
    !
    integer,intent(in) :: xmin,xmax 
    integer,intent(in) :: x1,x2,x3,x4,x5,x6
    integer,intent(out):: px1,px2,px3,px4,px5,px6 

    if ((xmin>=x1).and.(xmax<=x2)) then
       px1=xmin; px2=xmax
       px3=-3; px4=-4; px5=-5; px6=-6
    endif

    if ((xmin>=x5).and.(xmax<=x6)) then
       px1=-1; px2=-2; px3=-3; px4=-4
       px5=xmin; px6=xmax
    endif

    if ((xmin>=x3).and.(xmax<=x4)) then
       px1=-1; px2=-2
       px3=xmin; px4=xmax
       px5=-5; px6=-6
    endif

    if ((xmin<=x2).and.(xmax>=x3).and.(xmax<=x4)) then
       px1=xmin; px2=x2
       px3=x3; px4=xmax
       px5=-5; px6=-6
    endif

    if ((xmin>=x3).and.(xmin<=x4).and.(xmax>=x5)) then
       px1=-1; px2=-2
       px3=xmin; px4=x4
       px5=x5; px6=xmax
    endif

    if ((xmin<=x2).and.(xmax>=x5)) then
       px1=xmin; px2=x2
       px3=x3; px4=x4
       px5=x5; px6=xmax
    endif

  end subroutine dim_indices



  !==========================================================================
  subroutine test_kernel_indices(ctx,dims,coords,xmin,xmax,ymin,ymax,zmin,zmax)
 
    implicit none    

    type(t_context),intent(inout),target :: ctx
    integer,dimension(3),intent(in)      :: dims,coords
    integer,intent(in)                   :: xmin,xmax,ymin,ymax,zmin,zmax

    ctx%xmin=coords(1)*(xmax-xmin+1)/dims(1)+1
    ctx%xmax=(coords(1)+1)*xmax/dims(1)

    ctx%ymin=coords(2)*(ymax-ymin+1)/dims(2)+1
    ctx%ymax=(coords(2)+1)*ymax/dims(2)

    ctx%zmin=coords(3)*(zmax-zmin+1)/dims(3)+1
    ctx%zmax=(coords(3)+1)*zmax/dims(3)

  end subroutine test_kernel_indices



  !==========================================================================
  subroutine test_kernel_pml_indices(ctx,                           &
                                    x1,x2,x3,x4,x5,x6,              &
                                    y1,y2,y3,y4,y5,y6,              &
                                    z1,z2,z3,z4,z5,z6,              &
                                    xmin,xmax,ymin,ymax,zmin,zmax   )
    !
    type(t_context),intent(inout),target :: ctx
    integer,intent(in) :: x1,x2,x3,x4,x5,x6
    integer,intent(in) :: y1,y2,y3,y4,y5,y6
    integer,intent(in) :: z1,z2,z3,z4,z5,z6
    integer,intent(in) :: xmin,xmax,ymin,ymax,zmin,zmax

    call dim_indices(ctx%xmin,ctx%xmax,                         &
                     x1,x2,x3,x4,x5,x6,                         & 
                     ctx%x1,ctx%x2,ctx%x3,ctx%x4,ctx%x5,ctx%x6  )

    call dim_indices(ctx%ymin,ctx%ymax,                         &
                     y1,y2,y3,y4,y5,y6,                         & 
                     ctx%y1,ctx%y2,ctx%y3,ctx%y4,ctx%y5,ctx%y6  )

    call dim_indices(ctx%zmin,ctx%zmax,                         &
                     z1,z2,z3,z4,z5,z6,                         &
                     ctx%z1,ctx%z2,ctx%z3,ctx%z4,ctx%z5,ctx%z6  )

  end subroutine test_kernel_pml_indices



  !==========================================================================
  subroutine test_kernel_init(ctx,ierr,errmsg)
    !
    use mpi

    type(t_context),intent(inout),target :: ctx
    integer,intent(out)          :: ierr
    character(len=*),intent(out) :: errmsg

    integer             :: x1,x2,x3,x4,x5,x6
    integer             :: y1,y2,y3,y4,y5,y6
    integer             :: z1,z2,z3,z4,z5,z6

    integer             :: nx,ny,nz,k
    real                :: dt_sch,courant,vel_max,lambdamax
    logical             :: haspml

    character(len=256)  :: parfile

    ierr=0
    errmsg=''

    call FromPar('par',parfile,'iso.par')
    call test_kernel_param(ctx,parfile)

    call mpi_init(ierr)

    call mpi_comm_size(mpi_comm_world, size, ierr)
    call mpi_cart_create(mpi_comm_world, 3, dims, periods, reorder, comm_cart, ierr)
    call mpi_comm_rank(comm_cart, rank, ierr)
    call mpi_cart_coords(comm_cart, rank, 3, coords, ierr)

    nx=xmax-xmin+1
    ny=ymax-ymin+1
    nz=zmax-zmin+1

    x1=xmin
    x2=xmin+ndampx-1
    x3=xmin+ndampx
    x4=xmax-ndampx
    x5=xmax-ndampx+1
    x6=xmax

    y1=ymin
    y2=ymin+ndampy-1
    y3=ymin+ndampy
    y4=ymax-ndampy
    y5=ymax-ndampy+1
    y6=ymax

    z1=zmin
    z2=zmin+ndampz-1
    z3=zmin+ndampz
    z4=zmax-ndampz
    z5=zmax-ndampz+1
    z6=zmax

    ctx%lx=4
    ctx%ly=4
    ctx%lz=4

    ! Indices of the computational domain computed by the current process   
    call test_kernel_indices(ctx,dims,coords,xmin,xmax,ymin,ymax,zmin,zmax)

    ! PML Indices in the computational domain of the current process
    call test_kernel_pml_indices(ctx,                           &
                                x1,x2,x3,x4,x5,x6,              &
                                y1,y2,y3,y4,y5,y6,              &
                                z1,z2,z3,z4,z5,z6,              &
                                xmin,xmax,ymin,ymax,zmin,zmax   )

    if ((ndampx.ne.0).or.(ndampy.ne.0).or.(ndampz.ne.0)) then
       haspml=.true.
    else
       haspml=.false.
    endif

    ! Blocks sizes for spatial blocking
    call test_kernel_bloc(ctx,auto_compute_blocsize,nx,haspml,   &
                          l2_n_shared_core,l2_cache_size,        &
                          l3_n_shared_core,l3_cache_size         )


    ! Check if the source is in the domain
    call source_check(ctx)


    call test_kernel_allocate(ctx,ierr,errmsg)
    if (ierr.ne.0) return

    ctx%u0=0.
    ctx%u1=0.
    ctx%phi=0.
    ctx%roc2=0. !! Need to be filled with velocity model.
    do k=ctx%zmin,ctx%zmax
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
                             vel_max,lambdamax,dt_sch,ierr,errmsg      )    
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


    if (ctx%has_source) then
        call test_kernel_source(nstep,fmax,dt_sch,ctx%source)
    endif

  end subroutine test_kernel_init



  !===========================================================================
  subroutine test_kernel_finalize(ctx,ierr,errmsg)
    !
    use mpi

    type(t_context),intent(inout),target :: ctx
    integer,intent(out)          :: ierr
    character(len=*),intent(out) :: errmsg


    ierr=0
    errmsg=''

    call test_kernel_deallocate(ctx,ierr,errmsg)
    if (ierr.ne.0) return

    call mpi_finalize(ierr)

  end subroutine test_kernel_finalize



  !===========================================================================
  subroutine test_kernel_output(ctx,ierr)
    !
    use mpi

    type(t_context),intent(in),target :: ctx
    integer,intent(out)               :: ierr

    integer              :: fh
    integer              :: wavefield
    integer              :: status(MPI_STATUS_SIZE)
    integer              :: ndims,order,count
    integer,dimension(3) :: array_sizes,array_subsizes,array_starts
    integer(kind=MPI_OFFSET_KIND) :: disp

    !-- Derived Type Creation --
    ndims=3
    order=MPI_ORDER_FORTRAN
  
    array_sizes(:)=(/xmax-xmin+1,ymax-ymin+1,zmax-zmin+1/)
    array_subsizes(:)=(/ctx%xmax-ctx%xmin+1,ctx%ymax-ctx%ymin+1,ctx%zmax-ctx%zmin+1/)
    array_starts(:)=(/ctx%xmin-1,ctx%ymin-1,ctx%zmin-1/)
  
    call mpi_type_create_subarray(ndims,array_sizes,array_subsizes,array_starts,order,MPI_REAL,wavefield,ierr)
   
    call mpi_type_commit(wavefield,ierr)
   
    call mpi_barrier(comm_cart,ierr)
  
    !-- File Creation --
    call mpi_file_open(comm_cart,'mpi-snapshots.H@',                            &
                       MPI_MODE_CREATE+MPI_MODE_WRONLY+MPI_MODE_UNIQUE_OPEN,    &
                       MPI_INFO_NULL,fh,ierr                                    )
  
    !-- View Setting --
    disp=0
    call mpi_file_set_view(fh,disp,MPI_REAL,wavefield,'native',MPI_INFO_NULL,ierr)

    call mpi_barrier(comm_cart,ierr)
  
    !-- Data Printing --
    count=(ctx%xmax-ctx%xmin+1)*(ctx%ymax-ctx%ymin+1)*(ctx%zmax-ctx%zmin+1)

    call mpi_file_write_all(fh,ctx%u0(ctx%xmin:ctx%xmax,        &
                                      ctx%ymin:ctx%ymax,        &
                                      ctx%zmin:ctx%zmax),       &
                                      count,MPI_REAL,status,ierr)

    call mpi_barrier(comm_cart,ierr)
  
    !-- File Closure --
    call mpi_file_close(fh,ierr)
  
    !-- Derived Type Deallocation --
    call mpi_type_free(wavefield,ierr)

  end subroutine test_kernel_output



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

       call fd_iso_3d_lx4_opt(ctx%xmin,ctx%xmax,                        &
            ctx%ymin,ctx%ymax,ctx%zmin,ctx%zmax,                        &
            ctx%x2,ctx%x3,ctx%x4,ctx%x5,ctx%y2,ctx%y3,                  &
            ctx%y4,ctx%y5,ctx%z2,ctx%z3,ctx%z4,ctx%z5,                  &
            ctx%hdx_2,ctx%hdy_2,ctx%hdz_2,                              &
            ctx%coefx,ctx%coefy,ctx%coefz,                              &
            ctx%xmin,ctx%xmax,ctx%ymin,ctx%ymax,ctx%zmin,ctx%zmax,      &
            ptr_u0,ptr_u1,ctx%roc2,                                     &
            ctx%phi,ctx%eta,ctx%cache_bloc_x,ctx%cache_bloc_y           )

       if (ctx%has_source) then
           ptr_u0(ctx%ixs,ctx%iys,ctx%izs) =                            &
                    ptr_u0(ctx%ixs,ctx%iys,ctx%izs)                     &
                    + ctx%source(it)
       endif

       call fd_comm_boundary(comm_cart,rank,coords,                     &
            ctx%xmin,ctx%xmax,ctx%ymin,ctx%ymax,ctx%zmin,ctx%zmax,      &
            ctx%lx,ctx%ly,ctx%lz,                                       &
            ptr_u0                                                      )

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

end module m_test_kernel



!=============================================================================
program test

  use m_var_kernel
  use m_test_kernel, only : t_context,test_kernel_init,test_kernel_finalize,&
                            test_kernel,test_kernel_output

  implicit none

  type(t_context)             :: ctx
  integer                     :: ierr
  character(len=1024)         :: errmsg

  call test_kernel_init(ctx,ierr,errmsg)
  if (ierr.ne.0) then
     write(6,'(a)') errmsg
     stop
  endif

  call test_kernel(ctx,nstep,ierr,errmsg)

 !call test_kernel_output(ctx,ierr)

  call test_kernel_finalize(ctx,ierr,errmsg)
  if (ierr.ne.0) then
     write(6,'(a)') errmsg
     stop
  endif

end program test
