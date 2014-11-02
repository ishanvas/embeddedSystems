#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Author: Ishan Vashishtha <ivashish@andrew.cmu.edu>
 * Date:   Mon, 20 Oct 2014
 */
#include <new_swi.h>
#include <types.h>
#include <exports.h>
#include <exit.h>
#include <bits/errno.h>

/* prototype declarations fro read and write */
ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int, void *, size_t);



uint32_t global_data;

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	unsigned *vector = (unsigned *) 0x08; /* pointer to 0x08 vector table */
	
    unsigned vec_off = (*vector & 0x00000FFF); /* getting the last 12 bits from the instruction*/

	unsigned *table_ptr =  (unsigned *)(0x10 +vec_off); /* adding 16 to the offset */

	unsigned *swi_ptr = (unsigned *) (*table_ptr); /* pointer to the defaukt swi */

	unsigned new_swi_adr = (unsigned) New_S_Handler; /* pointer to new swi handler */

	volatile unsigned old_ins1, old_ins2; /* for storing old instructions, dont want to keep the values in reg */
	unsigned offset = 0x004;

	old_ins1=*swi_ptr; /* getting old instruction in old_ptr1*/
	old_ins2=*(swi_ptr+1); /* getting old instruction in old_ptr2*/

	*swi_ptr=(offset|0xe51FF000); /* storing ldr pc, [pc,#-4]*/
	*(swi_ptr+1)=new_swi_adr; /* storing address of new swi handler*/

	setup_stack(argc,argv); /* seting up user stack and calling user program */

/* Restoring to old values */
	 *swi_ptr=old_ins1; 
   *(swi_ptr+1)=old_ins2;


	return 0;
}

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


 ssize_t read(int fd, void *buf, size_t count)       
{                                                    
        // returning error in case fd doesn't point to stdin
        if(fd!=0)                                           
        {                                                   
                return -EBADF;                                  
        }                                                   
                                                            
        /* allowed ranges */
        unsigned int start_heap = 0xa2000000; 
                                                     
        unsigned int end_heap = 0xa2ffffff;  
                                             
        char* bufptr = (char *)buf;          
                                             
        // checking if the buffer is the allowed area of memory
        if( (unsigned int)&buf < start_heap || ( ((unsigned int)(buf)+count ) > end_heap ) )
        {                                                                 
                return -EFAULT;                                                
        }                                                                 
                                                                          
        unsigned int i =0;                                                         
	// Start getting data from stdin
        for (i = 0; i < count; ++i)                                       
                {                                                                 
                        char c = getc();                                          
                                                                                  
                        if(c == 4 )// checking for EOT                            
                        {                                              
                                break;                                            
                        }                                                         
                        else if ((c == 8)||(c == 127)) // checking for delete and backsp
                        {                                                               
                                i--;                                                    
                                puts("\b \b");                                           
                        }                                                               
                        else // otherwise                                               
                        {                                                         
                                                                                        
                                if ((c == 10)||(c == 13)) // breaks if new line or CR
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
                                                                                                        
          // finally 'i' will contain count of characters read( written in buffer)
                                                                                                        
                    return i;
 }



/* Handles SWI requests
* based on the SWI number redirects to appropriate SWI handler */
void C_SWI_Handler(unsigned swi_num, unsigned *regs )
  {                                                               
                                                                                  
          int fd;                                     
          char *buff;                                           
          size_t count;                                           
          size_t readCount;                                       
          size_t writeCount;                                      

          /* subtracting base SWI address */                                                              
          swi_num = swi_num - (0x900000);                     

          /* Routing based on swi_num */
          switch(swi_num) {                                   
          case 1 :                      
                  /* passing the adress of registers, helps in no return */                                         
                  exit(regs);                  
                  break;                        
          case 3 :                                                
                  /* populating args from values on stack */
                  fd = (int)*regs;                                           
                  buff =(char *)*(regs+1);     
                  count = (int)*(regs+2);      
                  readCount = read(fd,buff,count);                                
                  *regs = readCount;                                              
                  break;                                          
                                                                
        case 4 :                                                
                /* populating args from values on stack */
                fd = (int)*regs;                            
                buff =(char *)*(regs+1);                    
                count = (int)*(regs+2);                     
                writeCount = write(fd,buff,count);                             
                *regs = writeCount;                                            
                break;                              
                                                  
        default :                                               
                break;                                                     
        }                                         

}      
