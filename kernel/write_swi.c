#include <exports.h>
#include <types.h>
#include <bits/errno.h>


/* SWI Handler for write */
 ssize_t write(int fd, const void *buf, size_t count)                            
 {                                                                               
         // returning error in case fd doesn't point to stdout                   
         if(fd != 1)                                                             
         {                                                                       
                 return -EBADF;                                                      
         }                                                                       

        /* allowed ranges */                                                                                 
         unsigned int start_allow = 0xa0000000;                                   
         unsigned int end_allow = 0xa3efffff;   
                                                                                 
         char* bufptr = (char*)buf;                                              
                                                                                 
         // checking if the buffer is the allowed area of memory                 
         if( (unsigned int)buf < start_allow || ( ( (unsigned int)(buf)+count ) > end_allow ) )      
         {                                                                       
                 return -EFAULT;                                                      
         }     
                                                                                 
         // Start getting data from stdin                                        
         unsigned int i =0; 

         for (i = 0; i < count; ++i)                                             
                 {                                                                       
                        char c = bufptr[i];
                        /* prints \n for CR or LF */
			                  if ((c == 10)||(c == 13))
                  			{
                  				putc('\n');
                  			} 
                  			else{
                  				putc(c);                                                
                  			}
                 }                                                                       
                                                                                         
                 // finally 'i' will contain count of characters                         
                                                                                         
     return i;                                                               
  }                                                                   
