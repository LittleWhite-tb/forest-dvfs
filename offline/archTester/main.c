#include <stdio.h>

// This code is based of CPUID from Todd Allen : http://www.etallen.com/cpuid.html

#define WORD_EAX  0
#define WORD_EBX  1
#define WORD_ECX  2
#define WORD_EDX  3

#define WORD_NUM  4
   
#define FOUR_CHARS_VALUE(s) \
   ((unsigned int)((s)[0] + ((s)[1] << 8) + ((s)[2] << 16) + ((s)[3] << 24)))
#define IS_VENDOR_ID(words, s)                        \
   (   (words)[WORD_EBX] == FOUR_CHARS_VALUE(&(s)[0]) \
    && (words)[WORD_EDX] == FOUR_CHARS_VALUE(&(s)[4]) \
    && (words)[WORD_ECX] == FOUR_CHARS_VALUE(&(s)[8]))
    
#define BPI  32
#define POWER2(power) \
   (1 << (power))
#define RIGHTMASK(width) \
   (((width) >= BPI) ? ~0 : POWER2(width)-1)
#define BIT_EXTRACT_LE(value, start, after) \
   (((value) & RIGHTMASK(after)) >> start)

void asmCPUID(unsigned int eax, unsigned int words[WORD_NUM])
{
   asm("cpuid"
      : "=a" (words[WORD_EAX]),
        "=b" (words[WORD_EBX]),
        "=c" (words[WORD_ECX]),
        "=d" (words[WORD_EDX])
      : "a" (eax),
        "c" (0));
}

int main(void)
{
   unsigned int words[WORD_NUM];
   
   // EAX == 0 To have processor vendor
   asmCPUID(0,words);

   printf("   vendor_id = \"%-4.4s%-4.4s%-4.4s\"\n",
             (const char*)&words[WORD_EBX], 
             (const char*)&words[WORD_EDX], 
             (const char*)&words[WORD_ECX]);

   if (IS_VENDOR_ID(words, "GenuineIntel")) 
   {
      // Get processor type
      asmCPUID(1,words);
      
      unsigned int family = BIT_EXTRACT_LE(words[WORD_EAX],
                                           8,
                                           11 + 1);
      printf("\tFamily : %u\n",family);
      unsigned int model = BIT_EXTRACT_LE(words[WORD_EAX],
                                           4,
                                           7 + 1);
      printf("\tModel : %u\n",model);
      unsigned int stepping = BIT_EXTRACT_LE(words[WORD_EAX],
                                           0,
                                           3 + 1);
      printf("\tStepping : %u\n",stepping);
      unsigned int extFamily = BIT_EXTRACT_LE(words[WORD_EAX],
                                           20,
                                           27 + 1);
      printf("\tExt Family : %u\n",extFamily);
      unsigned int extModel = BIT_EXTRACT_LE(words[WORD_EAX],
                                           16,
                                           19 + 1);
      printf("\text Model : %u\n",extModel);
      
      if ( extFamily == 0 &&
           extModel == 2 && 
           family == 6 && 
           (model == 10 || model == 13) )
      {
         printf("It's a sandy bridge\n");
         return 1;
      }
      else
      {
         printf("You CPU is not a sandy bridge\n");
         return 0;
      }
   }
   else
   {
      printf("This test is only to detect Intel micro architecture\n");
      return 0;
   }

   return 0;
}
