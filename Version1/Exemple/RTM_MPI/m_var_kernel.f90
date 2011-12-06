module m_var_kernel

  implicit none
  
  real    :: dx,dy,dz
  integer :: xmin,xmax,ymin,ymax,zmin,zmax
  integer :: ndampx,ndampy,ndampz
  integer :: ntaperx,ntapery,ntaperz
  
  real    :: fmax,vscale,cfl_perc

  ! Hardware configuration
  integer :: l2_cache_size,l3_cache_size
  integer :: l2_n_shared_core,l3_n_shared_core

  integer :: auto_compute_blocsize

  integer :: nstep

  ! Processes
  integer              :: procs
  integer,dimension(3) :: dims
  integer              :: rank,size

  ! Cartesian Topology
  logical,dimension(3) :: periods=.false.
  integer,dimension(3) :: coords
  integer              :: comm_cart
  logical              :: reorder=.false.

end module m_var_kernel
