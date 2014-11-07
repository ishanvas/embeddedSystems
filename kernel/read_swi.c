#include <exports.h>
#include <types.h>
#include <bits/errno.h>


ssize_t read(int fd, void *buf, size_t count)       
{                                                    
        /* returning error in case fd doesn't point to stdin*/
        if(fd!=0)                                           
        {                                                   
                return -EBADF;                                  
        }                                                   
                                                            
        /* allowed ranges */
        unsigned int start_heap = 0xa2000000; 
                                                     
        unsigned int end_heap = 0xa2ffffff;  
                                             
        char* bufptr = (char *)buf;          
                                             
        /* checking if the buffer is the allowed area of memory*/
        if( (unsigned int)&buf < start_heap || ( ((unsigned int)(buf)+count ) > end_heap ) )
        {                                                                 
                return -EFAULT;                                                
        }                                                                 
                                                                          
        unsigned int i =0;                                                         
	       /* Start getting data from stdin*/
        for (i = 0; i < count; ++i)                                       
                {                                                                 
                        char c = getc();                                          
                                                                                  
                        if(c == 4 )/* checking for EOT */
                        {                                              
                                break;                                            
                        }                                                         
                        else if ((c == 8)||(c == 127)) /* checking for delete and backsp */
                        {                                                               
                                i--;                                                    
                                puts("\b \b");                                           
                        }                                                               
                        else /* otherwise */
                        {                                                         
                                                                                        
                                if ((c == 10)||(c == 13)) /* breaks if new line or CR */
                                {               
					                             bufptr[i] = '\n';                                      
					                             putc('\n');
				   	                           i++;					/* since we are breaking value of i would be 1 less*/
                                       break;                                          


                                }
                        				else
                        				{
                        					bufptr[i] = c;
                        					putc(c);	
                        				}   
                         }                                                               
                                                                                                        
                    }                                                                       
                                                                                                        
          /* finally 'i' will contain count of characters read( written in buffer) */
                                                                                                        
                    return i;
 }

 
