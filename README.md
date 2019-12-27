# ConcPicProcessing
concurrent picture processor written in C

Run make.
Then, run ./concurrent_picture_lib.
Provide it arguments to load pictures on start-up.
The following commands are available:

    liststore
    load <file_path > <file_name >
    unload <file_name >
    save <file_name > <file_path >

    invert <file_name>
    grayscale <file_name>
    rotate [90|180|270] <file_name>
    flip [H|V] <file_name>
    blur <file_name>
    
A file path must be either relative or absolute. 
All pictures must be *.jpg. 

The blur experiment is testing different concurrent approaches to process a blur command.
Having a fixed number of threads turned out to be optimal. Ideally, as many theads as cores on your machine.
