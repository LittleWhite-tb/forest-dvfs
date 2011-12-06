module parser
!------------


!-------------------------------------------------------------------------------
!-------------------------------------------------------------------------------
!                GENERIC ROUTINES FROMPAR and FROMFILE
!                -------------------------------------
!
!  FromPar( keyword, value, default_value )
!     Read an argument on the command line or in the file associated with
!     commandline keyword "par", where:
!     keyword:       is the keyword (character(*))
!     value:         will be the associated value (real, integer, logical or 
!                    character(*))
!     default_value: optional argument of type value. If not present and that
!                    the keyword and value are not found on the command line, 
!                    the progrzam stops
!
!  FromFile( filename, keyword, value, default_value )
!     Read an argument in a file
!     filename:      is the name of the file (character(*))
!     keyword:       is the keyword (character(*))
!     value:         will be the associated value (real, integer, logical or 
!                    character)
!     default_value: optional argument of type value. If not present and that
!                    the keyword and its associated value are not found on the
!                    command line, the program stops
!
!  ToFile( filename, keyword, value )
!     Write an argument in a file
!     filename:      is the name of the file (character(*))
!     keyword:       is the keyword (character(*))
!     value:         will be the associated value (real, integer, logical or 
!
!  Exception:
!     When value is of type logical, default_value can be an integer, and value
!        read from command line or file is considered as integer. If this 
!        quantity is 1, value will be .true. and .false. otherwise
!     When value is of type real, default_value can be integer
!     When value is of type character, read value can be a single word or a
!        sentence between ""
!
!  Example:
!
!     program myprog
!
!        use parser
!        real            :: dx
!        integer         :: nx
!        character(1024) :: inputfile
!        character(1024) :: Label
!        logical         :: test
!
!        call FromPar( "test", test, 0 )
!        call FromPar( "in", inputfile )           ! "in=" must defined on the
!                                                  ! command line
!        call FromFile( inputfile, "n1", nx )      ! "n1=" has to be present in
!                                                  !  the file
!        call FromFile( inputfile, "d2", dx, 10. ) ! if "d2=" is not found, dx
!                                                  ! will be 10.
!        call ToFile( "result.ascii", "d2", dx )
!
!     end program myprog
!
!-------------------------------------------------------------------------------
!-------------------------------------------------------------------------------



   integer, private, parameter :: cdim=4096, maxparm=300
   integer, private            :: iparm=0, iunit=987654321
   character(200), private     :: parsed_param(maxparm)
   character(1), private       :: blank=" ", guillemet="""", quote="'", &
                                  tab="	"
   logical, private            :: debug=.false.

   private :: getinodeparser, from_par_string, from_par_real, from_par_integer,&
              from_par_real2, from_par_logical, from_par_logical2,             &
              from_file_string, from_file_logical2, from_file_real8,           &
              from_file_real, from_file_integer, from_file_logical,            &
              from_file_real2, string2par, string2par2, string2real,           &
              to_file_string, to_file_integer, to_file_logical, to_file_real,  &
              strin2integer, fromfilestring, getcommandline


   interface FromPar
       module procedure from_par_string
       module procedure from_par_integer
       module procedure from_par_logical
       module procedure from_par_logical2
       module procedure from_par_real
       module procedure from_par_real2
   end interface

   interface FromFile
       module procedure from_file_string
       module procedure from_file_integer
       module procedure from_file_logical
       module procedure from_file_logical2
       module procedure from_file_real
       module procedure from_file_real2
       module procedure from_file_real8
   end interface

   interface ToFile
       module procedure to_file_string
       module procedure to_file_integer
       module procedure to_file_logical
       module procedure to_file_real
   end interface


contains
!-------



   subroutine writeparsed(filename)

      character(*), intent(in) :: filename
  
      integer :: i

      open(iunit,file=TRIM(filename))
      do i=1,iparm
         write(iunit,*) TRIM(parsed_param(i))
      end do
      close(iunit)

   end subroutine writeparsed

     
   integer function getinodeparser()

      implicit none 
#ifdef MPI
      include 'mpif.h'
#endif
      integer :: in, ierr

      getinodeparser=0
#ifdef MPI
      call MPI_COMM_RANK(MPI_COMM_WORLD,in,ierr)
      getinodeparser=in
#endif

   end function getinodeparser






!-------------------------------------------------------------------------------
!                               FROMPAR ROUTINES
!-------------------------------------------------------------------------------
   subroutine from_par_string(keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)           :: keyword
      character(*), intent(out)          :: value
      character(*), intent(in), optional :: default

      logical         :: lerr, lerrg, lerrb
      character(cdim) :: valueb, valueg
      integer         :: ilb, ilg, iposg, iposb, len_value, ierr, inodeparse

      value=""
      inodeparse=getinodeparser()

      if (inodeparse==0) then

         call fromparstring(keyword,valueg,lerrg,delimiter=guillemet,il=ilg, &
                            ipos=iposg)
         call fromparstring(keyword,valueb,lerrb,delimiter=blank,il=ilb,     &
                            ipos=iposb)
         if (TRIM(valueb)==(guillemet//guillemet)) lerrb=.true.
         lerr=(lerrg.and.lerrb)
         if (lerr) then
            if (present(default)) then
               value=TRIM(default)
            else
               write(0,*) 'Error from FromPar routine. Cannot read value' &
                          //' associated with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         else if (lerrg) then
            value=valueb
         else if (lerrb) then
            value=valueg
         else
            if (ilg>ilb) then
               value=valueg
            elseif (ilg==ilb) then
               if (iposg>iposb) then
                  value=valueg
               else
                  value=valueb
               end if
            else
               value=valueb
            end if
         end if
         len_value=len(value)

      end if

#ifdef MPI
      call MPI_BCAST(len_value,1,MPI_INTEGER,0,MPI_COMM_WORLD,ierr)
      call MPI_BCAST(value,len_value,MPI_CHARACTER,0,MPI_COMM_WORLD,ierr)
#endif

      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)="par: "//TRIM(keyword)//"="//TRIM(value)

   end subroutine from_par_string


   subroutine from_par_integer(keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)      :: keyword
      integer, intent(out)          :: value
      integer, intent(in), optional :: default

      logical         :: lerr, lerr2
      character(cdim) :: svalue
      integer         :: ierr, inodeparse

      inodeparse=getinodeparser()
      if (inodeparse==0) then

         call fromparstring(keyword,svalue,lerr)
  
         if (lerr) then
            if (present(default)) then
               value=default
            else
               write(0,*) 'Error from FromPar routine. Cannot read value' &
                          //' associated with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         else
            value=string2integer(svalue,lerr2)
            if (lerr2) then
               write(0,*) 'Error from FromPar routine. Bad value associated' &
                          //' with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         end if

      end if

#ifdef MPI
      call MPI_BCAST(value,1,MPI_INTEGER,0,MPI_COMM_WORLD,ierr)
#endif

      write(svalue,*) value
      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)="par: "//TRIM(keyword)//"="//TRIM(ADJUSTL(svalue))

   end subroutine from_par_integer


   subroutine from_par_logical(keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)      :: keyword
      logical, intent(out)          :: value
      integer, intent(in), optional :: default

      logical         :: lerr, lerr2
      character(cdim) :: svalue
      integer         :: ierr, inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then

         if (present(default)) then
            if ( default/=0 .and. default/=1 ) then
               write(0,*) 'Error from FromPar routine. Default value for' &
                          //' keyword ',TRIM(keyword),' can only be 0 or 1'
               call exit(-1)
            end if
         end if

         call fromparstring(keyword,svalue,lerr)
     
         if (lerr) then
            if (present(default)) then
               value=(default==1)
            else
               write(0,*) 'Error from FromPar routine. Cannot read value' &
                          //' associated with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         else
            value=(string2integer(svalue,lerr2)==1)
            if (lerr2) then
               write(0,*) 'Error from FromPar routine. Bad value associated' &
                          //' with keyword ',TRIM(keyword)
               call exit(-1)
            else if ( TRIM(svalue)/="0" .and. TRIM(svalue)/="1" ) then
               write(0,*) 'Error from FromPar routine. Value read for' &
                          //' keyword ',TRIM(keyword),' different to 0 or 1'
               call exit(-1)
            end if
         end if

      end if

#ifdef MPI
      call MPI_BCAST(value,1,MPI_LOGICAL,0,MPI_COMM_WORLD,ierr)
#endif

      write(svalue,*) value
      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)="par: "//TRIM(keyword)//"="//TRIM(ADJUSTL(svalue))

   end subroutine from_par_logical

   subroutine from_par_logical2(keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)      :: keyword
      logical, intent(out)          :: value
      logical, intent(in)           :: default

      logical         :: lerr, lerr2
      character(cdim) :: svalue
      integer         :: ierr, inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then

         call fromparstring(keyword,svalue,lerr)
     
         if (lerr) then
            value=default
         else
            value=(string2integer(svalue,lerr2)==1)
            if (lerr2) then
               write(0,*) 'Error from FromPar routine. Bad value associated' &
                          //' with keyword ',TRIM(keyword)
               call exit(-1)
            else if ( TRIM(svalue)/="0" .and. TRIM(svalue)/="1" ) then
               write(0,*) 'Error from FromPar routine. Value read for' &
                          //' keyword ',TRIM(keyword),' different to 0 or 1'
               call exit(-1)
            end if
         end if

      end if

#ifdef MPI
      call MPI_BCAST(value,1,MPI_LOGICAL,0,MPI_COMM_WORLD,ierr)
#endif

      write(svalue,*) value
      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)="par: "//TRIM(keyword)//"="//TRIM(ADJUSTL(svalue))

   end subroutine from_par_logical2


   subroutine from_par_real(keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)   :: keyword
      real, intent(out)          :: value
      real, intent(in), optional :: default

      logical         :: lerr, lerr2
      character(cdim) :: svalue
      integer         :: ierr, inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then

         call fromparstring(keyword,svalue,lerr)
  
         if (lerr) then
            if (present(default)) then
               value=default
            else
               write(0,*) 'Error from FromPar routine. Cannot read value' &
                          //' associated with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         else
            value=string2real(svalue,lerr2)
            if (lerr2) then
               write(0,*) 'Error from FromPar routine. Bad value associated' &
                          //' with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         end if

      end if
   
#ifdef MPI
      call MPI_BCAST(value,1,MPI_REAL,0,MPI_COMM_WORLD,ierr)
#endif

      write(svalue,*) value
      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)="par: "//TRIM(keyword)//"="//TRIM(ADJUSTL(svalue))

   end subroutine from_par_real


   subroutine from_par_real2(keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)   :: keyword
      real, intent(out)          :: value
      integer, intent(in)        :: default

      logical         :: lerr, lerr2
      character(cdim) :: svalue
      integer         :: ierr, inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then

         call fromparstring(keyword,svalue,lerr)
  
         if (lerr) then
            value=default
         else
            value=string2real(svalue,lerr2)
            if (lerr2) then
               write(0,*) 'Error from FromPar routine. Bad value associated' &
                          //' with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         end if

      end if
   
#ifdef MPI
      call MPI_BCAST(value,1,MPI_REAL,0,MPI_COMM_WORLD,ierr)
#endif

      write(svalue,*) value
      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)="par: "//TRIM(keyword)//"="//TRIM(ADJUSTL(svalue))

   end subroutine from_par_real2

  



!-------------------------------------------------------------------------------
!                              FROMFILE ROUTINES
!-------------------------------------------------------------------------------
   subroutine from_file_string(filename,keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)           :: filename, keyword
      character(*), intent(out)          :: value
      character(*), intent(in), optional :: default

      logical         :: lerr, lerrg, lerrb, lerrq
      character(cdim) :: valueb, valueg, valueq, lvalue(3), lvalue2(3)
      integer         :: ilb, ilg, ilq, iposg, iposb, iposq, len_value, ierr,  &
                         inodeparse, lipos(3), lil(3), lipos2(3), ngood,       &
                         nequal, indice(1), i, il

      inodeparse=getinodeparser()

      if (inodeparse==0) then

         valueb=""; valueg=""; valueq=""
         call fromfilestring(filename,keyword,valueq,lerrq,quote,ilq,iposq)
         call fromfilestring(filename,keyword,valueg,lerrg,guillemet,ilg,iposg)
         call fromfilestring(filename,keyword,valueb,lerrb,il=ilb,ipos=iposb)
         if (TRIM(valueb)==(guillemet//guillemet)) lerrb=.true.
         lerr=(lerrg.and.lerrb.and.lerrq)
         ! All are false
         if (lerr) then
            if (present(default)) then
               value=TRIM(default)
            else
               write(0,*) 'Error from FromFile routine. Cannot read value' &
                          //' associated with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         else
            ngood=0
            if (.not.lerrb) then
               ngood=ngood+1
               lvalue(ngood)=valueb
               lipos(ngood)=iposb
               lil(ngood)=ilb
            end if
            if (.not.lerrg) then
               ngood=ngood+1
               lvalue(ngood)=valueg
               lipos(ngood)=iposg
               lil(ngood)=ilg
            end if
            if (.not.lerrq) then
               ngood=ngood+1
               lvalue(ngood)=valueq
               lipos(ngood)=iposq
               lil(ngood)=ilq
            end if
            ! We have foond ngood value. determine the bigger ligne among them
            indice=maxloc(lil(1:ngood))   
            il=lil(indice(1))
            nequal=0
            do i=1,ngood
               if (lil(i)==il) then
                  nequal=nequal+1
                  lvalue2(nequal)=lvalue(i)
                  lipos2(nequal)=lipos(i)
               end if
            end do
            ! We have found nequal ligne among the ngood value, look for 
            ! biggest position in the line
            if (nequal==1) then
               value=lvalue2(nequal)
            else
               indice=maxloc(lipos2(1:nequal))   
               value=lvalue2(indice(1))
            end if
         end if
         len_value=len(value)

      end if

#ifdef MPI
      call MPI_BCAST(len_value,1,MPI_INTEGER,0,MPI_COMM_WORLD,ierr)
      call MPI_BCAST(value,len_value,MPI_CHARACTER,0,MPI_COMM_WORLD,ierr)
#endif

      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)=TRIM(filename)//": "//TRIM(keyword)//"="//TRIM(ADJUSTL(value))

   end subroutine from_file_string


   subroutine from_file_integer(filename,keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)      :: filename, keyword
      integer, intent(out)          :: value
      integer, intent(in), optional :: default

      logical         :: lerr, lerr2
      character(cdim) :: svalue
      integer         :: ierr, inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then

         call fromfilestring(filename,keyword,svalue,lerr)
      
         if (lerr) then
            if (present(default)) then
               value=default
            else
               write(0,*) 'Error from FromFile routine. Cannot read value' &
                          //' associated with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         else
            value=string2integer(svalue,lerr2)
            if (lerr2) then
               write(0,*) 'Error from FromFile routine. Bad value associated' &
                          //' with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         end if

      end if

#ifdef MPI
      call MPI_BCAST(value,1,MPI_INTEGER,0,MPI_COMM_WORLD,ierr)
#endif

      write(svalue,*) value
      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)=TRIM(filename)//": "//TRIM(keyword)//"="//TRIM(ADJUSTL(svalue))

   end subroutine from_file_integer


   subroutine from_file_logical(filename,keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)      :: filename, keyword
      logical, intent(out)          :: value
      integer, intent(in), optional :: default

      logical         :: lerr, lerr2
      character(cdim) :: svalue
      integer         :: ierr, inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then

         if (present(default)) then
            if ( default/=0 .and. default/=1 ) then
               write(0,*) 'Error from FromFile routine. Default value for' &
                          //' keyword ',TRIM(keyword),' can only be 0 or 1'
               call exit(-1)
            end if
         end if
   
         call fromfilestring(filename,keyword,svalue,lerr)
      
         if (lerr) then
            if (present(default)) then
               value=(default==1)
            else
               write(0,*) 'Error from FromFile routine. Cannot read value' &
                          //' associated with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         else
            value=(string2integer(svalue,lerr2)==1)
            if (lerr2) then
               write(0,*) 'Error from FromFile routine. Bad value associated' &
                          //' with keyword ',TRIM(keyword)
               call exit(-1)
            else if ( TRIM(svalue)/="0" .and. TRIM(svalue)/="1" ) then
               write(0,*) 'Error from FromFile routine. Value read for' &
                          //' keyword ',TRIM(keyword),' different to 0 or 1'
               call exit(-1)
            end if
         end if

      end if

#ifdef MPI
      call MPI_BCAST(value,1,MPI_LOGICAL,0,MPI_COMM_WORLD,ierr)
#endif

      write(svalue,*) value
      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)=TRIM(filename)//": "//TRIM(keyword)//"="//TRIM(ADJUSTL(svalue))

   end subroutine from_file_logical

   subroutine from_file_logical2(filename,keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)      :: filename, keyword
      logical, intent(out)          :: value
      logical, intent(in)           :: default

      logical         :: lerr, lerr2
      character(cdim) :: svalue
      integer         :: ierr, inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then

         call fromfilestring(filename,keyword,svalue,lerr)
      
         if (lerr) then
            value=default
         else
            value=(string2integer(svalue,lerr2)==1)
            if (lerr2) then
               write(0,*) 'Error from FromFile routine. Bad value associated' &
                          //' with keyword ',TRIM(keyword)
               call exit(-1)
            else if ( TRIM(svalue)/="0" .and. TRIM(svalue)/="1" ) then
               write(0,*) 'Error from FromFile routine. Value read for' &
                          //' keyword ',TRIM(keyword),' different to 0 or 1'
               call exit(-1)
            end if
         end if

      end if

#ifdef MPI
      call MPI_BCAST(value,1,MPI_LOGICAL,0,MPI_COMM_WORLD,ierr)
#endif

      write(svalue,*) value
      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)=TRIM(filename)//": "//TRIM(keyword)//"="//TRIM(ADJUSTL(svalue))

   end subroutine from_file_logical2


   subroutine from_file_real(filename,keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)   :: filename, keyword
      real, intent(out)          :: value
      real, intent(in), optional :: default

      logical         :: lerr, lerr2
      character(cdim) :: svalue
      integer         :: ierr, inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then

         call fromfilestring(filename,keyword,svalue,lerr)
      
         if (lerr) then
            if (present(default)) then
               value=default
            else
               write(0,*) 'Error from FromFile routine. Cannot read value' &
                          //' associated with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         else
            value=string2real(svalue,lerr2)
            if (lerr2) then
               write(0,*) 'Error from FromFile routine. Bad value associated' &
                          //' with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         end if

      end if

#ifdef MPI
      call MPI_BCAST(value,1,MPI_REAL,0,MPI_COMM_WORLD,ierr)
#endif

      write(svalue,*) value
      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)=TRIM(filename)//": "//TRIM(keyword)//"="//TRIM(ADJUSTL(svalue))

   end subroutine from_file_real


   subroutine from_file_real2(filename,keyword,value,default)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in)   :: filename, keyword
      real, intent(out)          :: value
      integer, intent(in)        :: default

      logical         :: lerr, lerr2
      character(cdim) :: svalue
      integer         :: ierr, inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then

         call fromfilestring(filename,keyword,svalue,lerr)
      
         if (lerr) then
            value=default
         else
            value=string2real(svalue,lerr2)
            if (lerr2) then
               write(0,*) 'Error from FromFile routine. Bad value associated' &
                          //' with keyword ',TRIM(keyword)
               call exit(-1)
            end if
         end if
   
      end if

#ifdef MPI
      call MPI_BCAST(value,1,MPI_REAL,0,MPI_COMM_WORLD,ierr)
#endif

      write(svalue,*) value
      iparm=min(iparm+1,maxparm)
      parsed_param(iparm)=TRIM(filename)//": "//TRIM(keyword)//"="//TRIM(ADJUSTL(svalue))

   end subroutine from_file_real2


   subroutine from_file_real8(filename,keyword,value)


      implicit none
      character(*), intent(in)   :: filename, keyword
      real(8), intent(out)       :: value

      real :: value2

      call from_file_real(filename,keyword,value2)
      value=value2

   end subroutine from_file_real8





!-------------------------------------------------------------------------------
!                              TOFILE ROUTINES
!-------------------------------------------------------------------------------
   subroutine to_file_string(filename,keyword,value)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in) :: filename, keyword, value

      integer         :: inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) call tofilestring(filename,keyword,value)

   end subroutine to_file_string


   subroutine to_file_integer(filename,keyword,value)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in) :: filename, keyword
      integer, intent(in)      :: value

      character(cdim) :: cvalue
      integer         :: inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then
         cvalue=integer2string(value)
         call tofilestring(filename,keyword,cvalue)
      end if

   end subroutine to_file_integer


   subroutine to_file_real(filename,keyword,value)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in) :: filename, keyword
      real, intent(in)         :: value

      character(cdim) :: cvalue
      integer         :: inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then
         cvalue=real2string(value)
         call tofilestring(filename,keyword,cvalue)
      end if

   end subroutine to_file_real


   subroutine to_file_logical(filename,keyword,value)

      implicit none
#ifdef MPI
      include 'mpif.h'
#endif
      character(*), intent(in) :: filename, keyword
      logical, intent(in)      :: value

      character(cdim) :: cvalue
      integer         :: inodeparse

      inodeparse=getinodeparser()

      if (inodeparse==0) then
         cvalue=logical2string(value)
         call tofilestring(filename,keyword,cvalue)
      end if

   end subroutine to_file_logical





!-------------------------------------------------------------------------------
!                    FROMPARSTRING / FROMFILESTRING
!-------------------------------------------------------------------------------
subroutine fromparstring(keyword,svalue,lerr,delimiter,il,ipos)

   implicit none
   character(*), intent(in)           :: keyword
   character(*), intent(out)          :: svalue
   logical, intent(out)               :: lerr
   character(1), intent(in), optional :: delimiter
   integer, intent(out), optional     :: il, ipos

   character(cdim) :: commandline, parfile
   character(cdim) :: svalue1, svalue2
   logical         :: lerrpar, lerr1, lerr2
   character(1)    :: delim
   integer         :: iposs, ils, iposs1, iposs2, ils1

   delim=blank
   if (present(delimiter)) delim=delimiter

   lerr=.false.
   call getcommandline(commandline)
   iposs=string2par(commandline,"par",parfile,lerrpar,blank)

   ! get value from par file
   lerr1=.true.
   if (.not.lerrpar)  &
      call fromfilestring(parfile,keyword,svalue1,lerr1,delim,ils1,iposs1)
   ! get value from commad line
   iposs2=string2par(commandline,keyword,svalue2,lerr2,delim)
   if (.not.lerr1) then
      svalue=svalue1
      iposs=iposs1
      ils=ils1
   end if
   if (.not.lerr2) then
      svalue=svalue2
      iposs=iposs2
      ils=9999999
   end if
   if (lerr1.and.lerr2) lerr=.true.
   if (.not.lerr) then
      if (present(il)) il=ils
      if (present(ipos)) ipos=iposs
   else
      if (present(il)) il=0
      if (present(ipos)) ipos=0
   endif

end subroutine fromparstring


subroutine fromfilestring(filename,keyword,svalue,lerr,delimiter,il,ipos)

   implicit none
   character(*), intent(in)            :: filename, keyword
   character(*), intent(out)           :: svalue
   logical, intent(out)                :: lerr
   character(1), intent(in), optional  :: delimiter
   integer, intent(out), optional      :: il, ipos

   character(cdim) :: temp, svaluetemp, temp2
   logical         :: lerr2
   character(1)    :: delim
   integer         :: iposs, ils, iposs2

   delim=blank
   if (present(delimiter)) delim=delimiter

   lerr=.true.
   open(unit=iunit,file=TRIM(filename),form='formatted',err=90)

   ils=1
   do
      read(iunit,100,end=80) temp
      iposs2=index(TRIM(temp),"#",.false.)
      if (iposs2==0) iposs2=len_trim(temp)
      iposs=string2par(temp(1:iposs2),TRIM(keyword),svaluetemp,lerr2,delim)
      if (.not.lerr2) then
         svalue=svaluetemp
         lerr=.false.
         if (present(il)) il=ils
         if (present(ipos)) ipos=iposs
      end if
      ils=ils+1
   end do
   if (lerr) then
      if (present(il)) il=0
      if (present(ipos)) ipos=0
   endif

90  write(0,*) 'Error opening file ',TRIM(filename)
    call exit(-1)
80  close(iunit)
100 format(a2048)

end subroutine fromfilestring


subroutine tofilestring(filename,keyword,svalue)

   implicit none
   character(*), intent(in) :: filename, keyword, svalue

   integer :: iunit

   open(unit=iunit,file=TRIM(filename),form='formatted',position='append')
   write(iunit,*) TRIM(keyword)//'='//TRIM(svalue)
   close(iunit)

end subroutine tofilestring





!-------------------------------------------------------------------------------
!                               DEEP ROUTINES
!-------------------------------------------------------------------------------
subroutine getcommandline(string)

   implicit none
   character(*), intent(out) :: string

   integer         :: i, iargc
   character(cdim) :: temp
   
   string=""
   do i=1,iargc()
      call getarg(i,temp)
      string=TRIM(string)//" "//TRIM(temp)
   end do

end subroutine getcommandline


integer function string2par(string,keyword,svalue,lerr,delimiter)

   implicit none
   character(*), intent(in)  :: string, keyword
   character(*), intent(out) :: svalue
   logical, intent(out)      :: lerr
   character(1), intent(in), optional :: delimiter

   logical      :: lerr1, lerr2, lerr3, lerr4
   integer      :: ipos1, ipos2, ipos3, ipos4, ipos
   character(cdim) :: svalue1, svalue2, svalue3, svalue4

   ipos=0; lerr=.true.

   if (present(delimiter)) then
      ipos1=string2par2(string,keyword,svalue1,lerr1,blank,tab,delimiter)
      ipos2=string2par2(string,keyword,svalue2,lerr2,blank,blank,delimiter)
      ipos3=string2par2(string,keyword,svalue3,lerr3,tab,tab,delimiter)
      ipos4=string2par2(string,keyword,svalue4,lerr4,tab,blank,delimiter)
   else
      ipos1=string2par2(string,keyword,svalue1,lerr1,blank,tab)
      ipos2=string2par2(string,keyword,svalue2,lerr2,blank,blank)
      ipos3=string2par2(string,keyword,svalue3,lerr3,tab,tab)
      ipos4=string2par2(string,keyword,svalue4,lerr4,tab,blank)
   end if

   if (.not.lerr1) then
      lerr=.false.
      ipos=max(ipos,ipos1)
      svalue=TRIM(svalue1)
   end if
   if (.not.lerr2) then
      lerr=.false.
      ipos=max(ipos,ipos2)
      svalue=TRIM(svalue2)
   end if
   if (.not.lerr3) then
      lerr=.false.
      ipos=max(ipos,ipos3)
      svalue=TRIM(svalue3)
   end if
   if (.not.lerr4) then
      lerr=.false.
      ipos=max(ipos,ipos4)
      svalue=TRIM(svalue4)
   end if

   string2par=ipos

end function string2par


integer function string2par2(string,keyword,svalue,lerr,lsep,rsep,delimiter)

   implicit none
   character(*), intent(in)  :: string, keyword
   character(*), intent(out) :: svalue
   logical, intent(out)      :: lerr
   character(1), intent(in)  :: lsep, rsep
   character(1), intent(in), optional :: delimiter

   character(cdim+2) :: temp, temp2, stringtemp
   integer           :: ipos, iend, lkey
   character(1)      :: deliml, delimr

   if (present(delimiter)) then
      deliml=delimiter
      delimr=delimiter
   else
      deliml=""
      delimr=rsep
   end if

   ! Add lsep at beginnig of string and rsep at end of string
   stringtemp=lsep//TRIM(ADJUSTL(string))//rsep
   temp=lsep//TRIM(keyword)//"="//deliml
   lerr=.false.
   lkey=len_trim(keyword)+1+1+len_trim(deliml)

   ! ipos is the position of temp(1) from the end of stringtemp
   if (debug) write(0,*) 'string2par: look for :',trim(temp),': in :', &
                         trim(stringtemp)
   ipos=index(TRIM(stringtemp),TRIM(temp),.true.)
   iend=index(stringtemp(ipos+lkey:),delimr,.false.)
   if ( ipos==0 .or. iend==0 .or. (ipos+iend-1)>cdim ) then
      lerr=.true.
      svalue=""
      string2par2=0
      if (debug) write(0,*) 'Error string2par',ipos,iend
      return
   end if
   iend=iend+ipos+lkey-2
   ipos=ipos+lkey
   if (ipos>iend) then
      svalue=""
      lerr=.true.
      string2par2=0
   else
      svalue=stringtemp(ipos:iend)
      string2par2=ipos
   end if
   if (debug) write(0,*) 'string2par: found :',trim(svalue),': in :',  &
                          trim(stringtemp)

end function string2par2



!-------------------------------------------------------------------------------
!                      STRING2VALUE / VALUE2STRING ROUTINES
!-------------------------------------------------------------------------------
real function string2real(string,lerr)

   implicit none
   character(*), intent(in)       :: string
   logical, intent(out), optional :: lerr

   real :: rvalue

   if (present(lerr)) lerr=.false.
   read(string,*,err=80) rvalue
   string2real=rvalue
   return
80 if (present(lerr)) lerr=.true.

end function string2real


integer function string2integer(string,lerr)

   implicit none
   character(*), intent(in)       :: string
   logical, intent(out), optional :: lerr

   if (debug) write(0,*) 'string2integer :',TRIM(string)
   if (present(lerr)) lerr=.false.
   read(string,*,err=80) string2integer
   return
80 if (present(lerr)) lerr=.true.

end function string2integer


character(cdim) function real2string(value)

   real, intent(in) :: value

   write(real2string,*) value
   real2string=TRIM(ADJUSTL(real2string))

end function real2string


character(cdim) function integer2string(value)

   integer, intent(in) :: value

   write(integer2string,*) value
   integer2string=TRIM(ADJUSTL(integer2string))

end function integer2string


character(cdim) function logical2string(value)

   logical, intent(in) :: value

   if (value) then
      logical2string="1"
   else
      logical2string="0"
   end if

end function logical2string


end module parser
!----------------
