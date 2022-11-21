# IMAGE ENCRYPTION AND DECRYPTION PROGRAM          
                                                        
Welcome to image encryption and decryption program.      
This program changes binary data of image pixels to hide 
your message in it without any visual changes for human  
eye. It is a manual that shows all supported flags and   
file formats.                                            
                                                         
# Here are all supported flags:                            
                                                         
 -i (--info) + path to the file:                          
      checks if file format is supported by the program,  
      its dimensions, occupied space on disc              
      and timestamp of its last modification              
                                                          
 -e (--encrypt) + path to the file + \"message\":         
      encrypts a message in quotes inside the file        
                                                          
 -d (--decrypt) + path to the file:                       
      decrypts a message from the file                    
                                                          
 -c (--check) + path to the file + \"message\":           
      checks if message in quotes can fit into the file   
                                                          
 -h (--help):                                             
      print the manual of the program                     
                                                        
# Here are all supported file formats:                     
                                                          
  bmp(bi_rgb), ppm, pgm    
