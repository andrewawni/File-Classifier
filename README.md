# File-Classifier

a script that classifies all your files according to their extension.

## How to use:
    classify [-option][directory] ... -c [extentsion][category]
### Options:
- [-df] : default 

    classify the current directory only, ignores any sub-directories.
    
- [-rk] : recursevily keep

    classify the current directory and all of its sub-directories, keeping everything in its own directory
    
- [-rr] : recursevily remove

    moves the content of all subdirectories to the root directory and classifies it afterwards.
    
- [-c] [file extension] [file type]

    defines a custom file extension, e.g: classifier -c .py Python-Scripts
    
### e.g:

```classify -rk /path/to/directory -c .sh shell-scripts```

```classify -df /path/to/directory```
         
## Example: 
  - before:
  
        testingDir
        ├── A
        │   ├── C
        │   │   ├── documents
        │   │   │   └── document.pdf
        │   │   └── K
        │   │       ├── music
        │   │       │   └── music.mp3
        │   │       └── videos
        │   │           └── movie.mkv
        │   └── videos
        │       └── movie.mp4
        ├── archives
        │   └── archive.rar
        ├── B
        │   ├── J
        │   │   └── documents
        │   │       └── chapter1.pdf
        │   └── Pictures
        │       └── image.png
        ├── D
        │   └── documents
        │       └── text.docx
        ├── E
        │   ├── Pictures
        │   │   └── picture.png
        │   └── videos
        │       └── movie.mkv
        └── F
        
  - ```classify -rk testingDir```
    
          testingDir
          ├── A
          │   ├── C
          │   │   ├── documents
          │   │   │   └── document.pdf
          │   │   └── K
          │   │       ├── music
          │   │       │   └── music.mp3
          │   │       └── videos
          │   │           └── movie.mkv
          │   └── videos
          │       └── movie.mp4
          ├── archives
          │   └── archive.rar
          ├── B
          │   ├── J
          │   │   └── documents
          │   │       └── chapter1.pdf
          │   └── Pictures
          │       └── image.png
          ├── D
          │   └── documents
          │       └── text.docx
          ├── E
          │   ├── Pictures
          │   │   └── picture.png
          │   └── videos
          │       └── movie.mkv
          └── F
          
  - ```classify -rr testingDir .py Python-scripts```
  
        testingDir
        ├── archives
        │   └── archive.rar
        ├── documents
        │   ├── chapter1.pdf
        │   ├── document.pdf
        │   └── text.docx
        ├── music
        │   └── music.mp3
        ├── Pictures
        │   ├── image.png
        │   └── picture.png
        ├── Python-scripts
        │   └── script.py
        └── videos
            ├── movie.mkv
            └── movie.mp4
