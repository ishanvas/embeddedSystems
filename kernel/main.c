/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Author: Ishan Vashishtha <ivashish@andrew.cmu.edu>
 * Date:   Mon, 20 Oct 2014
 */

#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>

#include <new_swi.h>
#include <types.h>
#include <exports.h>
#include <exit.h>
#include <bits/errno.h>
#include <timer_driver.h> 


/* prototype declarations fro read and write */
ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int, void *, size_t);


uint32_t global_data;

uint32_t sys_time =0;

unsigned * get_handler_ptr(unsigned vector_address)
{
  unsigned *vector = (unsigned *) vector_address;
  unsigned vec_off = (*vector & 0x00000FFF); /* getting the last 12 bits from the instruction*/
  unsigned *table_ptr =  (unsigned *)(0x8 + vector_address +vec_off); /* adding 8 since pc is pointing 8 bytes ahead*/
  unsigned *swi_ptr = (unsigned *) (*table_ptr); /* pointer to the defaukt swi */

  return swi_ptr;
}

void install_redirect_ins(unsigned* hdlr_ptr, unsigned new_swi_adr, unsigned ins[])
{
  unsigned offset = 0x004;

  ins[0]=*hdlr_ptr; /* getting old instruction in old_ptr1*/
  ins[1]=*(hdlr_ptr+1); /* getting old instruction in old_ptr2*/
  *hdlr_ptr=(offset|0xe51FF000); /* storing ldr pc, [pc,#-4]*/
  *(hdlr_ptr+1)=new_swi_adr; /* storing address of new swi handler*/	
}


void install_handler(unsigned vector_address, unsigned new_swi_adr, unsigned ins[])
{
  unsigned* hdlr_ptr = get_handler_ptr(vector_address); 
  install_redirect_ins(hdlr_ptr,new_swi_adr,ins);
}


void restore_handler(unsigned vector_address, unsigned ins[])
{
  unsigned* hdlr_ptr = get_handler_ptr(vector_address); 
  
  *hdlr_ptr=ins[0]; 
  *(hdlr_ptr+1)=ins[1];
}


int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	unsigned swi_ins[2] ={0,0}; /* array to store replaced instructions */
	unsigned irq_ins[2] ={0,0}; /* array to store replaced instructions */
	unsigned new_swi_adr = (unsigned) New_S_Handler; /* pointer to new swi handler */
	unsigned new_irq_adr = (unsigned) New_IRQ_Handler; /* pointer to new swi handler */

	/* installs new swi handler */
	install_handler(0x08,new_swi_adr,swi_ins);

		/* installs new irq handler */
	install_handler(0x18,new_irq_adr,irq_ins);

	init_timer();
	setup_irq_stack();
	setup_stack(argc,argv); /* seting up user stack and calling user program */

	/* Restoring default swi handler */
	restore_handler(0x08,swi_ins);
	restore_handler(0x18,irq_ins);
	
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

void C_IRQ_Handler(unsigned swi_num, unsigned *regs){                                                               
                                                                                  
	sys_time += 1;

}

