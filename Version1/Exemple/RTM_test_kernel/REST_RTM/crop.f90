
  subroutine crop(cv1,cv2,cv3,cv4,cv5,cv6,vmin,vmax,v1,v2,v3,v4,v5,v6)
    !
    integer,intent(out)       :: cv1,cv2,cv3,cv4,cv5,cv6
    integer,intent(in)        :: vmin,vmax,v1,v2,v3,v4,v5,v6


    if (v2.ne.-2.and.v2>=vmin) then
       cv1=max(v1,vmin)
       cv2=min(v2,vmax)
    else
       cv1=-1
       cv2=-2
    endif
    if (v3.ne.-3.and.v4>=vmin) then
       cv3=max(v3,vmin)
       cv4=min(v4,vmax)
    else
       cv3=-3
       cv4=-4
    endif
    if (v5.ne.-5.and.v5<=vmax) then
       cv5=max(v5,vmin)
       cv6=min(v6,vmax)
    else
       cv5=-5
       cv6=-6
    endif
  end subroutine crop


