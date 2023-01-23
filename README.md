# linux-playground

### 1. du - implemented an advanced version of ls command for linux-like operating systems in c.
  arguments:
  - -k
    - Report the number of blocks each entry is allocated in the filesystem .
  - -t [threshold]
    - Filter files based on a lower-bound threshold of > [threshold] bytes.
  - [file | directory]
    - Run the command on a specific file, or on a different directory than where it was originally called.
  -r
    - Recursively follow all subdirectories, outputting entries for each of their files, and including an output line for each subdirectory total.

    USAGE: `./du -k -t <threashold_bytes> [file | directory]`

### 2. shell - implemented shell in c
  features:
  - basic command execution like ls, grep etc.
  - I/O redirection
    > eg: ls > a.txt
  - Pipes
    > eg: cat words.txt | grep cat | sed s/cat/dog/ > doggerel.txt 
  
 ### 3. Implemented nice system call, pseudo random number generator in xv6 (a linux like operating system for learning).
 
 ### 4. implemented mutex, spinlocks for thread safe global variable modification.
 
 ### 5. Implemented uniq system call.
 
 ### 6. run.c - read/write blockSize*blockCount bytes from/to a file, given a blockSize and blockCount and display read/write performance in MB/s.  
  USAGE: `./run <filename> [-r|-w] <block_size> <block_count>`  
    
 ### 7. run2.c find a fileSize that can be read in reasonable time (hardcoded) for system performance analysis for various block sizes  
  USAGE: `./run2 <filename> <block_size>`
    
 ### 8. fast.c - read a very large file in the fastest possible way, used optimal blockSize(system specific) and multithreading  
  USAGE: `./fast <filename>`
 
 
 
 
 
