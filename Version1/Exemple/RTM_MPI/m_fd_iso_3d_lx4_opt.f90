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
