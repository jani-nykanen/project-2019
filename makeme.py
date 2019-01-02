# Generates makefiles for Cpp projects
# (c) 2018 Jani Nykänen

import sys
import os

# Get a list of all files
def getFileList(dirName):
    fileList = os.listdir(dirName)
    files = list()
    # Go through files
    for f in fileList:
        fullPath = os.path.join(dirName, f)
        if os.path.isdir(fullPath):
            files = files + getFileList(fullPath)
        else:
            files.append(fullPath)
    return files   
    
    
# Remove files with other than .cpp prefix
def filterFileList(files):
    ret = list();
    for f in files:
        if f[-4:] == ".cpp":
            ret.append(f);
         
    return ret;


# Write a makefile
def writeMakefile(outDir, files, isLib, output, ldflags, ccflags):
    f = open("makefile", "w");
    
    # Write comment
    f.write("# Makefile generated by makeme.py\n")
    
    # "All"
    f.write("all: " + output + " clean\n")
    
    # Write object files
    f.write("OBJS := ");
    for o in files:
        f.write(outDir + o[len(outDir):-4] + ".o")
        f.write(" ")
        pass
    f.write("\n");
    
    # Write flags
    f.write("LD_FLAGS := " + ldflags + "\n")
    f.write("CC_FLAGS := " + ccflags + "\n")
    
    # If not a library, set target
    if not isLib:
        f.write(output + ": $(OBJS)\n")
        f.write("\t")
        f.write("g++ $(CC_FLAGS) -o $@ $^ $(LD_FLAGS)")
        f.write("\n");
    else:
        # Build instructions for objects
        f.write("%.o: %.cpp\n")
        f.write("\tg++ $(CC_FLAGS) -c -o $@ $^ $(LD_FLAGS)\n")
        # Link & create an archive
        f.write(output + ": $(OBJS)\n")
        f.write("\tar rcs ../libengine.a $(OBJS)\n")
    
    # Clean
    f.write("clean:\n");
    f.write("\tfind . -type f -name '*.o' -delete\n");
    
    f.close();
    pass


# Go through parameters
arr = sys.argv[1:len(sys.argv)]
isLib = False
output = ""
ldflags = ""
ccflags = ""
files = ""
path = ""
for arg in arr:
    

    # Check if flag
    if arg[0] == "-":
        s = arg[1:]
        
        # Check if a library
        if s == "lib":
            isLib = True
        elif s == "bin":
            isLib = False
          
        # Check if special flag
        elif len(s) >= 5:
        
            # Check if an output directory
            if s[:4] == "out:":
                output = s[4:]
                
            # Check if a list of LD flags
            elif s[:4] == "ldf:":
                ldflags = s[4:]
                
            # Check if a list of CC flags
            elif s[:4] == "ccf:":
                ccflags = s[4:] 

    else:
    
        # Set path
        path = arg;
    
        # Get compiled files
        files = filterFileList(getFileList(path));

        # Create a makefile
        writeMakefile(path, files, isLib, output, ldflags, ccflags)
